// Copyright (c) 2017 Carmelo Evoli - MIT License
#include "XS4GCR/fragmentations/Evoli2026.h"

#include <plog/Log.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <set>
#include <stdexcept>
#include <utility>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
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

Evoli2026::Evoli2026(Evoli2026FallbackModel fallbackModel) : fallbackModel(fallbackModel) { init(); }

void Evoli2026::print() const {
  LOGI << "using Evoli2026 fragmentation model: IAEA 2026 direct-channel table and ghost list";
}

void Evoli2026::init() {
  if (!UTILS::fileExists(directChannelsFilename)) throw std::runtime_error("missing Evoli2026 direct-channel file");
  if (!UTILS::fileExists(ghostListFilename)) throw std::runtime_error("missing Evoli2026 ghost-list file");

  directChannels = FragmentationChannels(directChannelsFilename);
  ghostTree = GhostTree(ghostListFilename);
  setSigma();
}

std::shared_ptr<Fragmentation> Evoli2026::clone() { return std::make_shared<Evoli2026>(*this); }

bool Evoli2026::hasChannel(const FragmentationChannel& ch) const {
  std::set<PID> activeGhosts;
  return hasChannel(ch, activeGhosts);
}

bool Evoli2026::hasChannel(const FragmentationChannel& ch, std::set<PID> activeGhosts) const {
  if (directChannels.hasChannel(ch)) return true;
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

double Evoli2026::direct(const FragmentationChannel& ch, double T_n) const {
  if (directChannels.hasChannel(ch)) {
    const auto& energies = directChannels.getEnergies();
    const auto& crossSections = directChannels.getCrossSections(ch);
    if (energies.empty() || crossSections.empty()) return 0.;

    if (T_n < energies.front()) return 0.;
    if (T_n >= energies.back()) return crossSections.back();
    if (T_n == energies.front()) return crossSections.front();

    return GSL::linearInterpolate<double>(directChannels.getLogEnergies(), crossSections, std::log(T_n));
  }

  if (!isDirectFragmentationChannel(ch)) return 0.;

  const PID& projectile = ch.first;
  const PID& fragment = ch.second;
  if (fallbackModel == Evoli2026FallbackModel::ST99) {
    return yieldx(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), T_n);
  }
  return wsigma(projectile.getZ(), projectile.getA(), fragment.getZ(), fragment.getA(), T_n);
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
