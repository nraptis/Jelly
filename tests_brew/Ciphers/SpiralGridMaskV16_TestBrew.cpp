#include <string>

#include "../../src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridMaskV16>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x9203u),
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x9204u));
      },
      "SpiralGridMaskV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SpiralGridMaskV16", aError);
}

