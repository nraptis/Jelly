#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Weave/WeaveByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "WeaveByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::WeaveByteBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseCount(
                                              pCaseIndex, 0x110u),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x111u),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x112u));
                                    },
                                    pBlockSize, "WeaveByteBlockCipher",
                                    &pError);
                              });
}
