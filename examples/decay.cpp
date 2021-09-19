#include "XS4GCR.h"
using namespace XS4GCR;

/**
 * @example decay.cpp
 *
 * In this example the decay properties of a
 * nuclear species are displayed.
 *
 * @see https://arxiv.org/abs/1711.09616
 */

/**
 * @brief Display half-life in year and decay mode
 *
 * @param particle identifier
 */
void print_decay_properties(std::shared_ptr<NucleiChart> decays, PID particle) {
  auto tau_kyr = decays->getHalftime(particle) / cgs::kyr;
  if (tau_kyr > 0) {
    auto mode = decays->getMode(particle);
    std::cout << particle << " decays in " << tau_kyr << " kyr";
    std::cout << " with mode " << mode << "\n";
  } else {
    std::cout << particle << " is stable\n";
  }
}

/**
 * @brief Main function for example5
 */
int main() {
  try {
    LOG::startup_information();
    XSECS xsec;
    auto decays = xsec.createNucleiChart();
    PID Be10 = PID(4, 10);
    PID B10 = PID(5, 10);
    print_decay_properties(decays, Be10);
    print_decay_properties(decays, B10);
  } catch (const std::exception& e) {
    LOGF << "exception caught with message: " << e.what();
  }
}
