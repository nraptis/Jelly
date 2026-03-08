#include <string>

#include "../../src/Jelly.hpp"
#include "../../src/Ciphers/Splint/SplintBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  return jelly::test_brew::RunForEveryBlockSize(
      "SplintBlockCipher", [](int pBlockSize, std::string& pError) {
                                jelly::ExecuteTestBrew_Block(
                                    [](int pInnerBlockSize,
                                       std::size_t,
                                       jelly::CryptMode) {
                                      return std::make_unique<jelly::SplintBlockCipher>(
                                          pInnerBlockSize);
                                    },
                                    pBlockSize, "SplintBlockCipher", &pError);
                              });
}
