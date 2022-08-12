import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np

def plot_model(ax, filename, color, label, norm = 1):
    T, sigma_pp_pos = np.loadtxt(filename, usecols=(0,1), unpack=True, skiprows=1)
    T, sigma_pp_ele = np.loadtxt(filename, usecols=(0,5), unpack=True, skiprows=1)
    ax.plot(T / np.max(T), norm * T * T * sigma_pp_pos, color=color, label=label)
    ax.plot(T / np.max(T), norm * T * T * sigma_pp_ele, color=color, linestyle=":")

def plot_model_helium(ax, filename, color, label, norm = 1):
    T, sigma_phe, sigma_hep, sigma_hehe = np.loadtxt(filename, usecols=(0,2,3,4), unpack=True, skiprows=1)
    ax.plot(T / np.max(T), norm * T * T * sigma_phe, color=color, label=label)
    ax.plot(T / np.max(T), norm * T * T * sigma_hep, color=color, linestyle=':')
    ax.plot(T / np.max(T), norm * T * T * sigma_hehe, color=color, linestyle='--')

def set_axes(fig, title):
    ax = fig.add_subplot(111)
    ax.set_xlabel('E$_{e^+}$ / E$_{p}$')
    ax.set_xlim([1e-4, 1])
    ax.set_xscale('log')
    ax.set_ylabel(r'E$_{e^+}^2$ d$\sigma$/dE$_{e^+}$ [GeV mbarn]')
    ax.set_yscale('log')
    ax.set_title(title)
    return ax

def plot_xsecs_5GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '5 GeV')

    plot_model(ax, 'output/Kamae2006_5GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_5GeV_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 'output/AAFRAG_5GeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model(ax, 'output/Orusa2022_5GeV_pos.txt', 'tab:brown', 'Orusa2022')

    ax.set_ylim([5e-3, 5e0])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_5GeV.pdf')

def plot_xsecs_10GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '10 GeV')

    plot_model(ax, 'output/Kamae2006_10GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_10GeV_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 'output/AAFRAG_10GeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model(ax, 'output/Orusa2022_10GeV_pos.txt', 'tab:brown', 'Orusa2022')

    ax.set_ylim([1e-2, 1e1])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_10GeV.pdf')

def plot_xsecs_100GeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    plot_model(ax, 'output/Kamae2006_100GeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_100GeV_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 'output/AAFRAG_100GeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model(ax, 'output/Orusa2022_100GeV_pos.txt', 'tab:brown', 'Orusa2022')

    ax.set_ylim([1e-1, 1e2])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_100GeV.pdf')

def plot_xsecs_100GeV_helium():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '100 GeV')

    #plot_model_helium(ax, 'output/Kamae2006_1TeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model_helium(ax, 'output/AAFRAG_100GeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model_helium(ax, 'output/Orusa2022_100GeV_pos.txt', 'tab:red', 'Orusa2022')
    plot_model_helium(ax, 'output/HuangPohl2007_100GeV_pos.txt', 'tab:blue', 'HuangPohl2007')
    
    ax.plot([1e100],[1e100], linestyle='-', label='p-He', color='tab:gray')
    ax.plot([1e100],[1e100], linestyle=':', label='He-p', color='tab:gray')
    ax.plot([1e100],[1e100], linestyle='--', label='He-He', color='tab:gray')

    ax.set_ylim([1e-1, 1e3])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_helium_100GeV.pdf')

def plot_xsecs_1TeV():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')

    plot_model(ax, 'output/Kamae2006_1TeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model(ax, 'output/HuangPohl2007_1TeV_pos.txt', 'tab:blue', 'HuangPohl2007')
    plot_model(ax, 'output/AAFRAG_1TeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model(ax, 'output/Orusa2022_1TeV_pos.txt', 'tab:brown', 'Orusa2022')

    ax.set_ylim([1e0, 1e3])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_1TeV.pdf')

def plot_xsecs_1TeV_helium():
    fig = plt.figure(figsize=(10.5, 8))
    ax = set_axes(fig, '1 TeV')

    #plot_model_helium(ax, 'output/Kamae2006_1TeV_pos.txt', 'tab:red', 'Kamae2006')
    plot_model_helium(ax, 'output/AAFRAG_1TeV_pos.txt', 'tab:green', 'AAFRAG v101')
    plot_model_helium(ax, 'output/Orusa2022_1TeV_pos.txt', 'tab:red', 'Orusa2022')
    plot_model_helium(ax, 'output/HuangPohl2007_1TeV_pos.txt', 'tab:blue', 'HuangPohl2007')

    ax.plot([1e100],[1e100], linestyle='-', label='p-He', color='tab:gray')
    ax.plot([1e100],[1e100], linestyle=':', label='He-p', color='tab:gray')
    ax.plot([1e100],[1e100], linestyle='--', label='He-He', color='tab:gray')

    ax.set_ylim([1e0, 1e4])
    ax.legend(fontsize=15)
    plt.savefig('pos_xsecs_helium_1TeV.pdf')

if __name__== "__main__":
    plot_xsecs_5GeV()
    plot_xsecs_10GeV()
    plot_xsecs_100GeV()
#    plot_xsecs_100GeV_helium()
    plot_xsecs_1TeV()
#    plot_xsecs_1TeV_helium()
