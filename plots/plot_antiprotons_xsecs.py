import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def plot_model(ax, icol, filename, color, label, linestyle = '-'):
    x, sigma_pp = np.loadtxt(filename, usecols=(0,icol), unpack=True, skiprows=1)
    if linestyle == '-':
        ax.plot(x, x * x * sigma_pp, color=color, label=label)
    else:
        ax.plot(x, x * x * sigma_pp, color=color, linestyle=linestyle)
    
def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e-4, 1])
    ax.set_xscale('log')
    ax.set_xlabel(r'x')
    ax.set_ylabel(r'x$^2$ d$\sigma$/dx [mbarn]')
    ax.set_yscale('log')
    ax.set_ylim([1e-3, 1])
    ax.set_title(title)
    return ax

def plot_antiprotons_20GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 20 GeV')

    plot_model(ax, 5, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 5, 'output/DiMauro2014_pbar_xsecs.txt', 'tab:red', 'DiMauro2014')
    plot_model(ax, 5, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
#    plot_model(ax, 5, 'output/Duperray2003_pbar_xsecs.txt', 'tab:green', 'Duperray2004')
    plot_model(ax, 5, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 5, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
#    plot_model(ax, 5, 'output/TanNg1983_pbar_xsecs.txt', 'tab:purple', 'TanNg1983')
    plot_model(ax, 5, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')
    
    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_20GeV')
    
def plot_antiprotons_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 100 GeV')

    plot_model(ax, 9, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 9, 'output/DiMauro2014_pbar_xsecs.txt', 'tab:red', 'DiMauro2014')
    plot_model(ax, 9, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
#    plot_model(ax, 9, 'output/Duperray2003_pbar_xsecs.txt', 'tab:green', 'Duperray2004')
    plot_model(ax, 9, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 9, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
#    plot_model(ax, 9, 'output/TanNg1983_pbar_xsecs.txt', 'tab:purple', 'TanNg1983')
    plot_model(ax, 9, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')
    
    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_100GeV')

def plot_antiprotons_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 1 TeV')

    plot_model(ax, 13, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 13, 'output/DiMauro2014_pbar_xsecs.txt', 'tab:red', 'DiMauro2014')
    plot_model(ax, 13, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
#    plot_model(ax, 13, 'output/Duperray2003_pbar_xsecs.txt', 'tab:green', 'Duperray2004')
    plot_model(ax, 13, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 13, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
#    plot_model(ax, 13, 'output/TanNg1983_pbar_xsecs.txt', 'tab:purple', 'TanNg1983')
    plot_model(ax, 13, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')
 
    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_1TeV')

def plot_antiprotons_10TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 TeV')

    plot_model(ax, 17, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 17, 'output/DiMauro2014_pbar_xsecs.txt', 'tab:red', 'DiMauro2014')
    plot_model(ax, 17, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
#    plot_model(ax, 17, 'output/Duperray2003_pbar_xsecs.txt', 'tab:green', 'Duperray2004')
    plot_model(ax, 17, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 17, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
#    plot_model(ax, 17, 'output/TanNg1983_pbar_xsecs.txt', 'tab:purple', 'TanNg1983')
    plot_model(ax, 17, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_10TeV')

def plot_antiprotons_He_20GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 20 GeV')

    plot_model(ax, 7, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 7, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
    plot_model(ax, 7, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 7, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
    plot_model(ax, 7, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')

    plot_model(ax, 6, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG', ':')
    plot_model(ax, 6, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018', ':')
    plot_model(ax, 6, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS', ':')
    plot_model(ax, 6, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET', ':')
    plot_model(ax, 6, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017', ':')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_He_20GeV')

def plot_antiprotons_He_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 100 GeV')

    plot_model(ax, 10, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 10, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
    plot_model(ax, 10, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 10, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
    plot_model(ax, 10, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')

    plot_model(ax, 9, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG', ':')
    plot_model(ax, 9, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018', ':')
    plot_model(ax, 9, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS', ':')
    plot_model(ax, 9, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET', ':')
    plot_model(ax, 9, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017', ':')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_He_100GeV')
    
def plot_antiprotons_He_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 1 TeV')

    plot_model(ax, 15, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 15, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
    plot_model(ax, 15, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 15, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
    plot_model(ax, 15, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')

    plot_model(ax, 14, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG', ':')
    plot_model(ax, 14, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018', ':')
    plot_model(ax, 14, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS', ':')
    plot_model(ax, 14, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET', ':')
    plot_model(ax, 14, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017', ':')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_He_1TeV')

def plot_antiprotons_He_10TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 TeV')

    plot_model(ax, 19, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 19, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018')
    plot_model(ax, 19, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS')
    plot_model(ax, 19, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET')
    plot_model(ax, 19, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017')

    plot_model(ax, 18, 'output/AAFRAG_pbar_xsecs.txt', 'tab:brown', 'AAFRAG', ':')
    plot_model(ax, 18, 'output/Korsmeier2018_pbar_xsecs.txt', 'tab:green', 'Korsmeier2018', ':')
    plot_model(ax, 18, 'output/Feng2016_EPOS_pbar_xsecs.txt', 'tab:orange', 'Feng2016/EPOS', ':')
    plot_model(ax, 18, 'output/Feng2016_QGSJET_pbar_xsecs.txt', 'tab:blue', 'Feng2016/QGSJET', ':')
    plot_model(ax, 18, 'output/Winkler2017_pbar_xsecs.txt', 'tab:cyan', 'Winkler2017', ':')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pbar_He_10TeV')

if __name__== "__main__":
    plot_antiprotons_20GeV()
    plot_antiprotons_100GeV()
    plot_antiprotons_1TeV()
    plot_antiprotons_10TeV()
    plot_antiprotons_He_20GeV()
    plot_antiprotons_He_100GeV()
    plot_antiprotons_He_1TeV()
    plot_antiprotons_He_10TeV()
