#include <string>

#include "../../src/Ciphers/Weave/WeaveMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::WeaveMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x113u),
            jelly::test_brew::CaseCount(pCaseIndex, 0x114u),
            jelly::test_brew::CaseStride(pCaseIndex, 0x115u),
            jelly::test_brew::CaseStride(pCaseIndex, 0x116u));
      },
      "WeaveMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("WeaveMaskCipher", aError);
}
