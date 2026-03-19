#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Splint/SplintMaskBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "SplintMaskBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t pCaseIndex,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::SplintMaskBlockCipher>(
                                          pInnerBlockSize,
                                          peanutbutter::test_brew::CaseMask(
                                              pCaseIndex, 0x108u));
                                    },
                                    pBlockSize, "SplintMaskBlockCipher",
                                    &pError);
                              });
}
