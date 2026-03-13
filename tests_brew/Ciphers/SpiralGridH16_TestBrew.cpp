#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridH16.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridH16>(
            jelly::test_brew::CaseShift(pCaseIndex, 0x5116u));
      },
      "SpiralGridH16", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridH16", aError);
}

