#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Splint/SplintMaskBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "SplintMaskBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::SplintMaskBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseMask(
                                              pCaseIndex, 0x108u));
                                    },
                                    pBlockSize, "SplintMaskBlockCipher",
                                    &pError);
                              });
}
