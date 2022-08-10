#include "XS4GCR/models/secAAfrag101.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {
namespace AAfrag101 {

LookupTables::LookupTables(ParticleTypes particle) : m_particle(particle) { init(); }

void LookupTables::init() {
  if (m_particle == ParticleTypes::PBAR) {
    LOGW << "loading AAfrag101 antiproton tables";
    auto lgTsecMin = std::log10(81283.0547 * cgs::eV);
    auto lgTsecMax = std::log10(7.94328225E+20 * cgs::eV);
    auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
    {
      std::string filename = "data/AAfrag101_pap_p_p_04L";
      if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
      auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
      m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
      m_sigma_le_pp.readTable(3);
    }
    {
      std::string filename = "data/AAfrag101_pap_p_p_04";
      if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
      auto lgTprojMin = std::log10(10. * cgs::GeV);
      auto lgTprojMax = std::log10(3.96868071E+20 * cgs::eV);
      auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
      m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
      m_sigma_he_pp.readTable(3);
    }
  } else if (m_particle == ParticleTypes::GAMMA) {
    LOGW << "loading AAfrag101 gamma tables";
    auto lgTsecMin = std::log10(81283.051616409954 * cgs::eV);
    auto lgTsecMax = std::log10(7.9432823472427893E+020 * cgs::eV);
    auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
    {
      std::string filename = "data/AAfrag101_gam_p_p_04L";
      if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
      auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
      m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
      m_sigma_le_pp.readTable(2);
    }
    {
      std::string filename = "data/AAfrag101_gam_p_p_04";
      if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
      auto lgTprojMin = std::log10(10. * cgs::GeV);
      auto lgTprojMax = std::log10(3.9686807552000000E+020 * cgs::eV);
      auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
      m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
      m_sigma_he_pp.readTable(2);
    }
  }
}

double LookupTables::get(Channel channel, double T_proj, double T_sec) const {
  const double T_split = 10. * cgs::GeV;
  if (T_proj < T_split) {
    switch (channel) {
      case Channel::pp:
        return m_sigma_le_pp.get(T_proj, std::log10(T_sec));
      case Channel::pHe:
        return m_sigma_le_pHe.get(T_proj, std::log10(T_sec));
      case Channel::Hep:
        return m_sigma_le_Hep.get(T_proj, std::log10(T_sec));
      case Channel::HeHe:
        return m_sigma_le_HeHe.get(T_proj, std::log10(T_sec));
    }
  } else {
    switch (channel) {
      case Channel::pp:
        return m_sigma_he_pp.get(std::log10(T_proj), std::log10(T_sec));
      case Channel::pHe:
        return m_sigma_he_pHe.get(std::log10(T_proj), std::log10(T_sec));
      case Channel::Hep:
        return m_sigma_he_Hep.get(std::log10(T_proj), std::log10(T_sec));
      case Channel::HeHe:
        return m_sigma_he_HeHe.get(std::log10(T_proj), std::log10(T_sec));
    }
  }
  return 0;
}

}  // namespace AAfrag101
}  // namespace XS4GCR