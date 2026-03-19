#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridMaskHV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskHV16>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x9103u));
      },
      "SwapGridMaskHV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridMaskHV16", aError);
}

