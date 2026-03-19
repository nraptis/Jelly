#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridMaskHH64.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskHH64>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x9108u));
      },
      "SwapGridMaskHH64", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridMaskHH64", aError);
}

