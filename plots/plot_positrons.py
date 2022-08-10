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
    ax.set_xlabel('$T_{e^+}$ / $T_{p}$')
    ax.set_xlim([1e-3, 1])
    ax.set_xscale('log')
    ax.set_ylabel(r'$\sigma_{pp \rightarrow e^+}$ [mbarn]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_xsecs_1GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 GeV')

    plot_model(ax, 'output/Kamae2006_1GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_1GeV_pos.txt', 'tab:blue', 'HuangPohl2007')

    ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=16)
    plt.savefig('pos_xsecs_1GeV.pdf')

def plot_xsecs_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/Kamae2006_10GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_10GeV_pos.txt', 'tab:blue', 'HuangPohl2007')

    ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=16)
    plt.savefig('pos_xsecs_10GeV.pdf')

def plot_xsecs_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/Kamae2006_100GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_100GeV_pos.txt', 'tab:blue', 'HuangPohl2007')

    ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=16)
    plt.savefig('pos_xsecs_100GeV.pdf')

def plot_xsecs_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')

    plot_model(ax, 'output/Kamae2006_1TeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_1TeV_pos.txt', 'tab:blue', 'HuangPohl2007')

    ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=16)
    plt.savefig('pos_xsecs_1TeV.pdf')

if __name__== "__main__":
    plot_xsecs_1GeV()
    plot_xsecs_10GeV()
    plot_xsecs_100GeV()
    plot_xsecs_1TeV()
