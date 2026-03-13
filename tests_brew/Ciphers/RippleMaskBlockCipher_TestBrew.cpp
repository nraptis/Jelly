#include <string>

#include "../../src/Ciphers/Ripple/RippleMaskBlockCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "RippleMaskBlockCipher", [](int pBlockSize, std::string& pError) {
        jelly::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, jelly::CryptMode) {
              return std::make_unique<jelly::RippleMaskBlockCipher>(
                  pInnerBlockSize,
                  jelly::test_brew::CaseMask(pCaseIndex, 0x605u),
                  jelly::test_brew::CaseCount(pCaseIndex, 0x606u));
            },
            pBlockSize, "RippleMaskBlockCipher", &pError);
      });
}
