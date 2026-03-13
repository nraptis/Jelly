#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridMaskV64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskV64>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x9207u),
            jelly::test_brew::CaseShift(pCaseIndex, 0x9208u));
      },
      "SpiralGridMaskV64", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridMaskV64", aError);
}

