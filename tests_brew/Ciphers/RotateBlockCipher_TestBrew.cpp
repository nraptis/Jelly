#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Rotation/RotateBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "RotateBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::RotateBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseShift(
                                              pCaseIndex, 0x102u));
                                    },
                                    pBlockSize, "RotateBlockCipher", &pError);
                              });
}
