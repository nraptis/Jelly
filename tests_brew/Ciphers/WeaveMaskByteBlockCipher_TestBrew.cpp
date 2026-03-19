#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Weave/WeaveMaskByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "WeaveMaskByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::WeaveMaskByteBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseMask(
                                              pCaseIndex, 0x11bu),
                                          peanutbutter::test_brew::CaseCount(
                                              pCaseIndex, 0x11cu),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x11du),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x11eu));
                                    },
                                    pBlockSize, "WeaveMaskByteBlockCipher",
                                    &pError);
                              });
}
