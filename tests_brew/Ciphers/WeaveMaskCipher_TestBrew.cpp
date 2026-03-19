#include <string>

#include "../../src/Encryption/Ciphers/Weave/WeaveMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::WeaveMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x113u),
            peanutbutter::test_brew::CaseCount(pCaseIndex, 0x114u),
            peanutbutter::test_brew::CaseStride(pCaseIndex, 0x115u),
            peanutbutter::test_brew::CaseStride(pCaseIndex, 0x116u));
      },
      "WeaveMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("WeaveMaskCipher", aError);
}
