#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridMaskVV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskVV16>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x9101u));
      },
      "SwapGridMaskVV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridMaskVV16", aError);
}

