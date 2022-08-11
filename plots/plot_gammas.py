import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def plot_model(ax, filename, color, label, norm = 1.0):
    T, sigma_pp = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(T / np.max(T), norm * T * T * sigma_pp, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlabel('E$_{\gamma}$ / E$_{p}$')
    ax.set_xlim([1e-4, 1])
    ax.set_xscale('log')
    ax.set_ylabel(r'E$_{\gamma}^2$ d$\sigma$/dE$_{\gamma}$ [GeV mbarn]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_gammas_5GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '5 GeV')

    plot_model(ax, 'output/Kafexhiu2014G4_5GeV_gamma.txt', 'tab:olive', 'Kafexhiu2014 / GEANT4')
    #plot_model(ax, 'output/Kafexhiu2014P8_5GeV_gamma.txt', 'tab:pink', 'Kafexhiu2014 / PYTHIA8')
    #plot_model(ax, 'output/Kafexhiu2014Sibyll_5GeV_gamma.txt', 'tab:cyan', 'Kafexhiu2014 / SIBYLL')
    plot_model(ax, 'output/Kamae2006_5GeV_gamma.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/Kelner2006_5GeV_gamma.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 'output/AAFRAG_5GeV_gamma.txt', 'tab:brown', 'AAFRAG')

    ax.set_ylim([1e-3, 4e1])
    ax.legend(fontsize=16)
    plt.savefig('xsecs_gammas_5GeV.pdf')
    
def plot_gammas_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/Kafexhiu2014G4_10GeV_gamma.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    #plot_model(ax, 'output/Kafexhiu2014P8_10GeV_gamma.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    #plot_model(ax, 'output/Kafexhiu2014Sibyll_10GeV_gamma.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 'output/Kamae2006_10GeV_gamma.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_10GeV_gamma.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_10GeV_gamma.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e-1, 5e1])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_gammas_10GeV.pdf')

def plot_neutrinos_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/Kamae2006_10GeV_nu.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_10GeV_nu.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_10GeV_nu.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e-1, 5e1])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_neutrinos_10GeV.pdf')

def plot_gammas_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/Kafexhiu2014G4_100GeV_gamma.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 'output/Kafexhiu2014P8_100GeV_gamma.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 'output/Kafexhiu2014Sibyll_100GeV_gamma.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 'output/Kamae2006_100GeV_gamma.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_100GeV_gamma.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_100GeV_gamma.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e0, 5e2])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_gammas_100GeV.pdf')

def plot_neutrinos_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/Kamae2006_100GeV_nu.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_100GeV_nu.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_100GeV_nu.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e0, 5e2])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_neutrinos_100GeV.pdf')

def plot_gammas_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')
    
    plot_model(ax, 'output/Kafexhiu2014G4_1TeV_ph.txt', 'tab:olive', 'Kafexhiu2014/GEANT4')
    plot_model(ax, 'output/Kafexhiu2014P8_1TeV_ph.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8')
    plot_model(ax, 'output/Kafexhiu2014Sibyll_1TeV_ph.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL')
    plot_model(ax, 'output/Kamae2006_1TeV_ph.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_1TeV_ph.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_1TeV_ph.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e1, 5e3])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_gammas_1TeV.pdf')

def plot_neutrinos_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')
    
    plot_model(ax, 'output/Kamae2006_1TeV_nu.txt', 'tab:red', 'Kamae2006/PYTHIA6.2')
    plot_model(ax, 'output/Kelner2006_1TeV_nu.txt', 'tab:blue', 'Kelner2006/SIBYLL')
    plot_model(ax, 'output/AAFRAG_1TeV_nu.txt', 'tab:green', 'AAFRAG/QGSJET-II')

    ax.set_ylim([1e1, 5e3])
    ax.legend(fontsize=15)
    plt.savefig('xsecs_neutrinos_1TeV.pdf')

if __name__== "__main__":
    plot_gammas_5GeV()
    plot_gammas_10GeV()
    plot_neutrinos_10GeV()
    plot_gammas_100GeV()
    plot_neutrinos_100GeV()
    plot_gammas_1TeV()
    plot_neutrinos_1TeV()
