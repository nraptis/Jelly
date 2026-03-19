#include <string>

#include "../../src/Encryption/Ciphers/Ripple/RippleMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RippleMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x602u),
            peanutbutter::test_brew::CaseCount(pCaseIndex, 0x603u));
      },
      "RippleMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("RippleMaskCipher", aError);
}
