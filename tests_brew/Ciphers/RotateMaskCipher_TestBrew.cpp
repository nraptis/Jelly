#include <string>

#include "../../src/Encryption/Ciphers/Rotation/RotateMaskCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RotateMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x103u),
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x104u));
      },
      "RotateMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("RotateMaskCipher", aError);
}
