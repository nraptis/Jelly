#include <string>

#include "../../src/Encryption/Ciphers/Weave/WeaveCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::WeaveCipher>(
            peanutbutter::test_brew::CaseCount(pCaseIndex, 0x10au),
            peanutbutter::test_brew::CaseStride(pCaseIndex, 0x10bu),
            peanutbutter::test_brew::CaseStride(pCaseIndex, 0x10cu));
      },
      "WeaveCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("WeaveCipher", aError);
}
