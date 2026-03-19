#include <string>

#include "../../src/Encryption/Ciphers/Ripple/RippleBlockCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "RippleBlockCipher", [](int pBlockSize, std::string& pError) {
        peanutbutter::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RippleBlockCipher>(
                  pInnerBlockSize,
                  peanutbutter::test_brew::CaseCount(pCaseIndex, 0x604u));
            },
            pBlockSize, "RippleBlockCipher", &pError);
      });
}
