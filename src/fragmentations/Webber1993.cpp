// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/fragmentations/Webber1993.h"

#include <cmath>
#include <memory>
#include <stdexcept>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/pid.h"

namespace {

extern "C" void set_sigma_(int*);
extern "C" double wsigma_(int*, int*, int*, int*, double*);
extern "C" void yieldx_(int*, int*, int*, int*, float*, float*);

void setSigma() {
  int cdr = 99;
  set_sigma_(&cdr);
}

double wsigma(int IZ, int IA, int JZ, int JA, double T_n) {
  double T_n_MeV = T_n / XS4GCR::cgs::MeV;
  const double cs_mb = wsigma_(&IZ, &IA, &JZ, &JA, &T_n_MeV);
  return cs_mb * XS4GCR::cgs::mbarn;
}

double yieldx(int IZ, int IA, int JZ, int JA, double T_n) {
  float cs_mb = 0.;
  float T_n_MeV = static_cast<float>(T_n / XS4GCR::cgs::MeV);
  yieldx_(&IZ, &IA, &JZ, &JA, &T_n_MeV, &cs_mb);
  return cs_mb * XS4GCR::cgs::mbarn;
}

double Ferrando1998He2H(const double& T_n, const int& Z_I, const int& Z_F) {
  const double T_GeV = (T_n / XS4GCR::cgs::GeV < 1.51) ? T_n / XS4GCR::cgs::GeV : 1.51;
  const double Z = (Z_I < 26) ? Z_I : 26;
  const double mu_E = 0.1601136 - 0.21994302 * T_GeV + 0.08903134 * T_GeV * T_GeV;
  const double delta_E = 0.40183405 + 5.60541311 * T_GeV - 1.95868946 * T_GeV * T_GeV;
  const double fZI_Z = -2.90627699 + 1.19911266 * std::log(Z);
  return std::exp(mu_E * std::pow(std::fabs(Z_I - Z_F - fZI_Z * delta_E), 1.43));
}

}  // namespace

namespace XS4GCR {

Webber1993::Webber1993() { init(); }

void Webber1993::print() const { std::cout << "# using Webber1993 fragmentation model: Webber et al. 1993" << '\n'; }

void Webber1993::init() { setSigma(); }

std::shared_ptr<Fragmentation> Webber1993::clone() { return std::make_shared<Webber1993>(*this); }

bool Webber1993::hasChannel(const FragmentationChannel& ch) const {
  const PID& proj = ch.first;
  const PID& frag = ch.second;
  if (proj == frag) return false;
  if (proj.getZ() <= 2 || proj.getZ() > 92) return false;
  if (frag.getZ() < 1 || frag.getA() < 1) return false;
  if (frag.getA() >= proj.getA()) return false;
  if (frag.getZ() > proj.getZ()) return false;
  if (proj.getA() - frag.getA() < proj.getZ() - frag.getZ()) return false;
  return true;
}

double Webber1993::direct(const FragmentationChannel& ch, const double& T_n) const {
  const PID& proj = ch.first;
  const PID& frag = ch.second;
  double direct = 0.;
  if (frag.getZ() <= 3)
    direct = yieldx(proj.getZ(), proj.getA(), frag.getZ(), frag.getA(), T_n);
  else
    direct = wsigma(proj.getZ(), proj.getA(), frag.getZ(), frag.getA(), T_n);
  return direct;
}

double Webber1993::getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts) {
  if (do_ghosts) {
    throw std::runtime_error("Webber1993 does not implement ghost contributions");
  }
  if (!hasChannel(ch)) return 0.;

  double value = direct(ch, T_n);
  if (target == TARGET::He) {
    value *= Ferrando1998He2H(T_n, ch.first.getZ(), ch.second.getZ());
  } else if (target != TARGET::H) {
    throw std::runtime_error("target not implemented in Webber1993");
  }
  return value;
}

}  // namespace XS4GCR
