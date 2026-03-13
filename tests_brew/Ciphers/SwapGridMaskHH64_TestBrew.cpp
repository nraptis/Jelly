#include <string>

#include "../../src/Ciphers/SwapGrid/SwapGridMaskHH64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskHH64>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x9108u));
      },
      "SwapGridMaskHH64", &aError);
  return jelly::test_brew::ReportFlatResult("SwapGridMaskHH64", aError);
}

