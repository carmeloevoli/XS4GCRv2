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
    T, q1, q2, q3 = np.loadtxt(filename, usecols=(0,1,2,3), unpack=True, skiprows=1)
    ax.plot(T, norm * np.power(T, 2.2) * q1, color=color, linestyle=':')
    ax.plot(T, norm * np.power(T, 2.5) * q2, color=color, label=label)
    ax.plot(T, norm * np.power(T, 2.8) * q3, color=color, linestyle='--')

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e5])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')
    ax.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')
    ax.set_yscale('log')
    ax.set_ylim([0.01, 50])
    ax.set_title(title)
    return ax
    
def plot_gammas_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'$\gamma$ source term')
    
    plot_model(ax, 'output/Kamae2006_source_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e20)
    plot_model(ax, 'output/Kafexhiu2014G4_source_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 1e20)
    plot_model(ax, 'output/Kafexhiu2014P8_source_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 1e20)
    plot_model(ax, 'output/Kafexhiu2014Sibyll_source_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 1e20)
    plot_model(ax, 'output/Kelner2006_source_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e20)
    plot_model(ax, 'output/AAFRAG_source_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e20)

    ax.legend(fontsize=10)
    savefig(plt, 'xsecs_gammas_source')

def plot_neutrinos_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'$\nu$ source term')
    
    plot_model(ax, 'output/Kamae2006_source_nus.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e20)
    plot_model(ax, 'output/Kelner2006_source_nus.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e20)
    plot_model(ax, 'output/AAFRAG_source_nus.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e20)

    ax.legend(fontsize=10)
    savefig(plt, 'xsecs_nus_source')

def plot_positrons_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'e$^+$ source term')

    plot_model(ax, 'output/Kamae2006_source_pos.txt', 'tab:red', 'Kamae2006', 1e20)
    plot_model(ax, 'output/AAFRAG_source_pos.txt', 'tab:brown', 'AAFRAG', 1e20)
    plot_model(ax, 'output/HuangPohl2007_source_pos.txt', 'tab:blue', 'HuangPohl2007', 1e20)
    plot_model(ax, 'output/Orusa2022_source_pos.txt', 'tab:green', 'Orusa2022', 1e20)

    ax.legend(fontsize=10)
    savefig(plt, 'xsecs_positrons_source')

def plot_antiprotons_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'$\bar p$ source term')

    plot_model(ax, 'output/AAFRAG_source_pbar.txt', 'tab:brown', 'AAFRAG', 1e20)
    plot_model(ax, 'output/DiMauro2014_source_pbar.txt', 'tab:red', 'DiMauro2014', 1e20)
    plot_model(ax, 'output/Duperray2003_source_pbar.txt', 'tab:green', 'Duperray2004', 1e20)
    plot_model(ax, 'output/Feng2016_EPOS_source_pbar.txt', 'tab:orange', 'Feng2016/EPOS', 1e20)
    plot_model(ax, 'output/Feng2016_QGSJET_source_pbar.txt', 'tab:blue', 'Feng2016/QGSJET', 1e20)
    plot_model(ax, 'output/TanNg1983_source_pbar.txt', 'tab:purple', 'TanNg1983', 1e20)
    plot_model(ax, 'output/Winkler2017_source_pbar.txt', 'tab:cyan', 'Winkler2017', 1e20)

    ax.legend(fontsize=10)
    savefig(plt, 'xsecs_antiprotons_source')
    
if __name__== "__main__":
    plot_antiprotons_sourceterm()
    plot_gammas_sourceterm()
    plot_neutrinos_sourceterm()
    plot_positrons_sourceterm()
