#include <string>

#include "../../src/Ciphers/Rotation/RotateMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::RotateMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x103u),
            jelly::test_brew::CaseShift(pCaseIndex, 0x104u));
      },
      "RotateMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("RotateMaskCipher", aError);
}
