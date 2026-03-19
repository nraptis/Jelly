#include <string>

#include "../../src/Encryption/Ciphers/SpiralGrid/SpiralGridH64.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridH64>(
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x5164u));
      },
      "SpiralGridH64", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SpiralGridH64", aError);
}

