#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridMaskH64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskH64>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x9205u),
            jelly::test_brew::CaseShift(pCaseIndex, 0x9206u));
      },
      "SpiralGridMaskH64", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridMaskH64", aError);
}

