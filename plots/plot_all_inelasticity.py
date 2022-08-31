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
    T, inel = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(T, inel * 100., color=color, label=label)
    
def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e4])
    ax.set_ylim([0, 6])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\mathrm{p}}$ [GeV]')
    ax.set_ylabel(r'Y [\%]')
    ax.set_title(title)
    return ax
    
def plot_gamma_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kafexhiu2014G4_inelasticity_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 'output/Kafexhiu2014P8_inelasticity_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 'output/Kafexhiu2014Sibyll_inelasticity_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 'output/Kelner2006_inelasticity_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/Kamae2006_inelasticity_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/AAFRAG_inelasticity_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_gammas_inelasticity')
    
def plot_neutrino_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kamae2006_inelasticity_nus.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_inelasticity_nus.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_inelasticity_nus.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_nus_inelasticity')
    
def plot_positrons_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kamae2006_inelasticity_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/AAFRAG_inelasticity_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 'output/HuangPohl2007_inelasticity_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 'output/Orusa2022_inelasticity_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_pos_inelasticity')

def plot_antiprotons_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    ax.set_ylim([1., 20.])
    ax.set_yscale('log')

    plot_model(ax, 'output/AAFRAG_inelasticity_pbar.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 'output/DiMauro2014_inelasticity_pbar.txt', 'tab:red', 'DiMauro2014')
    plot_model(ax, 'output/Duperray2003_inelasticity_pbar.txt', 'tab:green', 'Duperray2004')
    plot_model(ax, 'output/Feng2016_EPOS_inelasticity_pbar.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 'output/Feng2016_QGSJET_inelasticity_pbar.txt', 'tab:blue', 'Feng2016/QGSJET')
    plot_model(ax, 'output/TanNg1983_inelasticity_pbar.txt', 'tab:purple', 'TanNg1983')
    plot_model(ax, 'output/Winkler2017_inelasticity_pbar.txt', 'tab:cyan', 'Winkler2017')
    
    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_pbar_inelasticity')

if __name__== "__main__":
    plot_antiprotons_inelasticity()
    plot_gamma_inelasticity()
    plot_neutrino_inelasticity()
    plot_positrons_inelasticity()
