#include <string>

#include "../../src/Encryption/Ciphers/Splint/SplintMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SplintMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x107u));
      },
      "SplintMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SplintMaskCipher", aError);
}
