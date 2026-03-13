#include <string>

#include "../../src/Ciphers/Ripple/RippleMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::RippleMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x602u),
            jelly::test_brew::CaseCount(pCaseIndex, 0x603u));
      },
      "RippleMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("RippleMaskCipher", aError);
}
