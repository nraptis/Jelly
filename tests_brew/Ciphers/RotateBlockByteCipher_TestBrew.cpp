#include <string>

#include "../../src/Ciphers/Rotation/RotateBlockByteCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "RotateBlockByteCipher", [](int pBlockSize, std::string& pError) {
        jelly::ExecuteTestBrew_Block(
            [](int pInnerBlockSize, std::size_t pCaseIndex, jelly::CryptMode) {
              return std::make_unique<jelly::RotateBlockByteCipher>(
                  pInnerBlockSize,
                  jelly::test_brew::CaseShift(pCaseIndex, 0x502u));
            },
            pBlockSize, "RotateBlockByteCipher", &pError);
      });
}
