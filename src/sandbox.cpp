#include "XS4GCR.h"

int main(int argc, char* argv[]) {
  try {
    XS4GCR::LOG::startup_information();
  } catch (const std::exception& e) {
    LOGE << "exception caught with message: " << e.what();
  }
  return 0;
}