import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def plot_model(ax, filename, color, label, norm = 1):
    T, sigma_pp = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(T / np.max(T), norm * T * sigma_pp, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlabel('$T_{\overline{p}}$ / $T_{p}$')
    ax.set_xlim([1e-3, 1])
    ax.set_xscale('log')
    ax.set_ylabel(r'$T d\sigma/dT$ [mbarn]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_xsecs_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/AAFRAG_10GeV_ap.txt', 'tab:orange', 'AAFRAG')
    plot_model(ax, 'output/TanNg1983_10GeV_ap.txt', 'tab:red', 'TanNg1983')
    plot_model(ax, 'output/DiMauro2014_10GeV_ap.txt', 'tab:green', 'DiMauro2014')
    plot_model(ax, 'output/Duperray2003_10GeV_ap.txt', 'tab:blue', 'Duperray2003')
    plot_model(ax, 'output/Winkler2017_10GeV_ap.txt', 'tab:brown', 'Winkler2017')
    #plot_model(ax, 'output/Feng2016_EPOS_10GeV_ap.txt', 'tab:purple', 'Feng2016 (EPOS)')
    #plot_model(ax, 'output/Feng2016_QGSJET_10GeV_ap.txt', 'tab:orange', 'Feng2016 (QGSJET)')

    ax.set_ylim([1e-6, 1e-2])
    ax.legend(fontsize=16)
    plt.savefig('ap_xsecs_10GeV.pdf')

def plot_xsecs_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/AAFRAG_100GeV_ap.txt', 'tab:orange', 'AAFRAG')
    plot_model(ax, 'output/TanNg1983_100GeV_ap.txt', 'tab:red', 'TanNg1983')
    plot_model(ax, 'output/DiMauro2014_100GeV_ap.txt', 'tab:green', 'DiMauro2014')
    plot_model(ax, 'output/Duperray2003_100GeV_ap.txt', 'tab:blue', 'Duperray2003')
    plot_model(ax, 'output/Winkler2017_100GeV_ap.txt', 'tab:brown', 'Winkler2017')
    #plot_model(ax, 'output/Feng2016_EPOS_100GeV_ap.txt', 'tab:purple', 'Feng2016 (EPOS)')
    #plot_model(ax, 'output/Feng2016_QGSJET_100GeV_ap.txt', 'tab:orange', 'Feng2016 (QGSJET)')

    ax.set_ylim([1e-3, 1e1])
    ax.legend(fontsize=16)
    plt.savefig('ap_xsecs_100GeV.pdf')

def plot_xsecs_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')
    
    plot_model(ax, 'output/AAFRAG_1TeV_ap.txt', 'tab:orange', 'AAFRAG')
    plot_model(ax, 'output/TanNg1983_1TeV_ap.txt', 'tab:red', 'TanNg1983')
    plot_model(ax, 'output/DiMauro2014_1TeV_ap.txt', 'tab:green', 'DiMauro2014')
    plot_model(ax, 'output/Duperray2003_1TeV_ap.txt', 'tab:blue', 'Duperray2003')
    plot_model(ax, 'output/Winkler2017_1TeV_ap.txt', 'tab:brown', 'Winkler2017')
    #plot_model(ax, 'output/Feng2016_EPOS_1TeV_ap.txt', 'tab:purple', 'Feng2016 (EPOS)')
    #plot_model(ax, 'output/Feng2016_QGSJET_1TeV_ap.txt', 'tab:orange', 'Feng2016 (QGSJET)')

    ax.set_ylim([1e-3, 1e1])
    ax.legend(fontsize=16)
    plt.savefig('ap_xsecs_1TeV.pdf')

if __name__== "__main__":
    plot_xsecs_10GeV()
    plot_xsecs_100GeV()
    plot_xsecs_1TeV()
