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
    T, qH, qHe = np.loadtxt(filename, usecols=(0,1,2), unpack=True, skiprows=1)
    ax.plot(T, norm * np.power(T, 2.7) * qHe, color=color, linestyle=':')
    ax.plot(T, norm * np.power(T, 2.7) * qH, color=color, label=label)
    #ax.plot(T, norm * np.power(T, 2.7) * 0.1 * qH, color=color, linestyle='--')

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e5])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\gamma}$ [GeV]')
    ax.set_ylabel(r'E$_{\gamma}^{2.7}$ q$_{\gamma}$ [a.u.]')
    ax.set_yscale('log')
    ax.set_ylim([1e-25, 1e-22])
    ax.set_title(title)
    return ax
    
def plot_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'interstellar source term')
    
    plot_model(ax, 'output/Kamae2006_ISM_source_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kafexhiu2014G4_ISM_source_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 'output/Kafexhiu2014P8_ISM_source_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 'output/Kafexhiu2014Sibyll_ISM_source_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 'output/Kelner2006_ISM_source_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_ISM_source_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=15)
    savefig(plt, 'xsecs_gammas_ISM_source')

if __name__== "__main__":
    plot_sourceterm()
