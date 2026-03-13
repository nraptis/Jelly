#include <string>

#include "../../src/Ciphers/Reverse/ReverseMaskCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::ReverseMaskCipher>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x401u));
      },
      "ReverseMaskCipher", &aError);
  return jelly::test_brew::ReportFlatResult("ReverseMaskCipher", aError);
}
