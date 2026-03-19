#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Weave/WeaveBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "WeaveBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::WeaveBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseCount(
                                              pCaseIndex, 0x10du),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x10eu),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x10fu));
                                    },
                                    pBlockSize, "WeaveBlockCipher", &pError);
                              });
}
