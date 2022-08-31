import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def plot_model(ax, filename, color, label, norm = 1):
    T, yHe, yC, yFe = np.loadtxt(filename, usecols=(0,1,2,3), unpack=True, skiprows=1)
    ax.plot(T, norm * yHe, color=color, label=label)
    ax.plot(T, norm * yC, color=color, linestyle=':')
    ax.plot(T, norm * yFe, color=color, linestyle='--')

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlabel('T [GeV]')
    ax.set_xlim([1e-1, 1e4])
    ax.set_xscale('log')
    ax.set_ylabel(r'$\tau_{\rm in}$ [Myr]')
    ax.set_title(title)
    return ax

def plot_inelastic_timescales():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/CROSEC_in.txt', 'tab:red', 'CROSEC')
    plot_model(ax, 'output/Letaw1983_in.txt', 'tab:green', 'Letaw1983')
    plot_model(ax, 'output/Tripathi1999_in.txt', 'tab:blue', 'Tripathi1999')

    #ax.set_ylim([1e-6, 1e-2])
    ax.legend(fontsize=16)
    plt.savefig('inelastic_timescales.pdf')

if __name__== "__main__":
    plot_inelastic_timescales()
