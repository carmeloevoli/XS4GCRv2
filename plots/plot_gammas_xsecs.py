import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def plot_model(ax, icol, filename, color, label, norm = 1.0):
    x, sigma_pp = np.loadtxt(filename, usecols=(0,icol), unpack=True, skiprows=1)
    ax.plot(x, x * x * sigma_pp, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e-5, 1])
    ax.set_xscale('log')
    ax.set_xlabel(r'x')
    ax.set_ylabel(r'x$^2$ d$\sigma$/dx [mbarn]')
    ax.set_yscale('log')
    ax.set_ylim([1e-2, 1e1])
    ax.set_title(title)
    return ax

def plot_gammas_5GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 5 GeV')

    plot_model(ax, 1, 'output/Kafexhiu2014G4_xsecs_gammas.txt', 'tab:olive', 'Kafexhiu2014 / GEANT4')
    plot_model(ax, 1, 'output/Kamae2006_xsecs_gammas.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 1, 'output/Kelner2006_xsecs_gammas.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 1, 'output/AAFRAG_xsecs_gammas.txt', 'tab:brown', 'AAFRAG')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_5GeV')

def plot_gammas_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 GeV')

    plot_model(ax, 5, 'output/Kafexhiu2014G4_xsecs_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 5, 'output/Kamae2006_xsecs_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 5, 'output/Kelner2006_xsecs_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 5, 'output/AAFRAG_xsecs_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_10GeV')
    
def plot_gammas_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 100 GeV')

    plot_model(ax, 9, 'output/Kafexhiu2014G4_xsecs_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 9, 'output/Kafexhiu2014P8_xsecs_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 9, 'output/Kafexhiu2014Sibyll_xsecs_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 9, 'output/Kamae2006_xsecs_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 9, 'output/Kelner2006_xsecs_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 9, 'output/AAFRAG_xsecs_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_100GeV')

def plot_gammas_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 1 TeV')

    plot_model(ax, 13, 'output/Kafexhiu2014G4_xsecs_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 13, 'output/Kafexhiu2014P8_xsecs_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 13, 'output/Kafexhiu2014Sibyll_xsecs_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 13, 'output/Kamae2006_xsecs_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 13, 'output/Kelner2006_xsecs_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 13, 'output/AAFRAG_xsecs_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_1TeV')

def plot_gammas_10TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 TeV')

    plot_model(ax, 17, 'output/Kafexhiu2014G4_xsecs_gammas.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 17, 'output/Kafexhiu2014P8_xsecs_gammas.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 17, 'output/Kafexhiu2014Sibyll_xsecs_gammas.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 17, 'output/Kamae2006_xsecs_gammas.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 17, 'output/Kelner2006_xsecs_gammas.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 17, 'output/AAFRAG_xsecs_gammas.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_gammas_10TeV')

if __name__== "__main__":
#    plot_gammas_5GeV()
    plot_gammas_10GeV()
    plot_gammas_100GeV()
    plot_gammas_1TeV()
    plot_gammas_10TeV()
