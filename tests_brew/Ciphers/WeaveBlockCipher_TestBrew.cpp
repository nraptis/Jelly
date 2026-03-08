#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Weave/WeaveBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "WeaveBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::WeaveBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseCount(
                                              pCaseIndex, 0x10du),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x10eu),
                                          jelly::test_brew::CaseStride(
                                              pCaseIndex, 0x10fu));
                                    },
                                    pBlockSize, "WeaveBlockCipher", &pError);
                              });
}
