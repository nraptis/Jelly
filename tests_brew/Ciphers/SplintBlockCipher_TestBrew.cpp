#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Splint/SplintBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "SplintBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::SplintBlockCipher>(
                                          pInnerBlockSize);
                                    },
                                    pBlockSize, "SplintBlockCipher", &pError);
                              });
}
