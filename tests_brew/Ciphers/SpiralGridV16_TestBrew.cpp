#include <string>

#include "../../src/Encryption/Ciphers/SpiralGrid/SpiralGridV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridV16>(
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x5216u));
      },
      "SpiralGridV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SpiralGridV16", aError);
}

