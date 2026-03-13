#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridV16.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridV16>(
            jelly::test_brew::CaseShift(pCaseIndex, 0x5216u));
      },
      "SpiralGridV16", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridV16", aError);
}

