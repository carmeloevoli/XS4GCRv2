import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import matplotlib.colors as colors
from matplotlib.patches import Rectangle, Patch
from matplotlib.cm import ScalarMappable
import numpy as np

GHOST_FILE = '../data/Evoli2026/ghost_list_IAEA2026.txt'
CRAMS_FILE = '../data/crams_nucleilist.csv'

# proton number Z for each element symbol appearing in the ghost list
Z_OF = {
    'H': 1, 'He': 2, 'Li': 3, 'Be': 4, 'B': 5, 'C': 6, 'N': 7, 'O': 8,
    'F': 9, 'Ne': 10, 'Na': 11, 'Mg': 12, 'Al': 13, 'Si': 14, 'P': 15,
    'S': 16, 'Cl': 17, 'Ar': 18, 'K': 19, 'Ca': 20, 'Sc': 21, 'Ti': 22,
    'V': 23, 'Cr': 24, 'Mn': 25, 'Fe': 26, 'Co': 27, 'Ni': 28, 'Cu': 29,
    'Zn': 30,
}

# half-life unit -> seconds
UNIT_TO_S = {
    'ns': 1e-9, 'us': 1e-6, 'ms': 1e-3, 's': 1.0,
    'm': 60.0, 'h': 3600.0, 'd': 86400.0, 'y': 365.25 * 86400.0,
}

def savefig(plt, filename):
    filename = 'figs/' + filename + '.pdf'
    print('- saving plot on ' + filename)
    plt.savefig(filename)

def load_ghosts(filename):
    """Return dict {(Z, A): half_life_seconds} for unique parent ghost nuclei."""
    ghosts = {}
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            tok = line.split()
            # parent_el  parent_A  daughter_el  daughter_A  branching  hl_value  hl_unit
            el, A = tok[0], int(tok[1])
            hl = float(tok[5]) * UNIT_TO_S[tok[6]]
            ghosts[(Z_OF[el], A)] = hl
    return ghosts

def load_channels(filename):
    """Return list of (pZ, pA, dZ, dA) for every decay channel."""
    channels = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            tok = line.split()
            # parent_el parent_A daughter_el daughter_A branching hl_value hl_unit
            channels.append((Z_OF[tok[0]], int(tok[1]),
                             Z_OF[tok[2]], int(tok[3])))
    return channels

def load_crams(filename):
    """Return (stable, unstable) sets of (Z, A) tracked CRAMS nuclei.

    A nucleus is unstable if its beta half-life column (Myr) is positive;
    stable nuclei carry the placeholder value -1.
    """
    stable, unstable = set(), set()
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            tok = [t.strip() for t in line.split(',')]
            Z, A, hl = int(tok[0]), int(tok[1]), float(tok[3])
            if hl > 0:
                unstable.add((Z, A))
            else:
                stable.add((Z, A))
    return stable, unstable

def add_cell(ax, Z, A, **kwargs):
    ax.add_patch(Rectangle((A - Z - 0.5, Z - 0.5), 1, 1, **kwargs))

def plot_ghostdecay():
    ghosts = load_ghosts(GHOST_FILE)
    channels = load_channels(GHOST_FILE)
    stable, unstable = load_crams(CRAMS_FILE)

    Z = np.array([k[0] for k in ghosts])
    A = np.array([k[1] for k in ghosts])
    N = A - Z
    log_hl = np.log10(np.array([ghosts[k] for k in ghosts]))

    fig = plt.figure(figsize=(11. * (13. / 11.), 6.4 * (13. / 11.)))
    ax = fig.add_subplot(111)

    # clip the scale at 1 s: shorter timescales are irrelevant here
    norm = colors.Normalize(vmin=0.0, vmax=log_hl.max(), clip=True)
    cmap = plt.get_cmap('viridis')

    # ghost (short-lived) nuclei, coloured by half-life
    for n, z, lhl in zip(N, Z, log_hl):
        add_cell(ax, z, n + z, facecolor=cmap(norm(lhl)),
                 edgecolor='white', linewidth=0.4)

    # tracked CRAMS nuclei: stable (grey) and unstable (red)
    for z, a in stable:
        add_cell(ax, z, a, facecolor='0.6', edgecolor='white', linewidth=0.4)
    for z, a in unstable:
        add_cell(ax, z, a, facecolor='tab:red', edgecolor='white', linewidth=0.4)

    # arrow from each parent to its daughter
    for pZ, pA, dZ, dA in channels:
        ax.annotate('', xy=(dA - dZ, dZ), xytext=(pA - pZ, pZ),
                    arrowprops=dict(arrowstyle='->', color='xkcd:yellow',
                                    lw=1.1, alpha=1.0, mutation_scale=6,
                                    shrinkA=3, shrinkB=3))

    all_N = np.concatenate([N, [a - z for z, a in stable | unstable]])
    all_Z = np.concatenate([Z, [z for z, a in stable | unstable]])
    ax.set_xlim(all_N.min() - 1, all_N.max() + 1)
    ax.set_ylim(all_Z.min() - 1, all_Z.max() + 1)
    ax.set_aspect('equal')
    ax.set_xlabel(r'Neutron number $N$')
    ax.set_ylabel(r'Proton number $Z$')

    sm = ScalarMappable(norm=norm, cmap=cmap)
    sm.set_array([])
    # colorbar tied to the axes height so it never exceeds the y-axis
    cax = ax.inset_axes([1.02, 0.0, 0.025, 1.0])
    cbar = fig.colorbar(sm, cax=cax, extend='min')
    cbar.set_label(r'ghost $\log_{10}(t_{1/2}\,/\,{\rm s})$')

    handles = [
        Patch(facecolor='0.6', edgecolor='white', label='stable'),
        Patch(facecolor='tab:red', edgecolor='white',
              label='unstable\n' + r'$1\,{\rm kyr} < t_{1/2} < 100\,{\rm Myr}$'),
    ]
    leg = ax.legend(handles=handles, loc='lower right',
                    frameon=True, framealpha=1.0)
    leg.get_frame().set_edgecolor('none')
    leg.set_zorder(10)

    savefig(plt, 'ghostdecay')

if __name__ == "__main__":
    plot_ghostdecay()
