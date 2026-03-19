#include <string>

#include "../../src/Encryption/Ciphers/Rotation/RotateCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RotateCipher>(
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x101u));
      },
      "RotateCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("RotateCipher", aError);
}
