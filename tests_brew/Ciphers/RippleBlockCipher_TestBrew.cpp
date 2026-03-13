#include <string>

#include "../../src/Ciphers/Ripple/RippleBlockCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "RippleBlockCipher", [](int pBlockSize, std::string& pError) {
        jelly::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, jelly::CryptMode) {
              return std::make_unique<jelly::RippleBlockCipher>(
                  pInnerBlockSize,
                  jelly::test_brew::CaseCount(pCaseIndex, 0x604u));
            },
            pBlockSize, "RippleBlockCipher", &pError);
      });
}
