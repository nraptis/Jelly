#include <string>

#include "../../src/Encryption/Ciphers/Reverse/ReverseMaskCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::ReverseMaskCipher>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x401u));
      },
      "ReverseMaskCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("ReverseMaskCipher", aError);
}
