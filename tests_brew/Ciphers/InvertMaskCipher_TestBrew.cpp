#include <string>

#include "../../src/Encryption/Ciphers/Invert/InvertMaskCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::InvertMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x1A7E27u));
      },
      "InvertMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("InvertMaskCipher", aError);
}
