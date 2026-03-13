#include <string>

#include "../../src/Ciphers/Invert/InvertMaskCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::InvertMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x1A7E27u));
      },
      "InvertMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("InvertMaskCipher", aError);
}
