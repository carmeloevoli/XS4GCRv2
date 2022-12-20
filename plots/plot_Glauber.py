import matplotlib
matplotlib.use('MacOSX')
import matplotlib.pyplot as plt
plt.style.use('./xs4gcr.mplstyle')
import numpy as np
import scipy.optimize as opt
import math

r = [0.0, 0.85, 2.095, 1.976, 1.671, 1.986, 2.57,  2.41,  2.23,
     2.519, 2.45, 2.42,  2.471, 2.440, 2.58,  2.611, 2.730, 2.662,
     2.727, 2.9,  3.040, 2.867, 2.969, 2.94,  3.075, 3.11,  3.06]

def get_wilson_rms_radius(A):
    factor = 1. # np.sqrt(5. / 3.)
    radius = 0.
    if (A > 26):
        radius = factor * (0.84 * np.power(A, 1. / 3.) + 0.55)
    else:
        radius = factor * r[A]
    return radius

def get_wilson_radius(A):
    r_0 = 0.85
    r = get_wilson_rms_radius(A)
    return 1.29 * np.sqrt(r * r - r_0 * r_0)

def plot_radii():
    fig = plt.figure(figsize=(10.5, 8))
    ax = fig.add_subplot(111)
    ax.set_xlim([0, 80])
    ax.set_xlabel('A')
    ax.set_ylabel('R [fm]')

    filename = '../data/nucleiRadii_Adani2013.txt'
    A, N, r, dr = np.loadtxt(filename, skiprows=2, usecols=(0,1,2,3), unpack=True)

    color = 'tab:red'
    ax.errorbar(A, r, yerr=dr, fmt='o', markeredgecolor=color, color=color, label='Adani+2013')

    ax.plot(A, 1.1 * np.power(A, 1. / 3.), label=r'$\propto$ A$^{1/3}$', color='tab:green')

    A = np.arange(1, 80)
    Y = []
    for A_i in A:
        Y.append(get_wilson_rms_radius(A_i))
    ax.plot(A, Y, label='Wilson radius', color='tab:blue')

#    ax.plot(A, 0.84 * np.power(A, 1. / 3.) + 0.55)

    ax.legend()
    plt.savefig('nuclear_radii.pdf')

#def sigma_ab(Z, B, Y1, Y2, s0, s1, eta1, eta2, s):
#    return Z + B * np.log(s / s0)**2. + Y1 * np.power(s1 / s, eta1) - Y2 * np.power(s1 / s, eta2)

def sigma_total_pp(plab):
    m_p = 0.938
    E = np.sqrt(plab * plab + m_p * m_p)
    s = 2. * m_p**2. + 2. * m_p * E
    return 21.7 * np.power(s, 0.0808) + 56.08 * np.power(s, -0.45)

def plot_data_pp(ax):
    filename = '../data/rpp2022-pp_total.dat'
    p, sigma, staErrUp, staErrLo, syErrUp, syErrLo = np.loadtxt(filename, usecols=(1,4,5,6,7,8), unpack=True, skiprows=11)
    totErrUp = np.sqrt(staErrUp * staErrUp + syErrUp * syErrUp)
    totErrLo = np.sqrt(staErrLo * staErrLo + syErrLo * syErrLo)
    color = 'tab:red'
    ax.errorbar(p, sigma, yerr=[totErrLo, totErrUp], fmt='o', markeredgecolor=color, color=color, zorder=2)

def plot_data_pn(ax):
    filename = '../data/rpp2022-np_total.dat'
    p, sigma, staErrUp, staErrLo, syErrUp, syErrLo = np.loadtxt(filename, usecols=(1,4,5,6,7,8), unpack=True, skiprows=11)
    totErrUp = np.sqrt(staErrUp * staErrUp + syErrUp * syErrUp)
    totErrLo = np.sqrt(staErrLo * staErrLo + syErrLo * syErrLo)
    color = 'tab:olive'
    ax.errorbar(p, sigma, yerr=[totErrLo, totErrUp], fmt='o', markeredgecolor=color, color=color, zorder=2)

def func_total(lnx, a, b, c, d):
    x = np.exp(lnx)
    return a * np.power(x, b) + c * np.power(x, -d)

def fit_total(ax):
    filename = '../data/rpp2022-pp_total.dat'
    xdata, ydata, staErrUp, staErrLo, syErrUp, syErrLo = np.loadtxt(filename, usecols=(1,4,5,6,7,8), unpack=True, skiprows=11)
    sigma = np.sqrt(staErrUp * staErrUp + syErrUp * syErrUp)
    
    x = []
    y = []
    s = []
    
    for i,j,k in zip(xdata, ydata, sigma):
        if i > 2. and i < 1e10:
            x.append(np.log(i))
            y.append(j)
            s.append(k)
    
    popt, pcov = opt.curve_fit(func_total, x, y, sigma=s, p0=[3.4e+01, 2.41e-02, 2.22e+01, 8.01e-01])
    print ("%10.5f %10.5f %10.5f %10.5f" % (popt[0], popt[1], popt[2], popt[3]))
    xdata = np.logspace(0, 15, 1000)
    ax.plot(xdata, func_total(np.log(xdata), *popt), zorder=3)

def plot_total_pp():
    fig = plt.figure(figsize=(11.5, 8))
    ax = fig.add_subplot(111)
    ax.set_xscale('log')
    ax.set_ylim([0,200])
    ax.set_ylabel(r'$\sigma$ [mb]')
    ax.set_xlim([0.1,1e11])
    ax.set_xlabel(r'$p_{lab}$ [GeV]')

    fit_total(ax)

    plot_data_pp(ax)
    plot_data_pn(ax)

    ax.text(1e1, 150, r'20.52069 x$^{0.09001}$ + 30.59911 x$^{-0.31521}$', fontsize=20)

    plt.savefig('sigma_total_pp.pdf')

def plot_pC():
    fig = plt.figure(figsize=(11.5, 8))
    ax = fig.add_subplot(111)
    ax.set_xscale('log')
    ax.set_ylim([220,280])
    ax.set_ylabel(r'$\sigma$ [mb]')
    ax.set_xlim([1,1e4])
    ax.set_xlabel(r'E [GeV]')

    E = np.logspace(0,4,1000)
    
    A = 12 # C12
    fmSquare2mbarn = 10.
    mp = 0.938 # GeV
    
    piR2 = math.pi * np.power(get_wilson_rms_radius(A), 2.0) * fmSquare2mbarn
    p = np.sqrt(E * E + 2. * mp * E)
    AsigmaTot = float(A) * func_total(np.log(E), 20.52069,0.09001,30.59911,0.31521)
    sigma = piR2 * np.log(1. + AsigmaTot / piR2)
       
    ax.text(300., 270., 'p+C12')
    ax.plot(E, sigma)

    print(np.log(247./233.)/np.log(1e3/30.))
    
    ax.plot([1,1e4],[233,233],':',color='tab:gray')
    ax.plot([1,1e4],[247,247],':',color='tab:gray')

    plt.savefig('sigma_pC.pdf')

if __name__== "__main__":
    plot_radii()
    plot_total_pp()
    plot_pC()
