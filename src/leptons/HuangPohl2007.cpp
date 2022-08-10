#include "XS4GCR/leptons/HuangPohl2007.h"

#include <algorithm>

#include "XS4GCR/core/cgs.h"
#include "XS4GCR/core/gsl.h"
#include "XS4GCR/core/utilities.h"

namespace XS4GCR {

HuangPohl2007SecLep::HuangPohl2007SecLep(const PID& lepton) {
  using std::log;
  m_lepton = lepton;
  double E_proj_min = 1.24 * pow(1.05, 1.) * cgs::GeV;
  double E_proj_max = 1.24 * pow(1.05, 374.) * cgs::GeV;
  size_t E_proj_size = 374;
  m_lgEproj = UTILS::LinAxis(log(E_proj_min), log(E_proj_max), E_proj_size);
  double T_sec_min = 1.058950e-02 * cgs::GeV;
  double T_sec_max = 9.443313e+07 * cgs::GeV;
  size_t T_sec_size = 201;
  m_lgTsec = UTILS::LinAxis(log(T_sec_min), log(T_sec_max), T_sec_size);
  m_sigma_pp = Grid<double>(E_proj_size, T_sec_size);
  m_sigma_Hep = Grid<double>(E_proj_size, T_sec_size);
  init();
}

void HuangPohl2007SecLep::print() const {
  LOGI << "using HUANGPOHL2007 lepton production model:";
  LOGI << "Huang et al., 2007, Astropart. Phys., 27, 429";
}

std::shared_ptr<SecondaryLeptons> HuangPohl2007SecLep::clone() { return std::make_shared<HuangPohl2007SecLep>(*this); }

void HuangPohl2007SecLep::init() {
  checkDatafilesExist();
  readDataFiles();
}

double HuangPohl2007SecLep::get(const PID& projectile, const TARGET& target, const double& T_proj,
                                const double& T_lepton) const {
  using std::log;

  const double lgE_proj = log(T_proj + cgs::protonMassC2);
  const auto lgE_proj_range = std::make_pair(m_lgEproj.front(), m_lgEproj.back());

  const double lgT_sec = log(T_lepton);
  const auto lgT_sec_range = std::make_pair(m_lgTsec.front(), m_lgTsec.back());

  double value = 0;

  if (UTILS::inRange(lgE_proj, lgE_proj_range) && UTILS::inRange(lgT_sec, lgT_sec_range)) {
    if (projectile == H1 && target == TARGET::H) {
      auto z = m_sigma_pp.get();
      value = GSL::interpolate2d<double>(m_lgEproj, m_lgTsec, z, lgE_proj, lgT_sec);
    } else if (projectile == He4 && target == TARGET::H) {
      auto z = m_sigma_Hep.get();
      value = GSL::interpolate2d<double>(m_lgEproj, m_lgTsec, z, lgE_proj, lgT_sec);
    }
  }
  return value;
}

void HuangPohl2007SecLep::checkDatafilesExist() {
  std::vector<std::string> inputfiles{eminus_p_datafile, eminus_he_datafile,  eplus_p_datafile,
                                      eplus_he_datafile, prodxsec_p_datafile, prodxsec_he_datafile};
  std::for_each(inputfiles.begin(), inputfiles.end(), [](std::string s) {
    if (!UTILS::fileExist(s)) throw std::runtime_error("error in reading an input file for HuangPohl2007 model.");
  });
}

std::vector<double> read_spectra_file(const std::string& filename, const size_t& x_size, const size_t& y_size) {
  std::vector<double> v;
  char s[20];
  std::ifstream infile(filename, std::ios::in);
  assert(infile.is_open());
  for (size_t i = 0; i < x_size; ++i) {
    for (size_t j = 0; j < y_size; ++j) {
      infile.get(s, 16);
      s[11] = 'E';
      v.push_back(atof(s));
    }
    infile.get(*s);
  }
  infile.close();
  return v;
}

std::vector<double> read_production_file(const std::string& filename, const size_t& x_size) {
  std::vector<double> v;
  char s[20];
  std::ifstream infile(filename, std::ios::in);
  assert(infile.is_open());
  for (size_t i = 0; i < x_size; ++i) {
    infile.get(s, 16);
    s[11] = 'E';
    v.push_back(atof(s));
  }
  infile.close();
  return v;
}

void HuangPohl2007SecLep::readDataFiles() {
  size_t T_sec_size = m_lgTsec.size();
  size_t E_proj_size = m_lgEproj.size();
  auto eminus_p_spectra = read_spectra_file(eminus_p_datafile, T_sec_size, E_proj_size);
  auto eminus_he_spectra = read_spectra_file(eminus_he_datafile, T_sec_size, E_proj_size);
  auto eplus_p_spectra = read_spectra_file(eplus_p_datafile, T_sec_size, E_proj_size);
  auto eplus_he_spectra = read_spectra_file(eplus_he_datafile, T_sec_size, E_proj_size);
  auto prod_xsec_p = read_production_file(prodxsec_p_datafile, E_proj_size);
  auto prod_xsec_he = read_production_file(prodxsec_he_datafile, E_proj_size);
  //   for (size_t ix = 0; ix < T_sec_size; ix++)  TODO remove this!
  //     for (size_t iy = 0; iy < E_proj_size; iy++) {
  //       if (m_lepton == electron) {
  //         m_sigma_pp.get(ix, iy) = eminus_p_spectra.at(ix * E_proj_size + iy) * prod_xsec_p.at(iy);
  //         m_sigma_Hep.get(ix, iy) = eminus_he_spectra.at(ix * E_proj_size + iy) * prod_xsec_he.at(iy);
  //       } else if (m_lepton == positron) {
  //         m_sigma_pp.get(ix, iy) = eplus_p_spectra.at(ix * E_proj_size + iy) * prod_xsec_p.at(iy);
  //         m_sigma_Hep.get(ix, iy) = eplus_he_spectra.at(ix * E_proj_size + iy) * prod_xsec_he.at(iy);
  //       }
  //     }
  for (size_t ix = 0; ix < T_sec_size; ix++)
    for (size_t iy = 0; iy < E_proj_size; iy++) {
      if (m_lepton == electron) {
        m_sigma_pp.get(iy, ix) = eminus_p_spectra.at(ix * E_proj_size + iy) * prod_xsec_p.at(iy);
        m_sigma_Hep.get(iy, ix) = eminus_he_spectra.at(ix * E_proj_size + iy) * prod_xsec_he.at(iy);
      } else if (m_lepton == positron) {
        m_sigma_pp.get(iy, ix) = eplus_p_spectra.at(ix * E_proj_size + iy) * prod_xsec_p.at(iy);
        m_sigma_Hep.get(iy, ix) = eplus_he_spectra.at(ix * E_proj_size + iy) * prod_xsec_he.at(iy);
      }
    }
  const double units = cgs::mbarn / cgs::GeV;
  m_sigma_pp.for_each([units](double& s) { s *= units; });
  m_sigma_Hep.for_each([units](double& s) { s *= units; });
}

}  // namespace XS4GCR