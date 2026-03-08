#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Weave/WeaveMaskByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "WeaveMaskByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::WeaveMaskByteBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseMask(
                                              pCaseIndex, 0x11bu),
                                          jelly::test_brew::CaseCount(
                                              pCaseIndex, 0x11cu),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x11du),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x11eu));
                                    },
                                    pBlockSize, "WeaveMaskByteBlockCipher",
                                    &pError);
                              });
}
