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
    
def plot_neutrino_inelasticity():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    
    plot_model(ax, 'output/Kamae2006_inelasticity_nus.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_inelasticity_nus.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_inelasticity_nus.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_nus_inelasticity')
    
if __name__== "__main__":
    plot_neutrino_inelasticity()
