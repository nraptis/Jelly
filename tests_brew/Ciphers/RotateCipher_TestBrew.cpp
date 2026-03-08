#include <string>

#include "../../src/Ciphers/Rotation/RotateCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::RotateCipher>(
            jelly::test_brew::CaseShift(pCaseIndex, 0x101u));
      },
      "RotateCipher", &aError);
  return jelly::test_brew::ReportFlatResult("RotateCipher", aError);
}
