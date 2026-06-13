subroutine user_main
  implicit none
  integer k,q
  double precision E_p,E_s,s1,s2,t(4)
  double precision spec_pap,spec_nan,spec_elpos,spec_gam,spec_nu,spec_ad,spec_ah

  write(*,*)'in main'
  
  E_p = 100 ! Primary energy in GeV
  k=1       ! Reaction channel
            ! p-p,p-He,He-p,He-He,C-p,Al-p,Fe-p,pbar-p,pbar-He
            ! Note that the interaction C-p,Al-p,Fe-p are not implemented for
            ! antideuteron and antihelium, while pbar-p,pbar-He are
            ! exclusively implemented for antideuteron and antihelium

  open(11,file='spec_aprot'); open(12,file='spec_aneut');
  open(13,file='spec_gam');   open(14,file='spec_nu');
  open(15,file='spec_nu_tot');open(16,file='spec_elpos');
  open(17,file='spec_adeut'); open(18,file='spec_ahel');

  
  E_s = 0.0001d0 ! Secondary energy in GeV (is incremented in do loop)
  do 
     q=2
     ! q is the index of the baryon number or flavor (neutrinos)
     ! spec_gam   q- 1: gamma
     ! spec_pap   q- 1: proton, 2: antiproton
     ! spec_elpos q- 1: electrons, 2: positrons
     ! spec_nu    q- 1: electron neutrino, 2: anti-electron neutrino,
     !               3: muon neutrino, 4: anti-muon neutrino
     ! spec_nan   q- 1: neutron, 2: antineutron
     ! spec_ad    q- 1: antideuteron
     ! spec_ah    q- 1: antihelium-3
     s1 = spec_pap(E_p,E_s,q,k)            ! E^2*dsigma/dE in GeV*mbarn
     write(11,*) E_s, s1
     s1 = spec_nan(E_p,E_s,q,k)
     write(12,*) E_s, s1
     s1 = spec_gam(E_p,E_s,1,k)
     write(13,*) E_s, s1
     q=1

     s1 = spec_ad(E_p,E_s,1,k)
     write(17,*) E_s, s1
     s1 = spec_ah(E_p,E_s,1,k)
     write(18,*) E_s, s1

     s1 = 0d0 ! Sum of neutrinos
     do q=1,4
        t(q) = spec_nu(E_p,E_s,q,k) ! Each neutrino species
        s1 = s1+t(q)
     end do
     write(14,*) E_s, t
     write(15,*) E_s, s1
     q=1; s1 = spec_elpos(E_p,E_s,q,k)
     q=2; s2 = spec_elpos(E_p,E_s,q,k)
     write(16,*) E_s, s1, s2
     E_s = 1.1d0*E_s ! Increment secondary energy
     if (E_s>E_p) exit
  end do

  write(*,*)'everything done'
 
end subroutine user_main
