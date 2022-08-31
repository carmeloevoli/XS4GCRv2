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
    ax.set_xlim([1e-4, 1])
    ax.set_xscale('log')
    ax.set_xlabel(r'x')
    ax.set_ylabel(r'x$^2$ d$\sigma$/dx [mbarn]')
    ax.set_yscale('log')
    ax.set_ylim([1e-3, 2e0])
    ax.set_title(title)
    return ax

def plot_positrons_5GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 5 GeV')

    plot_model(ax, 1, 'output/Kamae2006_xsecs_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 1, 'output/AAFRAG_xsecs_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 1, 'output/HuangPohl2007_xsecs_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 1, 'output/Orusa2022_xsecs_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pos_5GeV')

def plot_positrons_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 GeV')

    plot_model(ax, 5, 'output/Kamae2006_xsecs_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 5, 'output/AAFRAG_xsecs_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 5, 'output/HuangPohl2007_xsecs_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 5, 'output/Orusa2022_xsecs_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pos_10GeV')
    
def plot_positrons_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 100 GeV')

    plot_model(ax, 9, 'output/Kamae2006_xsecs_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 9, 'output/AAFRAG_xsecs_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 9, 'output/HuangPohl2007_xsecs_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 9, 'output/Orusa2022_xsecs_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pos_100GeV')

def plot_positrons_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 1 TeV')

    plot_model(ax, 13, 'output/Kamae2006_xsecs_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 13, 'output/AAFRAG_xsecs_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 13, 'output/HuangPohl2007_xsecs_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 13, 'output/Orusa2022_xsecs_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pos_1TeV')

def plot_positrons_10TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, r'E$_{\mathrm{p}} =$ 10 TeV')

    plot_model(ax, 17, 'output/Kamae2006_xsecs_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 17, 'output/AAFRAG_xsecs_pos.txt', 'tab:brown', 'AAFRAG')
    plot_model(ax, 17, 'output/HuangPohl2007_xsecs_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 17, 'output/Orusa2022_xsecs_pos.txt', 'tab:green', 'Orusa2022')

    ax.legend(fontsize=14)
    savefig(plt, 'xsecs_pos_10TeV')

if __name__== "__main__":
#    plot_gammas_5GeV()
    plot_positrons_10GeV()
    plot_positrons_100GeV()
    plot_positrons_1TeV()
    plot_positrons_10TeV()
