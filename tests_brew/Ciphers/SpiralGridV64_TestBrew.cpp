#include <string>

#include "../../src/Ciphers/SpiralGrid/SpiralGridV64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridV64>(
            jelly::test_brew::CaseShift(pCaseIndex, 0x5264u));
      },
      "SpiralGridV64", &aError);
  return jelly::test_brew::ReportFlatResult("SpiralGridV64", aError);
}
