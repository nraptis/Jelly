#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Splint/SplintByteBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return peanutbutter::test_brew::RunForEveryBlockSize(
      "SplintByteBlockCipher", [](int pBlockSize, std::string& pError) {
                                peanutbutter::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t,
                                       peanutbutter::CryptMode) {
                                      return std::make_unique<peanutbutter::SplintByteBlockCipher>(
                                          pInnerBlockSize);
                                    },
                                    pBlockSize, "SplintByteBlockCipher",
                                    &pError);
                              });
}
