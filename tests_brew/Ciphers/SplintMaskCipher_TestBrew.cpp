#include <string>

#include "../../src/Ciphers/Splint/SplintMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::SplintMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x107u));
      },
      "SplintMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("SplintMaskCipher", aError);
}
