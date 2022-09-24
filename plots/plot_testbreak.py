import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np
from scipy.misc import derivative

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def plot_model(ax, filename, color, label, norm = 1.0):
    T, q = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(T, norm * np.power(T, 2.8) * q, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e4])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')
    ax.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')
    ax.set_yscale('log')
    ax.set_ylim([1, 30])
    #ax.set_title(title)
    return ax
    
def plot_gammas_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '')
    
    ax.plot([1e3, 1e4], [20, 20], '--', color='tab:gray')
    ax.plot([1e1, 1e3], [2, 20], '--', color='tab:gray')
#    ax.plot([1e3, 1e3], [0.1, 100.], ':', color='tab:gray')

    plot_model(ax, 'output/Kamae2006_break_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e20)
#    plot_model(ax, 'output/Kafexhiu2014G4_break_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 1e20)
    plot_model(ax, 'output/Kafexhiu2014P8_break_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 1e20)
    plot_model(ax, 'output/Kafexhiu2014Sibyll_break_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 1e20)
 #   plot_model(ax, 'output/Kelner2006_break_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e20)
    plot_model(ax, 'output/AAFRAG_break_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e20)

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_testbreak')
    
if __name__== "__main__":
    plot_gammas_sourceterm()
