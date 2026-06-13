!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! AAfrag 2: Interpolation routines for Monte Carlo results on secondary     !!
!! production including light antinuclei in hadronic interactions            !!
!!                                                                           !!
!! Copyright: M. Kachelriess, S. Ostapchenko, J. Tjemsland (2023)            !!
!! Lisence:   CC BY-NC 4.0                                                   !!
!!                                                                           !!
!! see https://sourceforge.net/projects/aafrag/ for more info                !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! This file is not intended to be changed for normal use. The user should 
! make changes in user.f90. The functions used by the user is located on
! below line ~300.

!=============================================================================!
! Main program
program AAfrag201
  call banner
  call init       ! Initialise AAfrag
  call user_main  ! User function; The intension is that the user 
end program AAfrag201
!=============================================================================!
subroutine banner
  write(*,*) 
  write(*,*) '################################################################'
  write(*,*) '#  AAfrag v2.02                                                #'
  write(*,*) '#  Authors: M. Kachelriess, S. Ostapchenko, J. Tjemsland       #'
  write(*,*) '#  Refs.                                                       #'
  write(*,*) '#     Submitted to Comput.Phys.Commun.      [arXiv:2206.00998] #'
  write(*,*) '#     Comput.Phys.Commun. 245 (2019) 106846 [arXiv:1904.05129] #'
  write(*,*) '#  see https://sourceforge.net/projects/aafrag/ for more info. #'
  write(*,*) '#                                                              #'
  write(*,*) '# Disclaimer: This program comes without any guarantees!       #'
  write(*,*) '# Beware of errors and use common sense interpreting results!  #'
  write(*,*) '################################################################'
end subroutine banner
!=============================================================================!
! Initialises AAfrag by calling the appropriate initialisation routines
subroutine init
  call init_nucl
  call init_low
  call init_high
end subroutine init
!=============================================================================!
! Module for global parameters, and variables for loading interpolation tables
module spectra
  implicit none
  save
  
  !!! Photon, (anti-) proton, (anti-) neutron, electron/positron and neutrino
  integer, parameter :: n1=540,n2=1600,n3=8,n5=2,n6=7 ! Size of tables
  ! Threshold energy and mimimum kinetic energy in photon, 
  double precision, parameter :: E0=1.d1,E_min=1e-4
  ! Arrays for tables
  double precision gamqgs(0:n1,0:n2,1,n6),gamqgsL(0:n3,0:n2,1,n6)
  double precision elqgs(0:n1,0:n2,n5,n6),elqgsL(0:n3,0:n2,n5,n6)
  double precision nuqgs(0:n1,0:n2,n6,n6),nuqgsL(0:n3,0:n2,n6,n6)
  double precision papqgs(0:n1,0:n2,n5,n6),papqgsL(0:n3,0:n2,n5,n6)
  double precision nanqgs(0:n1,0:n2,n5,n6),nanqgsL(0:n3,0:n2,n5,n6)
  ! Arrays storing the index of the last E_p, E_s and q
  integer gamnums(3,n6),gamLnums(3,n6),elnums(3,n6),elLnums(3,n6)
  integer nunums(3,n6),nuLnums(3,n6),papnums(3,n6),papLnums(3,n6)
  integer nannums(3,n6),nanLnums(3,n6)
  
  !!! Antideuteron and antihelium
  integer, parameter :: n1d=130, n2d=400 ! Size of tables
  ! Arrays storing the index of the last E_p, E_s and q
  integer          adnums(1:3,9),ahnums(1:3,9) 
  ! Arrays for tables
  double precision adqgs(0:n1d,0:n2d,1,9), ahqgs(0:n1d,0:n2d,1,9)
  
  !!! General parameters
  ! Define small values to get around log(0) for logarithmic interpolation
  integer, parameter :: small_e=-40
  double precision   :: small=1d-42
  double precision, parameter :: m_p = 0.9382d0        ! proton mass/GeV 
end module spectra
!=============================================================================!
! Simple input check
subroutine check_values(E_p,E_s,k,ierr)
  use spectra
  implicit none
  integer k,ierr
  logical, save :: flag_s=.true., flag_p=.true., flag_kd=.true., flag_k=.true.
  double precision E_p,E_s,Emin,Emax

  ierr = 0

  select case (k)
  case (1)                ! pp
     Emin=4d0;    Emax=3.96d11
  case (2)                ! pHe
     Emin=4d0;    Emax=2.50d9
  case (3)                ! Hep
     Emin=5d0;    Emax=2.50d9
  case (4)                ! HeHe
     Emin=5d0;    Emax=2.50d9
  case (5)                ! C-p
     Emin=16d0;   Emax=2.50d9
  case (6)                ! Al-p
     Emin=40d0;   Emax=2.50d9
  case (7)                ! Fe-p
     Emin=100d0;  Emax=2.50d9
  case(8:9)               ! pbar-p, pbar-He; only dbar...
     if (flag_kd) then
        write(*,*)'WARNING: reaction ',k,' only implemented for antideuterons'
        flag_kd = .false.
     endif
     ierr=1
  case default
     write(*,*)'WARNING: reaction ',k,' not implemented'
     if (flag_k) then
        write(*,*)'WARNING: reaction ',k,' only implemented for antideuterons'
        flag_k = .false.
     endif
     ierr=2
  end select

  if (E_s<E_min.or.E_s>Emax) then
     if (flag_s) then
        write(*,*)'WARNING: E_s=', E_s, ' outside implemented energy range.'
        flag_s = .false.
     end if
     ierr=3
  end if

  if (E_p<Emin.or.E_p>Emax) then
     if (flag_p) then
        write(*,*)'WARNING: E_p=', E_p, ' outside implemented energy range.'
        flag_p = .false.
     end if
     ierr=4
  end if
  
end subroutine check_values
!=============================================================================!
! Perform bilinear interpolation between two data points
double precision function bilinear_interp(x, y, f1, f2, f3, f4, x1, x2, y1, y2)
  use spectra
  implicit none
  double precision x, y, f1, f2, f3, f4, x1, x2, y1, y2, t, u
  t = (x2 - x)/(x2 - x1)*f1 + (x - x1)/(x2 - x1)*f2
  u = (x2 - x)/(x2 - x1)*f4 + (x - x1)/(x2 - x1)*f3
  if (f1 < small_e .or. f2 < small_e .or. f3 < small_e .or. f4 < small_e) then
    bilinear_interp = small_e-10
  else
    bilinear_interp  = (y2 - y)/(y2 - y1)*t + (y - y1)/(y2 - y1)*u
  endif
end function bilinear_interp
!=============================================================================!
! Initialise interpolation tables for photons, electrons/positrons, neutrinos,
! (anti-) protons and (anti-)neutrons with logarithmic binning
subroutine init_high
  use spectra
  implicit none
  integer k
  character(len=7) :: f="Tables/", fname

  do k=1,7 ! Loop over interaction channel

     select case (k)
     case (1); fname = 'p_p_04'
     case (2); fname = 'p_He04'
     case (3); fname = 'He_p04'
     case (4); fname = 'HeHe04'
     case (5); fname = 'C_p_04'
     case (6); fname = 'Al_p04'
     case (7); fname = 'Fe_p04'
     case default
        write(*,*) 'reaction',k,'not implemented'
     end select
     
    call read_table(f//'gam_'//fname//' ',gamqgs(:,:,:,k),n1,n2,1,gamnums(:,k))
    call read_table(f//'el_'//fname//'  ',elqgs(:,:,:,k),n1,n2,n5,elnums(:,k))
    call read_table(f//'nu_'//fname//'  ',nuqgs(:,:,:,k),n1,n2,n6,nunums(:,k))
    call read_table(f//'pap_'//fname//' ',papqgs(:,:,:,k),n1,n2,n5,papnums(:,k))
    call read_table(f//'nan_'//fname//' ',nanqgs(:,:,:,k),n1,n2,n5,nannums(:,k))
  
  end do

  write(*,*) 'init high done' 

end subroutine init_high
!=============================================================================!
! Initialise interpolation tables
! 
! At low energies (E0<10 GeV) additional data points are given with linear
! spacing for p-p, p-He, He-p and He-He interactions
subroutine init_low
  use spectra
  implicit none
  integer k
  character*7 fname
  character(len=7) :: f="Tables/" 
  do k=1,4 ! Loop over interaction channel

    select case (k)
    case (1); fname = 'p_p_04L'
    case (2); fname = 'p_He04L'
    case (3); fname = 'He_p04L'
    case (4); fname = 'HeHe04L'
    case default
       write(*,*) 'Init_low failed: reaction',k,'not implemented'
       stop
    end select

    call read_table(f//'gam_'//fname,gamqgsL(:,:,:,k),n3,n2,1,gamLnums(:,k))
    call read_table(f//'el_'//fname//' ',elqgsL(:,:,:,k),n3,n2,n5,elLnums(:,k))
    call read_table(f//'nu_'//fname//' ',nuqgsL(:,:,:,k),n3,n2,n6,nuLnums(:,k))
    call read_table(f//'pap_'//fname,papqgsL(:,:,:,k),n3,n2,n5,papLnums(:,k))
    call read_table(f//'nan_'//fname,nanqgsL(:,:,:,k),n3,n2,n5,nanLnums(:,k))
 
  end do

  write(*,*) 'init low done' 
  
end subroutine init_low
!=============================================================================!
! Initialise interpolation tables for antideuteron and antihelium
subroutine init_nucl
  use spectra
  implicit none
  integer k
  character*6 filename

  do k=1,9 ! Loop over interaction channels

     select case (k)
     case(1); filename='p_p_04'
     case(2); filename='p_He04'
     case(3); filename='He_p04'
     case(4); filename='HeHe04'
     case(5); continue
     case(6); continue
     case(7); continue
     case(8); filename='ap_p04'
     case(9); filename='apHe04'
     case default
        write(*,*) 'Init_high failed: reaction',k,'not implemented'
        stop
     end select
     
     call read_table('Tables/ad_'//filename//'  ', &
        adqgs(:,:,1,k), n1d, n2d, 1, adnums(:,k))
     call read_table('Tables/ah_'//filename//'  ', &
        ahqgs(:,:,1,k), n1d, n2d, 1, ahnums(:,k)) 
  end do

  write(*,*) 'init nucl done' 

end subroutine init_nucl
!=============================================================================!
! Read tables from file
! 
! This subroutine is only used for the antideuteron and antihelium-3
! initialisation. For these particles, the reading is done "dynamically" since
! the binning type (e.g. linear and logarithic), and minimum/maximum energies
! varies between interactions
subroutine read_table(filename, tab, n1, n2, n3, last_element)
   use spectra, only : small
   implicit none
   integer n1, n2, n3
   integer err,n_r,n_c,n_q,last_element(3)
   double precision tab(0:n1,0:n2,n3,1)!, r
   double precision, allocatable :: tmp(:)
   character(len=18) filename
   character(len=1000000) line

   ! Read content
   open(21,file=filename)
   ! Determine E_s and number of columns 
   read(21, "(a)") line
   !do i = 1, 10000 !!! SLOW !!!
   !  read(line, *, iostat=err) ( r, j=1,i )
   !  if (err.ne.0) exit
   !end do
   !n_c = i - 2
   n_c = (len(trim(line))+1)/12 - 1
   allocate(tmp(0:n_c))
   
   tab = 0
   read(line, *) tab(0, 0:n_c,1,1)
   n_q=1; n_r=1; n_r=1;
   do     
      read(21, *, iostat=err) tmp
      if (tmp(0).eq.0) then ! new q
        n_q = n_q + 1
        n_r = 0 ! Note: all q should have same r
      endif
      tab(n_r, 0:n_c, n_q, 1) = tmp
      n_r = n_r + 1
      if (err.lt.0) then
         exit
      else if (err.gt.0) then
         write(*, *) 'Something went wrong when trying to read '//filename
         stop "error"
      endif
   end do
   close(21)
   
   last_element(1) = n_r-2
   last_element(2) = n_c
   last_element(3) = n_q

   where (tab.lt.small) tab = 1e-30*small
   tab = log10(tab)

   write(*,*) filename, "n_r=",n_r, "n_c=", n_c, "n_q=", n_q
end subroutine read_table
!=============================================================================!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!                                                                           !!
!!  The following functions (spec_*) are inteded to be used by the user by   !!
!!  appropriately changing user.f90.                                         !!
!!                                                                           !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! The functions spec* evaluates the production cross sections E^2 dsigma/dE in
! mb GeV. They have all the same input parameters: (E_p,E_s,qq,k)
! 
!        E_p: (double precision)
!             Primary energy in GeV
!        E_s: (double precision)
!             Secondary energy in GeV
!        q:   (integer)
!             Index of particle "sub-species"
!             Photons   (spec_gam)   - 1: gamma
!             Protons   (spec_pap)   - 1: proton, 2: antiproton
!             Electrons (spec_elpos) - 1: electrons, 2: positrons
!             Neutrinos (spec_nu)    - 1: electron neutrino,
!                       2: anti-electron neutrino, 3: muon neutrino,
!                       4: anti-muon neutrino
!             Neutrons  (spec_nan)   - 1: neutron, 2: antineutron
!             Deuteron  (spec_ad)    - 1: antideuteron
!             Helium-3  (spec_ah)    - 1: antihelium-3
!        k:   (integer)
!             Index of reaction channel 1:p-p, 2:He-p, 3:p-He, 4:He-He, 5:C-p,
!             6:Al-p, 7:Fe-p, 8:pbar-p, 9:pbar-He
!             Note that 4-7 are not implemented for antideuteron and
!             antihelium-3, while 8-9 are exclusively implemented for
!             antideuteron and antihelium-3.
!    Returns:
!        Production cross section E^2 dsigma/dE in mb GeV for primary energy
!        E_p and secondary energy E_s. The interaction type is determined by
!        q, and the secondary particle is determined by f and q.

!=============================================================================!
! Photon production spectrum
double precision function spec_gam(E_p,E_s,qq,k)
  use spectra
  implicit none
  integer i,j,k,ierr,qq,n,q
  double precision E_p,E_s,x,y,r,logd,d_min,dn
  double precision bilinear_interp
  
  ! qq dummy 
  q = 1 ! Force q=1, since it is the only alternative
  ! Check inputs
  call check_values(E_p,E_s,k,ierr)
  if (ierr>0) then
     spec_gam = 0.d0     
     return
  end if

  x = log10(E_p)
  y = log10(E_s)

  ! Different binning at low (linear) and high (logarithmic) energies
  ! The binning is "regular", and we can therefore directly read out
  ! the needed indices
  if (E_p<E0) then ! Low primary energy (linear)
    ! Extract binning info
    n = gamLnums(2,k) ! number of bins
    d_min = gamqgsL(0, 1, q, k) ! shift
    dn    = gamqgsL(0, 3, q, k) - gamqgsL(0, 2, q, k) ! bin size
    
    ! Find index of the first data point
    ! The new value will be somewhere between (i,j) and (i+1,j+1)
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_gam=0; return; endif
    i = int(E_p - 10**gamqgsL(1,0,q,k)) + 1
    
    ! Perform interpolation between the two data points
    r = bilinear_interp(x,y-dn,gamqgsL(i,j,q,k),gamqgsL(i+1,j,q,k), &
        gamqgsL(i+1,j+1,q,k),gamqgsL(i,j+1,q,k),gamqgsL(i,0,q,k), &
        gamqgsL(i+1,0,q,k),gamqgsL(0,j,q,k), gamqgsL(0,j+1,q,k) )
  else ! High primary energy (logarithmic)
    n = gamnums(2,k)
    d_min = gamqgs(0, 1, q, k)
    dn    = (gamqgs(0, n, q, k) - d_min)/(n-1)
    logd  = gamqgs(2, 0, q, k) - gamqgs(1, 0, q, k)
   
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_gam=0; return; endif
    i = int((x - gamqgs(1, 0, q, k))/logd) + 1
    
    r = bilinear_interp(x,y-dn,gamqgs(i,j,q,k),gamqgs(i+1,j,q,k), &
        gamqgs(i+1,j+1,q,k),gamqgs(i,j+1,q,k),gamqgs(i,0,q,k), &
        gamqgs(i+1,0,q,k),gamqgs(0,j,q,k), gamqgs(0,j+1,q,k) )
  end if

  if (r < small_e) then
    spec_gam = 0 ! Set to exaclty zero
  else
    spec_gam = E_s*10.d0**r
  endif

end function spec_gam
!=============================================================================!
! Electron and positron production spectrum
double precision function spec_elpos(E_p,E_s,q,k)
  use spectra
  implicit none
  integer i,j,k,ierr,n,q
  double precision E_p,E_s,x,y,r,logd,d_min,dn
  double precision bilinear_interp

  call check_values(E_p,E_s,k,ierr)
  if (ierr>0) then
     spec_elpos = 0.d0     
     return
  end if

  x = log10(E_p)
  y = log10(E_s)

  if (E_p<E0) then
    n = elLnums(2,k)
    d_min = elqgsL(0, 1, q, k)
    dn    = (elqgsL(0, n, q, k) - d_min)/(n-1)
     
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_elpos=0; return; endif
    i = int(E_p - 10**elqgsL(1,0,q,k)) + 1

    r = bilinear_interp(x,y-dn,elqgsL(i,j,q,k),elqgsL(i+1,j,q,k), &
        elqgsL(i+1,j+1,q,k),elqgsL(i,j+1,q,k),elqgsL(i,0,q,k), &
        elqgsL(i+1,0,q,k),elqgsL(0,j,q,k), elqgsL(0,j+1,q,k) )
  else
    n = elnums(2,k)
    d_min = elqgs(0, 1, q, k)
    dn    = (elqgs(0, n, q, k) - d_min)/(n-1)
    logd  = elqgs(2, 0, q, k) - elqgs(1, 0, q, k)
   
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_elpos=0; return; endif
    i = int((x - elqgs(1, 0, q, k))/logd) + 1
    
    r = bilinear_interp(x,y-dn,elqgs(i,j,q,k),elqgs(i+1,j,q,k), &
        elqgs(i+1,j+1,q,k),elqgs(i,j+1,q,k),elqgs(i,0,q,k), &
        elqgs(i+1,0,q,k),elqgs(0,j,q,k), elqgs(0,j+1,q,k) )
  end if

  if (r < small_e) then
    spec_elpos = 0
  else
    spec_elpos = E_s*10.d0**r
  endif

end function spec_elpos
!=============================================================================!
! Neutrino production spectrum
double precision function spec_nu(E_p,E_s,q,k)
  use spectra
  implicit none
  integer i,j,k,ierr,n,q
  double precision E_p,E_s,x,y,r,logd,d_min,dn
  double precision bilinear_interp

  call check_values(E_p,E_s,k,ierr)
  if (ierr>0) then
     spec_nu = 0.d0     
     return
  end if

  x = log10(E_p)
  y = log10(E_s)

  if (E_p<E0) then
    n = nuLnums(2,k)
    d_min = nuqgsL(0, 1, q, k)
    dn    = (nuqgsL(0, n, q, k) - d_min)/(n-1)
     
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_nu=0; return; endif
    i = int(E_p - 10**nuqgsL(1,0,q,k)) + 1
    r = bilinear_interp(x,y-dn,nuqgsL(i,j,q,k),nuqgsL(i+1,j,q,k), &
        nuqgsL(i+1,j+1,q,k),nuqgsL(i,j+1,q,k),nuqgsL(i,0,q,k), &
        nuqgsL(i+1,0,q,k),nuqgsL(0,j,q,k), nuqgsL(0,j+1,q,k) )
  else
    n = nunums(2,k)
    d_min = nuqgs(0, 1, q, k)
    dn    = nuqgs(0, 2, q, k) - nuqgs(0, 1, q, k)
    logd  = nuqgs(2, 0, q, k) - nuqgs(1, 0, q, k)
   
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_nu=0; return; endif
    i = int((x - nuqgs(1, 0, q, k))/logd) + 1
    
    r = bilinear_interp(x,y-dn,nuqgs(i,j,q,k),nuqgs(i+1,j,q,k), &
        nuqgs(i+1,j+1,q,k),nuqgs(i,j+1,q,k),nuqgs(i,0,q,k), &
        nuqgs(i+1,0,q,k),nuqgs(0,j,q,k), nuqgs(0,j+1,q,k) )
  end if



  if (r < small_e) then
    spec_nu = 0
  else
    spec_nu = E_s*10.d0**r
  endif

end function spec_nu
!=============================================================================!
! (Anti-) proton production spectrum
double precision function spec_pap(E_p,E_s,q,k)
  use spectra
  implicit none
  integer i,j,k,ierr,n,q
  double precision E_p,E_s,x,y,r,logd,d_min,dn
  double precision bilinear_interp

  call check_values(E_p,E_s,k,ierr)
  if (ierr>0) then
     spec_pap = 0.d0     
     return
  end if

  x = log10(E_p)
  y = log10(E_s)

  if (E_p<E0) then
    n = papLnums(2,k)
    d_min = papqgsL(0, 1, q, k)
    dn    = (papqgsL(0, n, q, k) - d_min)/(n-1)
     
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_pap=0; return; endif
    i = int(E_p - 10**papqgsL(1,0,q,k)) + 1
    r = bilinear_interp(x,y-dn,papqgsL(i,j,q,k),papqgsL(i+1,j,q,k), &
        papqgsL(i+1,j+1,q,k),papqgsL(i,j+1,q,k),papqgsL(i,0,q,k), &
        papqgsL(i+1,0,q,k),papqgsL(0,j,q,k), papqgsL(0,j+1,q,k) )
  else
    n = papnums(2,k)
    dn    = papqgs(0, 6, q, k) - papqgs(0, 5, q, k)
    logd  = papqgs(2, 0, q, k) - papqgs(1, 0, q, k)

    j = int((y - papqgs(0, 1, q, k))/dn)
    if (j < 1 .or. j > n-1) then; spec_pap=0; return; endif
    i = int((x - papqgs(1, 0, q, k))/logd) + 1

    r = bilinear_interp(x,y-dn,papqgs(i,j,q,k),papqgs(i+1,j,q,k), &
        papqgs(i+1,j+1,q,k),papqgs(i,j+1,q,k),papqgs(i,0,q,k), &
        papqgs(i+1,0,q,k),papqgs(0,j,q,k), papqgs(0,j+1,q,k) )

  end if

  if (r < small_e) then
    spec_pap = 0
  else
    spec_pap = E_s*10.d0**r
  endif

end function spec_pap
!=============================================================================!
! (Anti-) neutron production spectrum
double precision function spec_nan(E_p,E_s,q,k)
  use spectra
  implicit none
  integer i,j,k,ierr,n,q
  double precision E_p,E_s,x,y,r,logd,d_min,dn
  double precision bilinear_interp

  call check_values(E_p,E_s,k,ierr)
  if (ierr>0) then
     spec_nan = 0.d0     
     return
  end if

  x = log10(E_p)
  y = log10(E_s)

  if (E_p<E0) then
    n = nanLnums(2,k)
    d_min = nanqgsL(0, 1, q, k)
    dn    = (nanqgsL(0, n, q, k) - d_min)/(n-1)
     
    j = int((y - d_min)/dn)
    if (j < 1 .or. j > n-1) then; spec_nan=0; return; endif
    i = int(E_p - 10**nanqgsL(1,0,q,k)) + 1
    r = bilinear_interp(x,y-dn,nanqgsL(i,j,q,k),nanqgsL(i+1,j,q,k), &
        nanqgsL(i+1,j+1,q,k),nanqgsL(i,j+1,q,k),nanqgsL(i,0,q,k), &
        nanqgsL(i+1,0,q,k),nanqgsL(0,j,q,k), nanqgsL(0,j+1,q,k) )
  else

    n = nannums(2,k)
    dn    = nanqgs(0, 2, q, k) - nanqgs(0, 1, q, k)
    logd  = nanqgs(2, 0, q, k) - nanqgs(1, 0, q, k)
  
    j = int((y - nanqgs(0, 1, q, k))/dn)
    if (j < 1 .or. j > n-1) then; spec_nan=0; return; endif
    i = int((x - nanqgs(1, 0, q, k))/logd) + 1
    
    r = bilinear_interp(x,y-dn,nanqgs(i,j,q,k),nanqgs(i+1,j,q,k), &
        nanqgs(i+1,j+1,q,k),nanqgs(i,j+1,q,k),nanqgs(i,0,q,k), &
        nanqgs(i+1,0,q,k),nanqgs(0,j,q,k), nanqgs(0,j+1,q,k) )
  end if

  if (r < small_e) then
    spec_nan = 0
  else
    spec_nan = E_s*10.d0**r
  endif

end function spec_nan
!=============================================================================!
! Antideuteron production spectrum
double precision function spec_ad(E_p,E_s,qq,k)
  use spectra
  implicit none
  integer i,j,k,q,ii,qq
  double precision E_p,E_s
  double precision x,y,r
  double precision bilinear_interp
  logical, save :: flag=.false., flag_k=.false.

  ! Check input values
  if (k>4 .and. k<8) then
    if (.not. flag) then
      write(*, *) "WARNING: Reaction", k, &
                  "not implemented for antideuterons. Returning 0!"
      flag = .true. ! print only once...
    endif
    spec_ad = 0
    return
  endif

  q = qq
  if (q.le.0 .or. q.gt.adnums(3,k)) then
    if (.not. flag_k) then
     write(*,*) "WARNING: Only antideuterons are included in spec_ad. Using q=1"
     flag_k=.true.
    endif
    q = 1
  endif
 
  x = log10(E_p)
  y = log10(E_s)

  ! Outside interpolation range?
  if (x.lt.adqgs(1,0,q,k) .or. x.gt.adqgs(adnums(1,k),0,q,k) .or. &
      y.lt.adqgs(0,1,q,k) .or. y.gt.adqgs(0,adnums(2,k),q,k)) then
     spec_ad = 0
    return
  endif 
   
  ! Find index dynamically
  do ii=1,n1
    if (x<adqgs(ii,0,q,k)) then
      i = ii-1; exit
    end if
  end do
  do ii=1,n2
    if (y<adqgs(0,ii,q,k)) then
      j = ii-1; exit
    end if
  end do

  ! Perform interpolation
  r = bilinear_interp(x,y,adqgs(i,j,q,k), adqgs(i+1,j,q,k),adqgs(i+1,j+1,q,k), &
        adqgs(i,j+1,q,k),adqgs(i,0,q,k),adqgs(i+1,0,q,k),adqgs(0,j,q,k), &
        adqgs(0,j+1,q,k) )
       

  if (r < 0.9*small_e) then
    spec_ad = 0
  else
    spec_ad = E_s*10.d0**r
  endif
  

end function spec_ad
!=============================================================================!
! Antihelium-3 production spectrum
double precision function spec_ah(E_p,E_s,qq,k)
  use spectra
  implicit none
  integer i,j,k,q,ii,qq
  double precision E_p,E_s
  double precision x,y,r
  double precision bilinear_interp
  logical, save :: flag=.false., flag_k=.false.

  if (k>4 .and. k<8) then
    if (.not. flag) then
      write(*, *) "WARNING: Reaction", k, &
                  "not implemented for antihelium. Returning 0!"
      flag = .true.
    endif
    spec_ah = 0
    return
  endif

  x = log10(E_p)
  y = log10(E_s)
  
  q = qq
  if (q.le.0 .or. q.gt.ahnums(3,k)) then
    if (.not. flag_k) then
     write(*,*) "WARNING: Only antihelion are included in spec_ah. Using q=1"
     flag_k=.true.
    endif
    q = 1
  endif

  if (x.lt.ahqgs(1,0,q,k) .or. x.gt.ahqgs(ahnums(1,k),0,q,k) .or. &
      y.lt.ahqgs(0,1,q,k) .or. y.gt.ahqgs(0,ahnums(2,k),q,k)) then
     spec_ah = 0
    return
  endif 
   
  do ii=1,n1
    if (x<ahqgs(ii,0,q,k)) then
      i = ii-1; exit
    end if
  end do
  do ii=1,n2
    if (y<ahqgs(0,ii,q,k)) then
      j = ii-1; exit
    end if
  end do
  
  r = bilinear_interp(x,y,ahqgs(i,j,q,k), ahqgs(i+1,j,q,k),ahqgs(i+1,j+1,q,k), &
        ahqgs(i,j+1,q,k),ahqgs(i,0,k,q),ahqgs(i+1,0,q,k),ahqgs(0,j,q,k), &
        ahqgs(0,j+1,q,k) )
  

  if (r < 0.9*small_e) then
    spec_ah = 0
  else
    spec_ah = E_s*10.d0**r
  endif
  
end function spec_ah
!=============================================================================!
