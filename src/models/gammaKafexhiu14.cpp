#include "XS4GCR/models/gammaKafexhiu14.h"

#include <cmath>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/common.h"

namespace XS4GCR {
namespace Kafexhiu14 {

#define m_p (cgs::protonMassC2 / cgs::GeV)
#define m_pi (cgs::pionMassC2 / cgs::GeV)
#define sigma_pp(A) (ppInelastic(A) / cgs::mbarn)

double E_pi_max_LAB(double Tp) {
  //	"""
  //	This function calculates the maximum pi0 energy that
  //	is allowed by the kinematics the LAB frame.
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	Epi_maxLAB - is in [GeV]
  //	"""
  const double s = 2.0 * m_p * (Tp + 2.0 * m_p);
  const double gamma_CM = (Tp + 2.0 * m_p) / std::sqrt(s);
  const double E_pi_CM = (s - 4.0 * pow2(m_p) + pow2(m_pi)) / (2.0 * std::sqrt(s));
  const double P_pi_CM = std::sqrt(pow2(E_pi_CM) - pow2(m_pi));
  const double Beta_CM = std::sqrt(1.0 - 1.0 / pow2(gamma_CM));

  const double Epi_maxLAB = gamma_CM * (E_pi_CM + P_pi_CM * Beta_CM);  // in GeV
  return Epi_maxLAB;
}

double Egamma_max(double Tp) {
  //	"""
  //	This function calculates the maximum gamma-ray energy
  //	allowed by the kinematics in the LAB frame.
  //	Tp and Eg_max are in GeV.
  //	"""
  double gamma_pi_LAB = E_pi_max_LAB(Tp) / m_pi;
  double Beta_pi_LAB = std::sqrt(1.0 - 1.0 / pow2(gamma_pi_LAB));
  double Eg_max = (m_pi / 2.0) * gamma_pi_LAB * (1.0 + Beta_pi_LAB);
  return Eg_max;
}

double sigma_1_pi(double Tp) {
  //	"""
  //	This function calculates the one pi0 production cross section.
  //	It is valid for Tp <= 2 GeV. The channel included is pp->pp(pi0)
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_1_pi - is one pi0 production cross section in [mb]
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);
  const double M_RES = 1.1883;      // resonance effective mass in GeV
  const double Gamma_RES = 0.2264;  // resonance effective width in GeV
  const double sigma_0 = 7.66e-3;   // mb

  if (Tp <= Tp_th) return 0.0;

  if (Tp > Tp_th && Tp <= 2.0) {  // m
    double s = 2.0 * m_p * (Tp + 2.0 * m_p);
    double X = std::sqrt(s) - m_p;
    double eta =
        (std::sqrt(pow2(s - pow2(m_pi) - 4.0 * pow2(m_p)) - 16.0 * pow2(m_pi) * pow2(m_p))) / (2 * m_pi * sqrt(s));

    double g_RES = std::sqrt(pow2(M_RES) * (pow2(M_RES) + pow2(Gamma_RES)));
    double K_RES = std::sqrt(8.0) * M_RES * Gamma_RES * g_RES / (M_PI * std::sqrt(pow2(M_RES) + g_RES));
    double f_BW = m_p * K_RES / (pow2(pow2(X) - pow2(M_RES)) + pow2(M_RES * Gamma_RES));

    double XS_1_pi = sigma_0 * std::pow(eta, 1.95) * (1.0 + eta + std::pow(eta, 5.0)) * std::pow(f_BW, 1.86);
    return XS_1_pi;
  } else
    return 0.0;
}

double sigma_2_pi(double Tp) {
  //	"""
  //	This function calculates the 2 pion production cross section.
  //	It is valid Tp <= 2 GeV. The channels included here are
  //	1) p+p -> p+n +(pi+)+(pi0)
  //	2) p+p ->  D  +(pi+)+(pi0)
  //	3) p+p -> p+p +2(pi0)
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_2_pi - is the sum of the pi0 cross sections from the above channels in [mb]
  //	"""

  if (Tp < 0.56)                                                 // GeV
    return 0.0;                                                  // mb
  if (Tp >= 0.56 && Tp <= 2.0) {                                 // GeV
    double XS_2_pi = 5.7 / (1.0 + std::exp(-9.3 * (Tp - 1.4)));  // mb
    return XS_2_pi;
  } else
    return 0.0;
}

double multip_pi0_Geant4(double Tp) {
  //	"""
  //	This function calculates the average pi0 production multiplicity given by Geant4.
  //	This function is valid for TP>=1 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	multip_pi0 - is the average pi0 production multiplicity
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);
  if (Tp <= 2.0)  // GeV
    return 0.0;
  else if (Tp > 2.0 && Tp < 5.0) {  // GeV
    double Qp = (Tp - Tp_th) / m_p;
    double multip_pi0 = -6.0e-3 + 0.237 * Qp - 0.023 * pow2(Qp);
    return multip_pi0;
  } else {
    const double a_1 = 0.728;
    const double a_2 = 0.596;
    const double a_3 = 0.491;
    const double a_4 = 0.2503;
    const double a_5 = 0.117;

    double xi_p = (Tp - 3.0) / m_p;
    double multip_pi0 = a_1 * std::pow(xi_p, a_4) * (1.0 + std::exp(-a_2 * std::pow(xi_p, a_5))) *
                        (1.0 - std::exp(-a_3 * std::pow(xi_p, 0.25)));
    return multip_pi0;
  }
}

double multip_pi0_Pythia8(double Tp) {
  //	"""
  //	This function calculates the average pi0 production multiplicity given
  //	by Geant4 for Tp <= 50 GeV and Pythia 8 for Tp > 50 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	multip_pi0 - is the average pi0 production multiplicity
  //	"""

  if (Tp <= 50.0) {  // GeV
    return multip_pi0_Geant4(Tp);
  } else {
    const double a_1 = 0.652;
    const double a_2 = 0.0016;
    const double a_3 = 0.488;
    const double a_4 = 0.1928;
    const double a_5 = 0.483;

    double xi_p = (Tp - 3.0) / m_p;
    double multip_pi0 = a_1 * std::pow(xi_p, a_4) * (1.0 + std::exp(-a_2 * std::pow(xi_p, a_5))) *
                        (1.0 - std::exp(-a_3 * std::pow(xi_p, 0.25)));
    return multip_pi0;
  }
}

double multip_pi0_SIBYLL(double Tp) {
  //	"""
  //	This function calculates the average pi0 production multiplicity given
  //	by Geant4 for Tp <= 100 GeV and SIBYLL for Tp > 100 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	multip_pi0 - is the average pi0 production multiplicity
  //	"""

  if (Tp <= 100.0) {  // GeV
    return multip_pi0_Geant4(Tp);
  } else {
    const double a_1 = 5.436;
    const double a_2 = 0.254;
    const double a_3 = 0.072;
    const double a_4 = 0.075;
    const double a_5 = 0.166;

    double xi_p = (Tp - 3.0) / m_p;
    double multip_pi0 = a_1 * std::pow(xi_p, a_4) * (1.0 + std::exp(-a_2 * std::pow(xi_p, a_5))) *
                        (1.0 - std::exp(-a_3 * std::pow(xi_p, 0.25)));
    return multip_pi0;
  }
}

double multip_pi0_QGSJET(double Tp) {
  //	"""
  //	This function calculates the average pi0 production multiplicity given
  //	by Geant4 for Tp <= 100 GeV and QGSJET for Tp > 100 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	multip_pi0 - is the average pi0 production multiplicity
  //	"""

  if (Tp <= 100.0) {  // GeV
    return multip_pi0_Geant4(Tp);
  } else {
    const double a_1 = 0.908;
    const double a_2 = 0.0009;
    const double a_3 = 6.089;
    const double a_4 = 0.176;
    const double a_5 = 0.448;

    double xi_p = (Tp - 3.0) / m_p;
    double multip_pi0 = a_1 * std::pow(xi_p, a_4) * (1.0 + std::exp(-a_2 * std::pow(xi_p, a_5))) *
                        (1.0 - std::exp(-a_3 * std::pow(xi_p, 0.25)));
    return multip_pi0;
  }
}

double sigma_pi_Geant4(double Tp) {
  //	"""
  //	This function calculates the pi0 production cross section
  //	by using experimental data for Tp <= 2 GeV, and Geant4
  //	multiplicity for Tp > 2 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_pi0 - is the pi0 production cross section in [mb]
  //	"""
  double XS_pi0 = sigma_1_pi(Tp) + sigma_2_pi(Tp) + sigma_pp(Tp) * multip_pi0_Geant4(Tp);
  return XS_pi0;
}

double sigma_pi_Pythia8(double Tp) {
  //  """
  //	This function calculates the pi0 production cross section
  //	by using experimental data for Tp <= 2 GeV, Geant4
  //	multiplicity for 2 < Tp <= 50 GeV and Pythia8 multiplicity for Tp > 50 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_pi0 - is the pi0 production cross section in [mb]
  //	"""
  double XS_pi0 = sigma_1_pi(Tp) + sigma_2_pi(Tp) + sigma_pp(Tp) * multip_pi0_Pythia8(Tp);
  return XS_pi0;
}

double sigma_pi_SIBYLL(double Tp) {
  //  """
  //	This function calculates the pi0 production cross section
  //	by using experimental data for Tp <= 2 GeV, Geant4 multiplicity
  //	for 2 < Tp <= 100 GeV and SIBYLL multiplicity for Tp > 100 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_pi0 - is the pi0 production cross section in [mb]
  //	"""
  double XS_pi0 = sigma_1_pi(Tp) + sigma_2_pi(Tp) + sigma_pp(Tp) * multip_pi0_SIBYLL(Tp);
  return XS_pi0;
}

double sigma_pi_QGSJET(double Tp) {
  //  """
  //	This function calculates the pi0 production cross section
  //	by using experimental data for Tp <= 2 GeV, Geant4 multiplicity
  //	for 2 < Tp <= 100 GeV and QGSJET multiplicity for Tp > 100 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	XS_pi0 - is the pi0 production cross section in [mb]
  //	"""
  double XS_pi0 = sigma_1_pi(Tp) + sigma_2_pi(Tp) + sigma_pp(Tp) * multip_pi0_QGSJET(Tp);
  return XS_pi0;
}

double Amax_Geant4(double Tp) {
  //	"""
  //	This function calculates the peak value of the gamma-ray
  //	differential cross section. For Tp < 1 GeV is the fit from
  //	the experimental data, and for Tp >= 1 GeV is based on Geant4.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	Amax - is the peak of the gamma-ray differential cross section in [mb/GeV]
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);

  double theta_p = Tp / m_p;
  double Ltheta_p = std::log(theta_p);

  if (Tp <= Tp_th)
    return 0.0;
  else if (Tp < 1.0) {  // GeV
    double Amax = 5.9 * sigma_pi_Geant4(Tp) / E_pi_max_LAB(Tp);
    return Amax;
  } else if (Tp < 5.0) {  // GeV
    const double b_1 = 9.53;
    const double b_2 = -0.52;
    const double b_3 = 0.054;
    double Amax = b_1 * std::pow(theta_p, b_2) * std::exp(b_3 * pow2(Ltheta_p)) * sigma_pi_Geant4(Tp) / m_p;
    return Amax;
  } else {
    const double b_1 = 9.13;
    const double b_2 = -0.35;
    const double b_3 = 9.7e-3;
    double Amax = b_1 * std::pow(theta_p, b_2) * std::exp(b_3 * pow2(Ltheta_p)) * sigma_pi_Geant4(Tp) / m_p;
    return Amax;
  }
}

double Amax_Pythia8(double Tp) {
  //	"""
  //	This function calculates the peak value of the gamma-ray
  //	differential cross section. For Tp < 1 GeV is the fit from
  //	the experimental data, for 1 <= Tp <= 50 GeV is based on Geant4
  //	and for Tp > 50 GeV is based on Pythia8.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	Amax - is the peak of the gamma-ray differential cross section in [mb/GeV]
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);

  double theta_p = Tp / m_p;
  double Ltheta_p = std::log(theta_p);

  if (Tp > Tp_th && Tp <= 50.0) {
    return Amax_Geant4(Tp);
  } else {
    const double b_1 = 9.06;
    const double b_2 = -0.3795;
    const double b_3 = 0.01105;
    double Amax = b_1 * std::pow(theta_p, b_2) * std::exp(b_3 * pow2(Ltheta_p)) * sigma_pi_Pythia8(Tp) / m_p;
    return Amax;
  }
}

double Amax_SIBYLL(double Tp) {
  //	"""
  //	This function calculates the peak value of the gamma-ray
  //	differential cross section. For Tp < 1 GeV is the fit from
  //	the experimental data, for 1 <= Tp <= 100 GeV is based on Geant4
  //	and for Tp > 100 GeV is based on SIBYLL.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	Amax - is the peak of the gamma-ray differential cross section in [mb/GeV]
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);

  double theta_p = Tp / m_p;
  double Ltheta_p = std::log(theta_p);

  if (Tp > Tp_th && Tp <= 100.0) {
    return Amax_Geant4(Tp);
  } else {
    const double b_1 = 10.77;
    const double b_2 = -0.412;
    const double b_3 = 0.01264;
    double Amax = b_1 * std::pow(theta_p, b_2) * std::exp(b_3 * pow2(Ltheta_p)) * sigma_pi_SIBYLL(Tp) / m_p;
    return Amax;
  }
}

double Amax_QGSJET(double Tp) {
  //	"""
  //	This function calculates the peak value of the gamma-ray
  //	differential cross section. For Tp < 1 GeV is the fit from
  //	the experimental data, for 1 <= Tp <= 100 GeV is based on Geant4
  //	and for Tp > 100 GeV is based on QGSJET.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	Amax - is the peak of the gamma-ray differential cross section in [mb/GeV]
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);

  double theta_p = Tp / m_p;
  double Ltheta_p = std::log(theta_p);

  if (Tp > Tp_th && Tp <= 100.0) {
    return Amax_Geant4(Tp);
  } else {
    const double b_1 = 13.16;
    const double b_2 = -0.4419;
    const double b_3 = 0.01439;
    double Amax = b_1 * std::pow(theta_p, b_2) * std::exp(b_3 * pow2(Ltheta_p)) * sigma_pi_QGSJET(Tp) / m_p;
    return Amax;
  }
}

double F_Geant4(double Tp, double Egamma) {
  //	"""
  //	This function calculates the shape of the gamma-ray
  //	differential cross section function for a specific Tp.
  //	This function includes the experimental data
  //	for Tp < 1 GeV and Geant4 for Tp >= 1 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	FF - is the shape of the gamma-ray spectrum, it is unitless.
  //	"""
  constexpr double Tp_th = 2.0 * m_pi + (m_pi * m_pi) / (2.0 * m_p);

  double Y = Egamma + pow2(m_pi) / (4.0 * Egamma);
  double Y0 = Egamma_max(Tp) + pow2(m_pi) / (4.0 * Egamma_max(Tp));
  double X = (Y - m_pi) / (Y0 - m_pi);
  double theta = Tp / m_p;
  double kappa = 3.29 - 0.2 * std::pow(theta, -1.5);

  double q = (Tp - 1.0) / m_p;
  double C = 3.0 * m_pi / Y0;

  double FF = 0;
  if (X >= 0.0 and X < 1.0) {
    if (Tp > Tp_th && Tp < 1.0)  // GeV
      FF = std::pow(1. - X, kappa);
    else if (Tp >= 1.0 && Tp <= 4.0) {  // GeV
      double mu = 1.25 * std::pow(q, 1.25) * std::exp(-1.25 * q);
      double beta = mu + 2.45;
      double gamma = mu + 1.45;
      FF = std::pow(1. - X, beta) / std::pow(1. + X / C, gamma);
    } else if (Tp > 4.0 && Tp <= 20.0) {  // GeV
      double mu = 1.25 * std::pow(q, 1.25) * std::exp(-1.25 * q);
      double beta = 1.5 * mu + 4.95;
      double gamma = mu + 1.5;
      FF = std::pow(1. - X, beta) / std::pow(1. + X / C, gamma);
    } else if (Tp > 20.0 && Tp <= 100.0) {  // GeV
      FF = std::pow(1. - std::sqrt(X), 4.2) / (1 + X / C);
    } else if (Tp > 100.0) {  // GeV
      FF = std::pow(1. - std::sqrt(X), 4.9) / (1 + X / C);
    }
  }
  return FF;
}

double F_Pythia8(double Tp, double Egamma) {
  //	"""
  //	This function calculates the shape of the gamma-ray
  //	differential cross section function for a specific Tp.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 50 GeV and Pythia8 for Tp > 50 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	FF - is the shape of the gamma-ray spectrum, it is unitless.
  //	"""

  double Y = Egamma + pow2(m_pi) / (4.0 * Egamma);
  double Y0 = Egamma_max(Tp) + pow2(m_pi) / (4.0 * Egamma_max(Tp));
  double X = (Y - m_pi) / (Y0 - m_pi);
  double C = 3.5 * m_pi / Y0;

  if (X >= 0.0 && X < 1.0 && Tp > 50) {  // GeV
    double FF = std::pow(1.0 - std::sqrt(X), 4.0) / (1 + X / C);
    return FF;
  } else {
    return F_Geant4(Tp, Egamma);
  }
}

double F_SIBYLL(double Tp, double Egamma) {
  // 	"""
  //	This function calculates the shape of the gamma-ray
  //	differential cross section function for a specific Tp.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 100 GeV and SIBYLL for Tp > 100 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	FF - is the shape of the gamma-ray spectrum, it is unitless.
  //	"""

  double Y = Egamma + pow2(m_pi) / (4.0 * Egamma);
  double Y0 = Egamma_max(Tp) + pow2(m_pi) / (4.0 * Egamma_max(Tp));
  double X = (Y - m_pi) / (Y0 - m_pi);
  double C = 3.55 * m_pi / Y0;

  if (X >= 0.0 && X < 1.0 && Tp > 50) {  // GeV
    double FF = std::pow(1 - std::sqrt(X), 3.6) / (1 + X / C);
    return FF;
  } else {
    return F_Geant4(Tp, Egamma);
  }
}

double F_QGSJET(double Tp, double Egamma) {
  //  	"""
  //	This function calculates the shape of the gamma-ray
  //	differential cross section function for a specific Tp.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 100 GeV and QGSJET for Tp > 100 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	FF - is the shape of the gamma-ray spectrum, it is unitless.
  //	"""

  double Y = Egamma + pow2(m_pi) / (4.0 * Egamma);
  double Y0 = Egamma_max(Tp) + pow2(m_pi) / (4.0 * Egamma_max(Tp));
  double X = (Y - m_pi) / (Y0 - m_pi);
  double C = 3.55 * m_pi / Y0;

  if (X >= 0.0 && X < 1.0 && Tp > 50) {  // GeV
    double FF = std::pow(1. - std::sqrt(X), 4.5) / (1 + X / C);
    return FF;
  } else
    return F_Geant4(Tp, Egamma);
}

double dsigma_dEgamma_Geant4(double Tp, double Egamma) {
  //  """
  //	This function calculates the pp->pi0->gamma-ray
  //	differential cross section function.
  //	This function includes the experimental data
  //	for Tp < 1 GeV and Geant4 for Tp >= 1 GeV.
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	The value that is returned is in [mb/GeV]
  //	"""
  return Amax_Geant4(Tp) * F_Geant4(Tp, Egamma);
}

double dsigma_dEnu_Geant4(double Tp, double Egamma) { return 0; }

double dsigma_dEgamma_Pythia8(double Tp, double Egamma) {
  //	"""
  //	This function calculates the pp->pi0->gamma-ray
  //	differential cross section function.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 50 GeV and Pythia8 for Tp > 50 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	The value that is returned is in [mb/GeV]
  //	"""
  return Amax_Pythia8(Tp) * F_Pythia8(Tp, Egamma);
}

double dsigma_dEnu_Pythia8(double Tp, double Egamma) { return 0; }

double dsigma_dEgamma_SIBYLL(double Tp, double Egamma) {
  //	"""
  //	This function calculates the pp->pi0->gamma-ray
  //	differential cross section function.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 100 GeV and SIBYLL for Tp > 100 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	The value that is returned is in [mb/GeV]
  //	"""
  return Amax_SIBYLL(Tp) * F_SIBYLL(Tp, Egamma);
}

double dsigma_dEnu_SIBYLL(double Tp, double Egamma) { return 0; }

double dsigma_dEgamma_QGSJET(double Tp, double Egamma) {
  //	"""
  //	This function calculates the pp->pi0->gamma-ray
  //	differential cross section function.
  //	This function includes the experimental data for
  //	Tp < 1 GeV Geant4 for 1 <= Tp <= 100 GeV and QGSJET for Tp > 100 GeV
  //
  //	Tp - is the proton kinetic energy in the LAB frame in [GeV]
  //	The value that is returned is in [mb/GeV]
  //	"""
  return Amax_QGSJET(Tp) * F_QGSJET(Tp, Egamma);
}

double dsigma_dEnu_QGSJET(double Tp, double Egamma) { return 0; }

double dsigma_dE_gamma(double T_proj, double E_gamma, InteractionModel model) {
  double value = 0;
  if (model == InteractionModel::GEANT4)
    value = dsigma_dEgamma_Geant4(T_proj / cgs::GeV, E_gamma / cgs::GeV);
  else if (model == InteractionModel::PYTHIA8)
    value = dsigma_dEgamma_Pythia8(T_proj / cgs::GeV, E_gamma / cgs::GeV);
  else if (model == InteractionModel::SIBYLL)
    value = dsigma_dEgamma_SIBYLL(T_proj / cgs::GeV, E_gamma / cgs::GeV);
  else if (model == InteractionModel::QGSJET)
    value = dsigma_dEgamma_QGSJET(T_proj / cgs::GeV, E_gamma / cgs::GeV);
  return value * cgs::mbarn / cgs::GeV;
}

}  // namespace Kafexhiu14
}  // namespace XS4GCR