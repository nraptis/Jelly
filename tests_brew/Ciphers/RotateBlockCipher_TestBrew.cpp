#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Rotation/RotateBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "RotateBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::RotateBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseShift(
                                              pCaseIndex, 0x102u));
                                    },
                                    pBlockSize, "RotateBlockCipher", &pError);
                              });
}
