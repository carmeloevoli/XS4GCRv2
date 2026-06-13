###############################################################################
## AAfrag 2: Interpolation routines for Monte Carlo results on secondary     ##
## production including light antinuclei in hadronic interactions            ##
##                                                                           ##
## Copyright: M. Kachelriess, S. Ostapchenko, J. Tjemsland (2023)            ##
## Lisence:   CC BY-NC 4.0                                                   ##
##                                                                           ##
## see https://sourceforge.net/projects/aafrag/ for more info                ##
###############################################################################

from numpy import log10, loadtxt, where
from scipy.interpolate import interp2d

# Global parameters; should not be changed
E0=10
tables="Tables/"
small=-100

def banner():
    """ Print banner to console """
    print()
    print('##################################################################')
    print('#  AAfrag v2.02                                                  #')
    print('#  Authors: M. Kachelriess, S. Ostapchenko, J. Tjemsland         #')
    print('#  Refs.                                                         #')
    print('#     Submitted to Comput.Phys.Commun.      [arXiv:2206.00998]   #')
    print('#     Comput.Phys.Commun. 245 (2019) 106846 [arXiv:1904.05129]   #')
    print('#  see https://sourceforge.net/projects/aafrag/ for more info.   #')
    print('#                                                                #')
    print('# Disclaimer: This program comes without any guarantees! Beware  #')
    print('# of errors and use common sense interpreting results!           #')
    print('##################################################################')

def interp(filename,tables=tables):
    """ Reads interpolation tables and initialises an array of callable
    binomial interpolation class.
    
    Inputs:
    	filename: (str)
    	          File containing the table
    	tables:   (str) default=tables
    	          Name of folder containing the tables
    Returns:
    	f: (array of RectBivariateSpline)
    	   The entries are callable as f[:](x, y), where x is the
    	   primary energy in GeV and y is the secondary energy in GeV.
    """
    print(filename)
    
    data = loadtxt(tables+filename)
    # Find the position in the table for the different baryon number
    # (p/pbar, n/nbar, e+/e-, nu/nubar) and flavours (neutrinos)
    index = where(data[:, 0]<1e-50)[0]
    
    E_p = data[index[-1]+1:, 0] # Primary energy
    E_s = data[0, 2:] # Secondary energy
    
    f = []
    for i in index: # Loop over particles in the table
        sigma = data[i+1:i+len(E_p)+1,1:-1]
        sigma[where(sigma<1e-50)]=10**small # Small
        f.append(interp2d(log10(E_s),log10(E_p),
            log10(sigma),fill_value=small))
    return f

def init(tables=tables):
    """ Initialises AAfrag 2.
    Inputs:
        tables: (str) default=tables
        Name of folder containing the tables
    """
    banner() # Print banner
    # Load tables for (anti-) proton, (anti-) neutron and (anti-) neutrinos
    # in p-p, p-He, He-p, He-He, C-p, Al-p and Fe-p interactions
    for filename in ["p_p_04", "p_He04", "He_p04", "HeHe04",
            "C_p_04", "Al_p04", "Fe_p04"]:    
        f_gam.append(interp("gam_"+filename,tables))
        f_el.append(interp("el_"+filename,tables))
        f_nu.append(interp("nu_"+filename,tables))
        f_pap.append(interp("pap_"+filename,tables))
        f_nan.append(interp("nan_"+filename,tables))
    # Low energy, small binning
    for filename in ["p_p_04L", "p_He04L", "He_p04L", "HeHe04L"]: 
        f_gamL.append(interp("gam_"+filename,tables))
        f_elL.append(interp("el_"+filename,tables))
        f_nuL.append(interp("nu_"+filename,tables))
        f_papL.append(interp("pap_"+filename,tables))
        f_nanL.append(interp("nan_"+filename,tables))
    # Import tables for antideuteron and entihelium-3 in p-p, p-He, He-p, He-He,
    # pbar-p and pbar-He interactions
    for filename in ["p_p_04", "p_He04", "He_p04", "HeHe04", "", "", "",
            "ap_p04", "apHe04"]:
        if filename == "":
            f_ad.append([])
            f_ah.append([])
            continue
        f_ad.append(interp("ad_"+filename,tables))
        f_ah.append(interp("ah_"+filename,tables))

# These are initialised with init()
f_gam=[]; f_el=[]; f_nu=[]; f_pap=[]; f_nan=[]; f_ad=[]; f_ah=[];
f_gamL=[]; f_elL=[]; f_nuL=[]; f_papL=[]; f_nanL=[]

def spec(f,E_p,E_s,q,k):
    """ Evaluates the production cross sections E^2 dsigma/dE in mb GeV.
    Inputs:
        f:   (array of callables)
             Interpolation arrays initialised by init()
        E_p: (float or numpy array of floats)
             Primary energy in GeV
        E_s: (float of numpy array of floats)
             Secondary energy in GeV
        q:   (int) 
             Index of reaction channel 1:p-p, 2:He-p, 3:p-He, 4:He-He, 5:C-p,
             6:Al-p, 7:Fe-p, 8:pbar-p, 9:pbar-He
        k:   (int)
             Index of particle "sub-species"
             Photons   (f=f_gam)   - 1: gamma
             Protons   (f=f_pap)   - 1: proton, 2: antiproton
             Electrons (f=f_elpos) - 1: electrons, 2: positrons
             Neutrinos (f=f_nu)    - 1: electron neutrino,
                       2: anti-electron neutrino, 3: muon neutrino,
                       4: anti-muon neutrino
             Neutrons  (f=f_nan)   - 1: neutron, 2: antineutron
             Deuteron  (f=f_ad)    - 1: antideuteron
             Helium-3  (f=f_ah)    - 1: antihelium-3
    Returns:
        Production cross section E^2 dsigma/dE in mb GeV for primary energy
        E_p and secondary energy E_s. The interaction type is determined by
        q, and the secondary particle is determined by f and q.
    """
    # Linear interpolation of logarithm of data
    x = log10(E_p); y = log10(E_s)
    tmp = f[k-1][q-1](y, x) 
    spectrum = E_s*10**tmp
    spectrum[where(tmp<small/3)] = 0 # Set to zero where assigned small
    return spectrum 

# ------- Main functions intended for the user -------

# These lambda functions are essentially convenient wrappers for spec
spec_gam = lambda *a: spec(f_gamL, *a) if a[0]<E0 else spec(f_gam, *a)
spec_pap = lambda *a: spec(f_papL, *a) if a[0]<E0 else spec(f_pap, *a)
spec_elpos = lambda *a: spec(f_elL,*a) if a[0]<E0 else spec(f_el,  *a)
spec_nu  = lambda *a: spec(f_nuL,  *a) if a[0]<E0 else spec(f_nu,  *a)
spec_nan = lambda *a: spec(f_nanL, *a) if a[0]<E0 else spec(f_nan, *a)
spec_ad  = lambda *a: spec(f_ad,   *a)
spec_ah  = lambda *a: spec(f_ah,   *a)

spec_gam.__doc__ = "Interpolation routine for photons; wrapper of spec"
spec_pap.__doc__ = "Interpolation routine for (anti-) protons; wrapper of spec"
spec_elpos.__doc__ = "Interpolation routine for electrons and positrons; " \
	           + "wrapper of spec"
spec_nu.__doc__ = "Interpolation routine for neutrinos; wrapper of spec"
spec_ad.__doc__ = "Interpolation routine for antideuterons; wrapper of spec"
spec_ah.__doc__ = "Interpolation routine for antihelium-3; wrapper of spec"

# ------- This is an example run of AAfrag 2 -------
def example_plot(k=1, E_p=100):
    """ Produces an example plot the example run of AAfrag 2 and
    saves it as plot.pdf.
    
    Inputs:
        k: (int) default=1
           Index of reaction channel used for the title
           1:p-p, 2:He-p, 3:p-He, 4:He-He, 5:C-p,
           6:Al-p, 7:Fe-p, 8:pbar-p, 9:pbar-He
        E_p: (float) default=100
           Primary energy energy used for the title
    """
    from numpy import loadtxt
    import matplotlib.pyplot as plt

    channel=(["","p-p","He-p","p-He","He-He","C-p","Al-p","Fe-p",
             r"$\bar{\rm p}$-p",r"$\bar{\rm p}$-He"])[k]

    s_aprot = loadtxt("spec_aprot").T
    s_aneut = loadtxt("spec_aneut").T
    s_adeut = loadtxt("spec_adeut").T
    s_ahel  = loadtxt("spec_ahel").T
    s_nu    = loadtxt("spec_nu_tot").T
    s_elpos = loadtxt("spec_elpos").T

    plt.figure(figsize=(5, 4))
    plt.title(r"%s @ $E_\mathrm{prim}=%.1f$ GeV"%(channel, E_p))
    plt.plot(*s_aprot, "-",  label="Antiproton")
    plt.plot(*s_aneut, "--", label="Antineutron")
    plt.plot(*s_adeut, "-",  label="Antideuteron")
    plt.plot(*s_ahel,  "-",  label="Antihelium")
    plt.plot(*s_nu,     ":", label="Neutrinos")
    plt.plot(s_elpos[0], s_elpos[1], "-.",  label="Electron")
    plt.plot(s_elpos[0], s_elpos[2], "--", label="Positron")
    plt.xlabel(r"Secondary energy, $E$ [GeV]")
    plt.ylabel(r"$E^2\mathrm{d}\sigma/\mathrm{d}E$ [mb GeV]")
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(E_p*1e-4, E_p)
    plt.ylim(1e-11, 1e3)
    plt.legend()
    plt.tight_layout()
    plt.savefig("plot.pdf")
    #plt.show()


if __name__ == "__main__":
    
    from numpy import logspace,log10,savetxt,transpose

    init() # Initialise AAfrag 2; load interpolation tables
    
    E_p = 100 # Primary energy in GeV
    k = 1     # Reaction channel
              # p-p,He-p,p-He,He-He,C-p,Al-p,Fe-p,pbar-p,pbar-He
              # Note that the interaction C-p,Al-p,Fe-p are not implemented for
              # antideuteron and antihelium, while pbar-p,pbar-He are
              # exclusively implemented for antideuteron and antihelium
    E_s = logspace(-4, log10(E_p), 100) # Seondary energy in GeV

    # q is the index of the baryon number or flavor (neutrinos)
    # spec_gam   - 1: gamma
    # spec_pap   - 1: proton, 2: antiproton
    # spec_elpos - 1: electrons, 2: positrons
    # spec_nu    - 1: electron neutrino, 2: anti-electron neutrino,
    #              3: muon neutrino, 4: anti-muon neutrino
    # spec_nan   - 1: neutron, 2: antineutron
    # spec_ad    - 1: antideuteron
    # spec_ah    - 1: antihelium-3
    q=2
    s_aprot = spec_pap(E_p,E_s,q,k)
    s_aneut = spec_nan(E_p,E_s,q,k)
    q=1
    s_gam   = spec_gam(E_p,E_s,q,k)
    s_adeut = spec_ad(E_p,E_s,q,k)
    s_ahel  = spec_ah(E_p,E_s,q,k)
    s_nu    = sum([spec_nu(E_p,E_s,q,k) for q in [1,2,3,4]])
    q=1; s_el  = spec_elpos(E_p,E_s,q,k)
    q=2; s_pos = spec_elpos(E_p,E_s,q,k)

    # Store results for plotting
    savetxt("spec_aprot", transpose([E_s, s_aprot]),     fmt='%.5e')
    savetxt("spec_aneut", transpose([E_s, s_aneut]),     fmt='%.5e')
    savetxt("spec_gam",   transpose([E_s, s_gam]),       fmt='%.5e')
    savetxt("spec_adeut", transpose([E_s, s_adeut]),     fmt='%.5e')
    savetxt("spec_ahel",  transpose([E_s, s_ahel]),      fmt='%.5e')
    savetxt("spec_nu_tot",transpose([E_s, s_nu]),        fmt='%.5e')
    savetxt("spec_elpos", transpose([E_s, s_el, s_pos]), fmt='%.5e')

    example_plot(k, E_p)
