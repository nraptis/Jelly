#include <string>

#include "../../src/Encryption/Ciphers/Ripple/RippleCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RippleCipher>(
            peanutbutter::test_brew::CaseCount(pCaseIndex, 0x601u));
      },
      "RippleCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("RippleCipher", aError);
}
