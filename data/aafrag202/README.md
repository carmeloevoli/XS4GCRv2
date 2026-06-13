# `AAfrag2`: Interpolation routines for Monte Carlo results on secondary production in proton-proton, proton-nucleus and nucleus-nucleus interactions


`AAfrag 2` is a tool that interpolates results relevant for secondary interactions in cosmic ray studies from the Monte Carlo simulation QGSJET-II-04m. The calculation of the production cross section of photons, neutrinos, electrons, positrons, protons, antiprotons in p-p, p-He, He-p, He-He, C-p-, Al-p, Fe-p interactions, as well as production cross section of antideuteron and antihelium-3 in p-p, p-He, He-p, He-He, pbar-p and pbar-He interactions, are included. The energy range of primary particles ranges from GeV to EeV; for antinuclei, only energies relevant for secondary production in the Galaxy are included. The tool allows the users to benefit from the advantages of a Monte Carlo simulation, with minimal computational effort. The results from the Monte Carlo simulations were stored in tables, and the main purpose of `AAfrag 2` is to provide the user with convenient interpolation routines. The interpolation is performed using binomial interpolation, with a fill value 0 outside the data range.

### Functions and variables

The program includes five functions that are intended to be used by the user (`spec_gam`, `spec_nu`, `spec_elpos`, `spec_pap`, `spec_nan`, `spec_ad`, `spec_ah`) which interpolate the production spectra of secondaries {photons, neutrinos/antineutrinos, electrons/positrons, protons/antiprotons, neutrons/antineutrons, antideuterons and antihelium-3} in various cosmic ray interactions. The output is $E_s^2 d\sigma/dE_s$ in mb GeV. An example of their uses is given in the example programs. The functions have the same input parameters: `(E_p, E_s, q, k)`:

- `E_p`: Total energy of primary particle in the lab frame
- `spec_pap`: - `E_s`: Total kinetic energy of the secondary particle
- `q`: Particle type; `spec_gam` (1:photon), `spec_pap` (1:proton, 2:antiproton), `spec_elpos` (1:electrons, 2:positrons), `spec_nu` (1: electron neutrino, 2: anti-electron neutrino, 3: muon neutrino, 4: anti-muon neutrino), `spec_nan` (1: neutron, 2: antineutron), `spec_ad` (1: antideuteron), `spec_ah` (1: antihelion+antitriton).
- `k`: Reaction type 1..7: {p-p,He-p,p-He,He-He,C-p,Al-p,Fe-p,pbar-p,pbar-He}. The latter two are only implemented for `spec_ad` and `spec_ah`, while the C-p, Al and Fe-p are not included.

Valid primary energy range:

- p-p (`k=1`) `E_p=[4, 3.9e11]` GeV
- p-He (`k=2`) `E_p=[4, 2.5e9]` GeV
- H-ep (`k=3`) `E_p=[4, 2.5e9]` GeV
- He-He (`k=4`) `E_p=[5, 2.5e9]` GeV
- C-p (`k=5`) `E_p=[16, 2.5e9]` GeV
- Al-p (`k=6`) `E_p=[40, 2.5e9]` GeV
- Fe-p (`k=7`) `E_p=[100, 2.5e9]` GeV

Outside the valid energy ranges, the cross section is set to 0.  In the Fortran version warning is given and the output set to zero when invalid parameters are used. No parameter checks are preformed in the Python version.

The production of antideuteron and antihelion is only considered in the energy range relevant for secondary production, ~`[20, 50e3]` GeV (down to few GeV for ap-p and ap-He collisions) (see arXiv:2002.10481).

## Installation

To compile the Fortran version program, run

> make AAfrag

Then,

> ./AAfrag.out 

runs the example program.

The Python 3 version for the common pedestrian can be found in AAfrag.py.
It can be run with

> python3 AAfrag.py

The script can either be executed as a standalone script, or imported as a
Python module. 

## Documentation

For full documentation see [https://sourceforge.net/projects/aafrag/](https://sourceforge.net/projects/aafrag/).

## References

When using this program, please cite

- M. Kachelriess, S. Ostapchenko and J. Tjemsland, *Comput. Phys. Commun.* **287** (2023), 108698 [arXiv:2206.00998].
- M. Kachelrieß, I. V. Moskalenko and S. Ostapchenko, *Comput. Phys. Commun.* **245** (2019), 106846 [arXiv:1904.05129]

The program is based on work published in the following references:

- S. Ostapchenko, *Phys. Rev. D* **83**, 014018 (2011) [arXiv:1010.1869]; *EPJ Web Conf.* **52**, 02001 (2013)
- M. Kachelrieß, I. V. Moskalenko, and S. S. Ostapchenko, *Astrophys. J.* **803**, 54 (2015) [arXiv:    1502.04158]
- M. Kachelrieß, S. Ostapchenko and J. Tjemsland, *JCAP* **08** (2020), 048 [arXiv:2002.10481]; J. Tjemsland, *PoS* **TOOLS2020** (2021), 006 [arXiv:2012.12252]

You may consider to cite these works if they are of relevance to your application.

## Changelog

*v2.02*

- Fix wrong primary energy in the Fortran version (Thanks to I. Moskalenko for pointing this out)
- Fix "gap" between in tables `nu_HeHe04L` and `nu_HeHe04`, and `nu_He_pL` and `nu_He_p`
- Reduce minimum primary energy for C-p, Al-p and Fe-p from 900 GeV to 26 GeV, 58 GeV and 120 GeV, respectively.
- Degrade stop conditions to warning messages in the Fortran version

---

This work is licensed under a [Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License](http://creativecommons.org/licenses/by-nc-nd/4.0/)
