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
    ax.set_xlabel(r'E$_{\gamma}$ [GeV]')
    ax.set_ylabel(r'E$_{\gamma}^\alpha$ q$_{\gamma}$ [a.u.]')
    ax.set_yscale('log')
    ax.set_ylim([0.06, 10])
    ax.set_title(title)
    return ax
    
def plot_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'source term')
    
    plot_model(ax, 'output/Kamae2006_source_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e18)
    plot_model(ax, 'output/Kafexhiu2014G4_source_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 1e18)
    plot_model(ax, 'output/Kafexhiu2014P8_source_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 1e18)
    plot_model(ax, 'output/Kafexhiu2014Sibyll_source_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 1e18)
    plot_model(ax, 'output/Kelner2006_source_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e18)
    plot_model(ax, 'output/AAFRAG_source_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e18)

    ax.legend(fontsize=10)
    savefig(plt, 'xsecs_gammas_source')

if __name__== "__main__":
    plot_sourceterm()
