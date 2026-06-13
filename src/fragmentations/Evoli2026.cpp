// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/fragmentations/Evoli2026.h"

#include <plog/Log.h>

#include <algorithm>
#include <memory>
#include <set>
#include <stdexcept>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/utilities.h"

namespace {

extern "C" void set_sigma_(int*);
extern "C" double wsigma_(int*, int*, int*, int*, double*);
extern "C" void yieldx_(int*, int*, int*, int*, float*, float*);

void setSigma() {
  int cdr = 99;
  set_sigma_(&cdr);
}

double wsigma(int projectileZ, int projectileA, int fragmentZ, int fragmentA, double T_n) {
  double T_n_MeV = T_n / XS4GCR::cgs::MeV;
  const double sigmaMb = wsigma_(&projectileZ, &projectileA, &fragmentZ, &fragmentA, &T_n_MeV);
  return sigmaMb * XS4GCR::cgs::mbarn;
}

double yieldx(int projectileZ, int projectileA, int fragmentZ, int fragmentA, double T_n) {
  float sigmaMb = 0.;
  float T_n_MeV = static_cast<float>(T_n / XS4GCR::cgs::MeV);
  yieldx_(&projectileZ, &projectileA, &fragmentZ, &fragmentA, &T_n_MeV, &sigmaMb);
  return sigmaMb * XS4GCR::cgs::mbarn;
}

bool isDirectFragmentationChannel(const XS4GCR::FragmentationChannel& ch) {
  const XS4GCR::PID& projectile = ch.first;
  const XS4GCR::PID& fragment = ch.second;
  if (projectile == fragment) return false;
  if (projectile.getZ() < 2 || projectile.getZ() > 92) return false;
  if (fragment.getZ() < 1 || fragment.getA() < 1) return false;
  if (fragment.getA() >= projectile.getA()) return false;
  if (fragment.getZ() > projectile.getZ()) return false;
  if (projectile.getA() - fragment.getA() < projectile.getZ() - fragment.getZ()) return false;
  return true;
}

}  // namespace

namespace XS4GCR {

Evoli2026::Evoli2026() { init(); }

void Evoli2026::print() const {
  LOGI << "using Evoli2026 fragmentation model: Evoli et al. 2019 fit with ghost contributions";
}

void Evoli2026::init() {
  if (!UTILS::fileExists(fitDataFilename)) throw std::runtime_error("missing Evoli2026 fit-data file");
  if (!UTILS::fileExists(fitParamsFilename)) throw std::runtime_error("missing Evoli2026 fit-parameter file");
  if (!UTILS::fileExists(ghostListFilename)) throw std::runtime_error("missing Evoli2026 ghost-list file");

  fitData = FitData(fitDataFilename);
  fittingFunctions = FittingFunctions(fitParamsFilename);
  ghostTree = GhostTree(ghostListFilename);
  setSigma();
}

std::shared_ptr<Fragmentation> Evoli2026::clone() { return std::make_shared<Evoli2026>(*this); }

bool Evoli2026::hasChannel(const FragmentationChannel& ch) const {
  std::set<PID> activeGhosts;
  return hasChannel(ch, activeGhosts);
}

bool Evoli2026::hasChannel(const FragmentationChannel& ch, std::set<PID> activeGhosts) const {
  if (isDirectFragmentationChannel(ch)) return true;

  const PID& projectile = ch.first;
  const PID& child = ch.second;
  if (ghostTree.nParents(child) == 0) return false;
  if (!activeGhosts.insert(child).second) {
    throw std::runtime_error("cycle detected in Evoli2026 ghost tree at isotope " + child.str());
  }

  for (const auto& parent : ghostTree.getParents(child)) {
    if (hasChannel(std::make_pair(projectile, parent.first), activeGhosts)) return true;
  }
  return false;
}

double Evoli2026::getTotal(const FragmentationChannel& ch, const TARGET& target, const double& T_n, bool do_ghosts) {
  if (T_n <= 0.) return 0.;
  if (target == TARGET::He) return 0.;
  if (target != TARGET::H) throw std::runtime_error("target not implemented in Evoli2026");

  double value = do_ghosts ? withGhosts(ch, T_n) : direct(ch, T_n);
  return std::max(value, 0.);
}

double Evoli2026::bestfitNormalization(const FragmentationChannel& ch) const {
  const auto data = fitData.getData(ch);
  const PID& projectile = ch.first;
  const PID& fragment = ch.second;

  std::pair<double, double> bestfit(1e100, 1.);
  for (double norm = 0.1; norm < 10.; norm *= 1.01) {
    double chi2 = 0.;
    for (const auto& point : data) {
      if (point.sigmaError <= 0.) continue;

      double model = 0.;
      if (fragment.getZ() <= 3)
        model = norm * yieldx(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), point.T);
      else
        model = norm * wsigma(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), point.T);
      chi2 += pow2(point.sigma - model) / pow2(point.sigmaError);
    }
    if (chi2 < bestfit.first) {
      bestfit.first = chi2;
      bestfit.second = norm;
    }
  }
  return bestfit.second;
}

double Evoli2026::direct(const FragmentationChannel& ch, double T_n) const {
  if (!isDirectFragmentationChannel(ch)) return 0.;

  double value = 0.;
  if (fittingFunctions.hasChannel(ch)) {
    value = fittingFunctions.get(ch, T_n);
  } else {
    double norm = 1.;
    if (fitData.hasChannel(ch)) norm = bestfitNormalization(ch);

    const PID& projectile = ch.first;
    const PID& fragment = ch.second;
    if (fragment.getZ() <= 3)
      value = norm * yieldx(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), T_n);
    else
      value = norm * wsigma(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), T_n);
  }
  return std::max(value, 0.);
}

double Evoli2026::withGhosts(const FragmentationChannel& ch, double T_n) const {
  std::set<PID> activeGhosts;
  return withGhosts(ch, T_n, activeGhosts);
}

double Evoli2026::withGhosts(const FragmentationChannel& ch, double T_n, std::set<PID> activeGhosts) const {
  const PID& projectile = ch.first;
  const PID& child = ch.second;
  if (ghostTree.nParents(child) == 0) return direct(ch, T_n);
  if (!activeGhosts.insert(child).second) {
    throw std::runtime_error("cycle detected in Evoli2026 ghost tree at isotope " + child.str());
  }

  double value = direct(ch, T_n);
  for (const auto& parent : ghostTree.getParents(child)) {
    value += parent.second * withGhosts(std::make_pair(projectile, parent.first), T_n, activeGhosts);
  }
  return value;
}

}  // namespace XS4GCR
