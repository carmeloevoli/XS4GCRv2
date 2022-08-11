#include "XS4GCR/models/secAAfrag101.h"

#include "XS4GCR/core/cgs.h"

namespace XS4GCR {
namespace AAfrag101 {

LookupTables::LookupTables(ParticleTypes particle) : m_particle(particle) { init(); }

void LookupTables::initAntiprotonTables() {
  // auto lgTsecMin = std::log10(81283.0547 * cgs::eV);
  // auto lgTsecMax = std::log10(7.94328225E+20 * cgs::eV);
  // auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
  // {
  //   std::string filename = "data/AAfrag101_pap_p_p_04L";
  //   if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
  //   auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
  //   m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
  //   m_sigma_le_pp.readTable(3);
  // }
  // {
  //   std::string filename = "data/AAfrag101_pap_p_p_04";
  //   if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
  //   auto lgTprojMin = std::log10(10. * cgs::GeV);
  //   auto lgTprojMax = std::log10(3.96868071E+20 * cgs::eV);
  //   auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
  //   m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
  //   m_sigma_he_pp.readTable(3);
  // }
}

void LookupTables::initGammaTables() {
  auto lgTsecMin = std::log10(81283.051616409954 * cgs::eV);
  auto lgTsecMax = std::log10(7.9432823472427893E+020 * cgs::eV);
  auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
  {
    std::string filename = "data/AAfrag101/gam_p_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pp.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_p_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pHe.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_He_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_Hep = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_Hep.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_He_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_HeHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_HeHe.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_p_p_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(3.9686807552000000E+020 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
    m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pp.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_p_He_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_pHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pHe.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_He_p_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_Hep = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_Hep.readTable(2);
  }
  {
    std::string filename = "data/AAfrag101/gam_He_He_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_HeHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_HeHe.readTable(2);
  }
}

void LookupTables::initNeutrinoTables() {
  auto lgTsecMin = std::log10(81283.0547 * cgs::eV);
  auto lgTsecMax = std::log10(7.94328225E+20 * cgs::eV);
  auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
  {
    std::string filename = "data/AAfrag101/nu_p_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pp.readNuTables();
  }
  {
    std::string filename = "data/AAfrag101/nu_p_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pHe.readNuTables();
  }
  {
    std::string filename = "data/AAfrag101/nu_He_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_Hep = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_Hep.readNuTables();
  }
  {
    std::string filename = "data/AAfrag101/nu_He_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_HeHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_HeHe.readNuTables();
  }
  {
    std::string filename = "data/AAfrag101/nu_p_p_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(3.96868071E+20 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
    m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pp.readNuTables();
  }
  {
    std::string filename = "data/AAfrag101/nu_p_He_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_pHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pHe.readNuTables();
  }
  // { // TODO wrong!
  //   std::string filename = "data/AAfrag101/nu_He_p_04";
  //   if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
  //   auto lgTprojMin = std::log10(10. * cgs::GeV);
  //   auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
  //   auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
  //   m_sigma_he_Hep = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
  //   m_sigma_he_Hep.readTable(2);
  // }
  // {
  //   std::string filename = "data/AAfrag101/nu_He_He_04";
  //   if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
  //   auto lgTprojMin = std::log10(10. * cgs::GeV);
  //   auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
  //   auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
  //   m_sigma_he_HeHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
  //   m_sigma_he_HeHe.readTable(2);
  // }
}

void LookupTables::initPositronTables() {
  auto lgTsecMin = std::log10(81283.051616409954 * cgs::eV);
  auto lgTsecMax = std::log10(7.9432823472427893E+020 * cgs::eV);
  auto lgTsecAxis = UTILS::LinAxis(lgTsecMin, lgTsecMax, 1600);
  {
    std::string filename = "data/AAfrag101/el_p_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pp = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pp.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_p_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(4. * cgs::GeV, 10. * cgs::GeV, 7);
    m_sigma_le_pHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_pHe.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_He_p_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_Hep = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_Hep.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_He_He_04L";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto TprojAxis = UTILS::LinAxis(5. * cgs::GeV, 10. * cgs::GeV, 6);
    m_sigma_le_HeHe = AAfragTable(filename, TprojAxis, lgTsecAxis);
    m_sigma_le_HeHe.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_p_p_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(3.9686807552000000E+020 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 54);
    m_sigma_he_pp = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pp.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_p_He_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_pHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_pHe.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_He_p_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_Hep = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_Hep.readTable(3);
  }
  {
    std::string filename = "data/AAfrag101/el_He_He_04";
    if (!UTILS::fileExist(filename)) throw std::runtime_error("problem with reading data file");
    auto lgTprojMin = std::log10(10. * cgs::GeV);
    auto lgTprojMax = std::log10(2.5056888000000000E+018 * cgs::eV);
    auto lgTprojAxis = UTILS::LinAxis(lgTprojMin, lgTprojMax, 43);
    m_sigma_he_HeHe = AAfragTable(filename, lgTprojAxis, lgTsecAxis);
    m_sigma_he_HeHe.readTable(3);
  }
}

void LookupTables::init() {
  if (m_particle == ParticleTypes::PBAR) {
    LOGW << "loading AAfrag101 antiproton tables";
    initAntiprotonTables();
  } else if (m_particle == ParticleTypes::GAMMA) {
    LOGW << "loading AAfrag101 gamma tables";
    initGammaTables();
  } else if (m_particle == ParticleTypes::POSITRON) {
    LOGW << "loading AAfrag101 positron tables";
    initPositronTables();
  } else if (m_particle == ParticleTypes::ALLNUS) {
    LOGW << "loading AAfrag101 neutrino tables";
    initNeutrinoTables();
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
        return m_sigma_le_Hep.get(T_proj * 4., std::log10(T_sec));
      case Channel::HeHe:
        return m_sigma_le_HeHe.get(T_proj * 4., std::log10(T_sec));
    }
  } else {
    switch (channel) {
      case Channel::pp:
        return m_sigma_he_pp.get(std::log10(T_proj), std::log10(T_sec));
      case Channel::pHe:
        return m_sigma_he_pHe.get(std::log10(T_proj), std::log10(T_sec));
      case Channel::Hep:
        return m_sigma_he_Hep.get(std::log10(T_proj * 4.), std::log10(T_sec));
      case Channel::HeHe:
        return m_sigma_he_HeHe.get(std::log10(T_proj * 4.), std::log10(T_sec));
    }
  }
  return 0;
}

}  // namespace AAfrag101
}  // namespace XS4GCR