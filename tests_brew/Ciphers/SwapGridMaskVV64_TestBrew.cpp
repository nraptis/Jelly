#include <string>

#include "../../src/Ciphers/SwapGrid/SwapGridMaskVV64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskVV64>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x9105u));
      },
      "SwapGridMaskVV64", &aError);
  return jelly::test_brew::ReportFlatResult("SwapGridMaskVV64", aError);
}

