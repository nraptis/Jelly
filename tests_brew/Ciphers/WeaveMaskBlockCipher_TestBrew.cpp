#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Weave/WeaveMaskBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "WeaveMaskBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::WeaveMaskBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseMask(
                                              pCaseIndex, 0x117u),
                                          peanutbutter::test_brew::CaseCount(
                                              pCaseIndex, 0x118u),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x119u),
                                          peanutbutter::test_brew::CaseStride(
                                              pCaseIndex, 0x11au));
                                    },
                                    pBlockSize, "WeaveMaskBlockCipher",
                                    &pError);
                              });
}
