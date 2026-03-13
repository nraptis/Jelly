#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridMaskH16.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskH16>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x9201u),
            jelly::test_brew::CaseShift(pCaseIndex, 0x9202u));
      },
      "SpiralGridMaskH16", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridMaskH16", aError);
}

