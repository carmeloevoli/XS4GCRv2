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
    #ax.set_ylim([1e1, 1e4])
    ax.set_xscale('log')
    ax.set_xlabel(r'E [GeV/n]')
    ax.set_ylabel(r'E$^{2.5}$ $\Phi$ [GeV/n$^{-1}$ m$^{-2}$ s$^{-1}$ sr$^{-1}$]')
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
    ax.set_xlim([1, 1e4])
    ax.set_ylim([1e2, 4e4])
    ax.set_ylabel(r'E$^{2.8}$ $\Phi$ [GeV/n$^{1.8}$ m$^{-2}$ s$^{-1}$ sr$^{-1}$]')

    plot_CRDB_data('CRDB_H_AMS02_2015.txt', 2.8, 'tab:blue')
    plot_CRDB_data('CRDB_He_AMS02_2017.txt', 2.8, 'tab:orange')

    T, H, He = get_model('output/lis.txt')
    ax.plot(T, np.power(T, 2.8) * H, color='tab:purple', label='H')
    ax.plot(T, np.power(T, 2.8) * He, color='tab:red', label='He')

    ax.legend(fontsize=18)
    savefig(plt, 'cr_phe')
    
def plot_antimatter():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '')
    ax.set_xlim([5, 1e3])
    ax.set_ylim([1, 1e1])
    ax.set_xlabel(r'E [GeV]')
    ax.set_ylabel(r'E$^{2.8}$ $\Phi$ [GeV$^{1.8}$ m$^{-2}$ s$^{-1}$ sr$^{-1}$]')

    plot_CRDB_data('CRDB_pbar_AMS02_2016.txt', 2.8, 'tab:blue')
    plot_CRDB_data('CRDB_pos_AMS02_2019.txt', 2.8, 'tab:green')

    T, X, qH, qHe = np.loadtxt('output/qism.txt', usecols=(0,1,2,3), unpack=True, skiprows=1)
    ax.plot(T, np.power(T, 2.8) * (2.2 * X * qH), color='tab:purple', label=r'DiMauro $\bar p$')
    #ax.plot(T, np.power(T, 2.8) * (2.2 * X * pbar_He), color='tab:blue', linestyle='--', label='qHe')

    T, X, qH, qHe = np.loadtxt('output/qism.txt', usecols=(0,1,4,5), unpack=True, skiprows=1)
    ax.plot(T, np.power(T, 2.8) * (2.0 * X * qH), color='tab:red', label=r'Winkler $\bar p$')
    #ax.plot(T, np.power(T, 2.8) * (2.2 * X * pos_He), color='tab:orange', linestyle='--', label='qHe')

    T, X, qH, qHe = np.loadtxt('output/qism.txt', usecols=(0,1,6,7), unpack=True, skiprows=1)
    ax.plot(T, np.power(T, 2.8) * (2.0 * X * qH), color='tab:red', label=r'Kamae $e^+$')
    #ax.plot(T, np.power(T, 2.8) * (2.2 * X * pos_He), color='tab:orange', linestyle='--', label='qHe')

    T, X, qH, qHe = np.loadtxt('output/qism.txt', usecols=(0,1,8,9), unpack=True, skiprows=1)
    ax.plot(T, np.power(T, 2.8) * (2.0 * X * qH), color='tab:red', label=r'Orusa $e^+$')
    #ax.plot(T, np.power(T, 2.8) * (2.2 * X * pos_He), color='tab:orange', linestyle='--', label='qHe')




    ax.legend(fontsize=15)
    savefig(plt, 'cr_antimatter')
    
if __name__== "__main__":
    plot_phe()
    plot_antimatter()
