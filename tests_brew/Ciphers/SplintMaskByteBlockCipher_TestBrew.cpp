#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Splint/SplintMaskByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "SplintMaskByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::SplintMaskByteBlockCipher>(
                                          pInnerBlockSize,
                                          jelly::test_brew::CaseMask(
                                              pCaseIndex, 0x109u));
                                    },
                                    pBlockSize, "SplintMaskByteBlockCipher",
                                    &pError);
                              });
}
