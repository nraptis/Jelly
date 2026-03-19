#include <string>

#include "../../src/Encryption/Ciphers/SpiralGrid/SpiralGridV64.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridV64>(
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x5264u));
      },
      "SpiralGridV64", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SpiralGridV64", aError);
}
