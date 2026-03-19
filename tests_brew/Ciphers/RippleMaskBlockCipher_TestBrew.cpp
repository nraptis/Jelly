#include <string>

#include "../../src/Encryption/Ciphers/Ripple/RippleMaskBlockCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "RippleMaskBlockCipher", [](int pBlockSize, std::string& pError) {
        peanutbutter::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RippleMaskBlockCipher>(
                  pInnerBlockSize,
                  peanutbutter::test_brew::CaseMask(pCaseIndex, 0x605u),
                  peanutbutter::test_brew::CaseCount(pCaseIndex, 0x606u));
            },
            pBlockSize, "RippleMaskBlockCipher", &pError);
      });
}
