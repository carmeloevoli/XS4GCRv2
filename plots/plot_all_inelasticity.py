import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def plot_model(ax, filename, color, label, norm = 1.0):
    Es, Ep = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(Es, Ep / Es, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e5])
    ax.set_ylim([5, 25])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\mathrm{s}}$ [GeV]')
    ax.set_ylabel(r'$\langle$E$_{\mathrm{p}}$$\rangle$ / E$_{\mathrm{s}}$')
    #ax.set_title(title)
    return ax
    
def plot_gamma_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kafexhiu2014G4_gammas_primary.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 100.)
    plot_model(ax, 'output/Kafexhiu2014P8_gammas_primary.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 100.)
    plot_model(ax, 'output/Kafexhiu2014Sibyll_gammas_primary.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 100.)
    plot_model(ax, 'output/Kelner2006_gammas_primary.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/Kamae2006_gammas_primary.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 100.)
    #plot_model(ax, 'output/AAFRAG_gammas_primary.txt', 'tab:green', 'AAFRAG/QGSJET-II', 100.)

    ax.hlines(1e1, 1e5, 10, ls=':', color='tab:gray')
    
    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_gammas_inelasticity')

def plot_neutrino_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kamae2006_nus_inelasticity.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 100.)
    plot_model(ax, 'output/Kelner2006_nus_inelasticity.txt', 'tab:blue', 'Kelner2006/SIBYLL', 100.)
    plot_model(ax, 'output/AAFRAG_nus_inelasticity.txt', 'tab:green', 'AAFRAG/QGSJET-II', 100.)

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_nus_inelasticity')
    
def plot_positrons_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kamae2006_pos_inelasticity.txt', 'tab:red', 'Kamae2006', 100.)
    plot_model(ax, 'output/AAFRAG_pos_inelasticity.txt', 'tab:brown', 'AAFRAG', 100.)
    plot_model(ax, 'output/HuangPohl2007_pos_inelasticity.txt', 'tab:blue', 'HuangPohl2007', 100.)
    plot_model(ax, 'output/Orusa2022_pos_inelasticity.txt', 'tab:green', 'Orusa2022', 100.)

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_pos_inelasticity')

def plot_antiprotons_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    ax.set_ylim([1., 20.])
    #ax.set_yscale('log')

    plot_model(ax, 'output/AAFRAG_pbar_inelasticity.txt', 'tab:brown', 'AAFRAG', 100.)
    plot_model(ax, 'output/Korsmeier2018_pbar_inelasticity.txt', 'tab:green', 'Korsmeier2018', 100.)
    plot_model(ax, 'output/Feng2016_EPOS_pbar_inelasticity.txt', 'tab:orange', 'Feng2016/EPOS', 100.)
    plot_model(ax, 'output/Feng2016_QGSJET_pbar_inelasticity.txt', 'tab:blue', 'Feng2016/QGSJET', 100.)
    plot_model(ax, 'output/Winkler2017_pbar_inelasticity.txt', 'tab:cyan', 'Winkler2017', 100.)
    
    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_pbar_inelasticity')

def plot_tertiary_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    ax.set_ylim([1., 40.])
    #ax.set_yscale('log')

    plot_model(ax, 'output/AAFRAG_inelasticity_terp.txt', 'tab:brown', 'AAFRAG', 100.)
    
    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_terp_inelasticity')

def plot_all_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    ax.set_ylim([0.3, 30.])
    ax.set_yscale('log')
    
    #plot_model(ax, 'output/AAFRAG_terp_inelasticity.txt', 'tab:gray', 'p', 100.)
    plot_model(ax, 'output/Korsmeier2018_pbar_inelasticity.txt', 'tab:red', r'$\bar{p}$', 100.)
    plot_model(ax, 'output/Orusa2022_pos_inelasticity.txt', 'tab:orange', r'$e^+$', 100.)
    plot_model(ax, 'output/AAFRAG_nus_inelasticity.txt', 'tab:green', r'$\nu$', 100.)
    plot_model(ax, 'output/AAFRAG_gammas_inelasticity.txt', 'tab:brown', r'$\gamma$', 100.)

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_all_inelasticity')

if __name__== "__main__":
    plot_gamma_inelasticity()
#    plot_neutrino_inelasticity()
#    plot_positrons_inelasticity()
#    plot_antiprotons_inelasticity()
#    plot_tertiary_inelasticity()
#    plot_all_inelasticity()
