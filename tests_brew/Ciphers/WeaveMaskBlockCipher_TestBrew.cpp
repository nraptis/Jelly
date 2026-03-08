#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Weave/WeaveMaskBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "WeaveMaskBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::WeaveMaskBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseMask(
                                              pCaseIndex, 0x117u),
                                          jelly::test_brew::CaseCount(
                                              pCaseIndex, 0x118u),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x119u),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x11au));
                                    },
                                    pBlockSize, "WeaveMaskBlockCipher",
                                    &pError);
                              });
}
