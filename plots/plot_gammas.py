import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def plot_model(ax, filename, color, label, norm = 1):
    T, sigma_pp = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    ax.plot(T / np.max(T), norm * T * T * sigma_pp, color=color, label=label)

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlabel('$T_{\gamma}$ / $T_{p}$')
    ax.set_xlim([1e-4, 1])
    ax.set_xscale('log')
    ax.set_ylabel(r'$T_{\gamma}^2 \, d\sigma/dT_{\gamma}$ [mbarn]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_xsecs_5GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '5 GeV')

    plot_model(ax, 'output/Kamae2006_5GeV_gamma.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/Kelner2006_5GeV_gamma.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 'output/AAFRAG_5GeV_gamma.txt', 'tab:green', 'AAFRAG')

    x, y = np.loadtxt('test_gamma_aafrag_5GeV.txt', usecols=(1,2), unpack=True, skiprows=0)
    T_p = 5e9
    ax.plot(x / T_p, y * x / 1e9, linestyle=':', color='tab:gray')

    ax.set_ylim([1e-3, 4e1])
    ax.legend(fontsize=16)
    plt.savefig('gamma_xsecs_5GeV.pdf')
    
def plot_xsecs_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    ax.set_xlim([1e-3, 1])
    ax.set_ylim([1e-1, 3e1])

    plot_model(ax, 'output/Kamae2006_10GeV_gamma.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/Kelner2006_10GeV_gamma.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 'output/AAFRAG_10GeV_gamma.txt', 'tab:green', 'AAFRAG')

    #ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=16)
    plt.savefig('gamma_xsecs_10GeV.pdf')

def plot_xsecs_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/Kamae2006_100GeV_gamma.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/Kelner2006_100GeV_gamma.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 'output/AAFRAG_100GeV_gamma.txt', 'tab:green', 'AAFRAG')

    ax.set_ylim([1e0, 4e2])
    ax.legend(fontsize=16)
    plt.savefig('gamma_xsecs_100GeV.pdf')

def plot_xsecs_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')

    plot_model(ax, 'output/Kamae2006_1TeV_gamma.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/Kelner2006_1TeV_gamma.txt', 'tab:blue', 'Kelner2006')
    plot_model(ax, 'output/AAFRAG_1TeV_gamma.txt', 'tab:green', 'AAFRAG')

    x, y = np.loadtxt('test_gamma_aafrag_1TeV.txt', usecols=(1,2), unpack=True, skiprows=0)
    T_p = 1e12
    ax.plot(x / T_p, y * x / 1e9, linestyle=':', color='tab:gray')

    ax.set_ylim([1e1, 4e3])
    ax.legend(fontsize=16)
    plt.savefig('gamma_xsecs_1TeV.pdf')

if __name__== "__main__":
    plot_xsecs_5GeV()
    plot_xsecs_10GeV()
    plot_xsecs_100GeV()
    plot_xsecs_1TeV()
