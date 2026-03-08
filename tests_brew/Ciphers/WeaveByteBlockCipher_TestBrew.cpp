#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Weave/WeaveByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "WeaveByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::WeaveByteBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseCount(
                                              pCaseIndex, 0x110u),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x111u),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x112u));
                                    },
                                    pBlockSize, "WeaveByteBlockCipher",
                                    &pError);
                              });
}
