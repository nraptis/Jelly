#include <string>

#include "../../src/Ciphers/Ripple/RippleCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::RippleCipher>(
            jelly::test_brew::CaseCount(pCaseIndex, 0x601u));
      },
      "RippleCipher", &aError);
  return jelly::test_brew::ReportFlatResult("RippleCipher", aError);
}
