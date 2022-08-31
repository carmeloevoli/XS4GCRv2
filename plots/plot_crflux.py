import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def get_model(filename):
    T, H, He = np.loadtxt(filename, usecols=(0,1,2), unpack=True, skiprows=1)
    return T, H, He
    
def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([.1, 1e6])
    ax.set_ylim([1e1, 1e4])
    ax.set_xscale('log')
    ax.set_xlabel(r'T [GeV/n]')
    ax.set_ylabel(r'T$^{2.5}$ $\Phi$ [GeV/n$^{-1}$ m$^{-2}$ s$^{-1}$ sr$^{-1}$]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_CRDB_data(filename, slope, color):
    R, y, err_y_lo, err_y_hi = np.loadtxt(filename, skiprows=2, usecols=(1,4,9,10), unpack=True)
    yerr = [R**slope * err_y_lo, R**slope * err_y_hi]
    plt.errorbar(R, R**slope * y, yerr=yerr, fmt='o', markeredgecolor=color, color=color)

def plot_phe():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '')
#   ax.plot(T, inel * 100., color=color, label=label)

    plot_CRDB_data('CRDB_H_AMS02_2015.txt', 2.5, 'tab:blue')
    plot_CRDB_data('CRDB_He_AMS02_2017.txt', 2.5, 'tab:orange')

    T, H, He = get_model('output/lis.txt')
    ax.plot(T, np.power(T, 2.5) * H, color='tab:red')
    ax.plot(T, np.power(T, 2.5) * He, color='tab:green')

    ax.legend(fontsize=15)
    savefig(plt, 'cr_phe')

if __name__== "__main__":
    plot_phe()
