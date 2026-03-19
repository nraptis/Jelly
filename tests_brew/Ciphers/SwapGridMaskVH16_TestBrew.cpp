#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridMaskVH16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskVH16>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x9102u));
      },
      "SwapGridMaskVH16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridMaskVH16", aError);
}

