import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np
from scipy.misc import derivative

def savefig(plt, filename):
    filename = filename + '.pdf'
    print ('- saving plot on ' + filename)
    plt.savefig(filename)

def plot_model(ax, filename, color, label, norm = 1.0):
    T, q20, q24, q28 = np.loadtxt(filename, usecols=(0,1,2,3), unpack=True, skiprows=1)
    ax.plot(T, norm * np.power(T, 2.4) * q24, color=color, label=label)
    return T, np.power(T, 2.4) * q24
    
def plot_model_slope(ax, filename, color, label, norm = 1.0):
    T, q20, q24, q28 = np.loadtxt(filename, usecols=(0,1,2,3), unpack=True, skiprows=1)
    lnT = np.log(T)
    q24 = np.power(T, 2.4) * q24
    lnq = np.log(q24)
    ax.plot(T[1:], np.diff(lnq) / np.diff(lnT), color=color, label=label)
    q28 = np.power(T, 2.8) * q28
    lnq = np.log(q28)
    ax.plot(T[1:], np.diff(lnq) / np.diff(lnT), color=color, linestyle=':')

def plot_model_ratio(ax, filename_0, filename, color, label, norm = 1.0):
    T_0, q20_0, q24_0, q28_0 = np.loadtxt(filename_0, usecols=(0,1,2,3), unpack=True, skiprows=1)
    T, q20, q24, q28 = np.loadtxt(filename, usecols=(0,1,2,3), unpack=True, skiprows=1)
    ax.plot(T, (q24 - q24_0) / q24_0, color=color, label=label)
    ax.plot(T, (q28 - q28_0) / q28_0, color=color, linestyle=':')

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlim([1e1, 1e5])
    ax.set_xscale('log')
    ax.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')
    ax.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')
    ax.set_yscale('log')
    ax.set_ylim([0.01, 50])
    #ax.set_title(title)
    return ax
    
def plot_gammas_sourceterm():
    fig = plt.figure(figsize=(18.5, 8.5))
    ax1 = fig.add_subplot(121)
    
    ax1.set_xscale('log')
    ax1.set_xlim([1e1, 1e5])
    ax1.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax1.set_yscale('log')
    ax1.set_ylim([0.2, 2.])
    ax1.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')

    #ax1.text(20, 4, r'$\alpha = 2.4$')
    
    plot_model(ax1, 'output/Kamae2006_gammas_source.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e20)
    plot_model(ax1, 'output/Kafexhiu2014G4_gammas_source.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 1e20)
    plot_model(ax1, 'output/Kafexhiu2014P8_gammas_source.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 1e20)
    plot_model(ax1, 'output/Kafexhiu2014Sibyll_gammas_source.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 1e20)
    plot_model(ax1, 'output/Kelner2006_gammas_source.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e20)
    plot_model(ax1, 'output/AAFRAG_gammas_source.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e20)

    ax1.legend(fontsize=14)
    
    ax2 = fig.add_subplot(122)

    ax2.set_xscale('log')
    ax2.set_xlim([1e1, 1e5])
    ax2.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax2.set_ylim([-1.5, 1.5])
    ax2.set_ylabel(r'relative ratio')

    ax2.text(20, 1.2, r'$\alpha = 2.4/2.8$')

    plot_model_ratio(ax2, 'output/AAFRAG_gammas_source.txt', 'output/Kamae2006_gammas_source.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1)
    plot_model_ratio(ax2, 'output/AAFRAG_gammas_source.txt', 'output/Kafexhiu2014G4_gammas_source.txt', 'tab:olive', 'Kafexhiu2014/GEANT4', 1)
    plot_model_ratio(ax2, 'output/AAFRAG_gammas_source.txt', 'output/Kafexhiu2014P8_gammas_source.txt', 'tab:pink', 'Kafexhiu2014/PYTHIA8', 1)
    plot_model_ratio(ax2, 'output/AAFRAG_gammas_source.txt', 'output/Kafexhiu2014Sibyll_gammas_source.txt', 'tab:cyan', 'Kafexhiu2014/SIBYLL', 1)
    plot_model_ratio(ax2, 'output/AAFRAG_gammas_source.txt', 'output/Kelner2006_gammas_source.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1)

    savefig(plt, 'xsecs_gammas_source')

def plot_neutrinos_sourceterm():
    fig = plt.figure(figsize=(18.5, 8.5))
    ax1 = fig.add_subplot(121)

    ax1.set_xscale('log')
    ax1.set_xlim([1e1, 1e5])
    ax1.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax1.set_yscale('log')
    ax1.set_ylim([0.2   , 2.])
    ax1.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')

    #ax1.text(20, 4, r'$\alpha = 2.4$')
 
    plot_model(ax1, 'output/Kamae2006_nus_source.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1e20)
    plot_model(ax1, 'output/Kelner2006_nus_source.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1e20)
    plot_model(ax1, 'output/AAFRAG_nus_source.txt', 'tab:green', 'AAFRAG/QGSJET-II', 1e20)

    ax1.legend(fontsize=14)
   
    ax2 = fig.add_subplot(122)

    ax2.set_xscale('log')
    ax2.set_xlim([1e1, 1e5])
    ax2.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax2.set_ylim([-1.5, 1.5])
    ax2.set_ylabel(r'relative ratio')

    ax2.text(20, 1.2, r'$\alpha = 2.4/2.8$')

    plot_model_ratio(ax2, 'output/AAFRAG_nus_source.txt', 'output/Kamae2006_nus_source.txt', 'tab:red', 'Kamae2006/PYTHIA6.2', 1)
    plot_model_ratio(ax2, 'output/AAFRAG_nus_source.txt', 'output/Kelner2006_nus_source.txt', 'tab:blue', 'Kelner2006/SIBYLL', 1)
    
    savefig(plt, 'xsecs_nus_source')

def plot_positrons_sourceterm():
    fig = plt.figure(figsize=(19.0, 8.5))
    ax1 = fig.add_subplot(121)

    ax1.set_xscale('log')
    ax1.set_xlim([1e1, 1e5])
    ax1.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax1.set_yscale('log')
    ax1.set_ylim([0.04, 0.4])
    ax1.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')

    #ax1.text(20, .5, r'$\alpha = 2.4$')

    plot_model(ax1, 'output/Kamae2006_pos_source.txt', 'tab:red', 'Kamae2006', 1e20)
    plot_model(ax1, 'output/AAFRAG_pos_source.txt', 'tab:brown', 'AAFRAG', 1e20)
    plot_model(ax1, 'output/HuangPohl2007_pos_source.txt', 'tab:blue', 'HuangPohl2007', 1e20)
    plot_model(ax1, 'output/Orusa2022_pos_source.txt', 'tab:green', 'Orusa2022', 1e20)

    ax1.legend(fontsize=14)
   
    ax2 = fig.add_subplot(122)

    ax2.set_xscale('log')
    ax2.set_xlim([1e1, 1e4])
    ax2.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax2.set_ylim([-1.5, 1.5])
    ax2.set_ylabel(r'relative ratio')

    ax2.text(20, 1., r'$\alpha = 2.4/2.8$')

    plot_model_ratio(ax2, 'output/Orusa2022_pos_source.txt', 'output/AAFRAG_pos_source.txt', 'tab:brown', 'AAFRAG', 1)
    plot_model_ratio(ax2, 'output/Orusa2022_pos_source.txt', 'output/Kamae2006_pos_source.txt', 'tab:red', 'Kamae2006', 1)
    plot_model_ratio(ax2, 'output/Orusa2022_pos_source.txt', 'output/HuangPohl2007_pos_source.txt', 'tab:blue', 'HuangPohl2007', 1)

    savefig(plt, 'xsecs_pos_source')

def plot_antiprotons_sourceterm():
    fig = plt.figure(figsize=(19.0, 8.5))
    ax1 = fig.add_subplot(121)

    ax1.set_xscale('log')
    ax1.set_xlim([1e1, 1e5])
    ax1.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax1.set_yscale('log')
    ax1.set_ylim([0.01, 0.1])
    ax1.set_ylabel(r'E$_{\textrm{s}}^\alpha$ q$_{\textrm{s}}$ [a.u.]')

    plot_model(ax1, 'output/AAFRAG_pbar_source.txt', 'tab:brown', 'AAFRAG', 1e20)
    plot_model(ax1, 'output/Korsmeier2018_pbar_source.txt', 'tab:green', 'Korsmeier2018', 1e20)
    #plot_model(ax1, 'output/DiMauro2014_pbar_source.txt', 'tab:red', 'DiMauro2014', 1e20)
    #plot_model(ax1, 'output/Duperray2003_pbar_source.txt', 'tab:green', 'Duperray2004', 1e20)
    plot_model(ax1, 'output/Feng2016_EPOS_pbar_source.txt', 'tab:orange', 'Feng2016/EPOS', 1e20)
    plot_model(ax1, 'output/Feng2016_QGSJET_pbar_source.txt', 'tab:blue', 'Feng2016/QGSJET', 1e20)
    #plot_model(ax1, 'output/TanNg1983_pbar_source.txt', 'tab:purple', 'TanNg1983', 1e20)
    plot_model(ax1, 'output/Winkler2017_pbar_source.txt', 'tab:cyan', 'Winkler2017', 1e20)

    ax1.legend(fontsize=14)
   
    ax2 = fig.add_subplot(122)

    ax2.set_xscale('log')
    ax2.set_xlim([1e1, 1e4])
    ax2.set_xlabel(r'E$_{\textrm{s}}$ [GeV]')

    ax2.set_ylim([-1.5, 1.5])
    ax2.set_ylabel(r'relative ratio')

    plot_model_ratio(ax2, 'output/Korsmeier2018_pbar_source.txt', 'output/AAFRAG_pbar_source.txt', 'tab:brown', 'AAFRAG', 1)
    plot_model_ratio(ax2, 'output/Korsmeier2018_pbar_source.txt', 'output/Feng2016_EPOS_pbar_source.txt', 'tab:orange', 'Feng2016/EPOS', 1)
    plot_model_ratio(ax2, 'output/Korsmeier2018_pbar_source.txt', 'output/Feng2016_QGSJET_pbar_source.txt', 'tab:blue', 'Feng2016/QGSJET', 1)
    plot_model_ratio(ax2, 'output/Korsmeier2018_pbar_source.txt', 'output/Winkler2017_pbar_source.txt', 'tab:cyan', 'Winkler2017', 1)

    savefig(plt, 'xsecs_pbar_source')
    
def plot_all_sourceterm():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, 'Inelasticity')
    ax.set_ylim([1e-2, 1e0])
    ax.set_xlim([1e1, 1e4])

#    plot_model(ax, 'output/AAFRAG_terp_source.txt', 'tab:gray', r'$p$', 1e20)
    T, qpbar = plot_model(ax, 'output/Korsmeier2018_pbar_source.txt', 'tab:purple', r'$\bar p$', 1e20)
    T, qpos = plot_model(ax, 'output/Orusa2022_pos_source.txt', 'tab:green', r'$e^+$', 1e20)
    T, qgamma = plot_model(ax, 'output/AAFRAG_gammas_source.txt', 'tab:red', r'$\gamma$', 1e20)
    T, qnu = plot_model(ax, 'output/AAFRAG_nus_source.txt', 'tab:blue', r'$\nu$', 1e20)

    y = 0.09 * (T / 1e4)**0.15
    ax.plot(T, y, linestyle=':', color='tab:gray')

    ax.legend(fontsize=20)
    savefig(plt, 'xsecs_all_source')
    
if __name__== "__main__":
    plot_gammas_sourceterm()
    plot_neutrinos_sourceterm()
    plot_positrons_sourceterm()
    plot_antiprotons_sourceterm()
    plot_all_sourceterm()
