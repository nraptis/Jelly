#include <string>

#include "../../src/Encryption/Ciphers/Rotation/RotateBlockByteCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "RotateBlockByteCipher", [](int pBlockSize, std::string& pError) {
        peanutbutter::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RotateBlockByteCipher>(
                  pInnerBlockSize,
                  peanutbutter::test_brew::CaseShift(pCaseIndex, 0x502u));
            },
            pBlockSize, "RotateBlockByteCipher", &pError);
      });
}
