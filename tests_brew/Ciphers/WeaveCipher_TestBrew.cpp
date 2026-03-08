#include <string>

#include "../../src/Ciphers/Weave/WeaveCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::WeaveCipher>(
            jelly::test_brew::CaseCount(pCaseIndex, 0x10au),
            jelly::test_brew::CaseStride(pCaseIndex, 0x10bu),
            jelly::test_brew::CaseStride(pCaseIndex, 0x10cu));
      },
      "WeaveCipher", &aError);
  return jelly::test_brew::ReportFlatResult("WeaveCipher", aError);
}
