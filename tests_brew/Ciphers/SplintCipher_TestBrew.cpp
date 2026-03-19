#include <string>

#include "../../src/Encryption/Ciphers/Splint/SplintCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SplintCipher>();
      },
      "SplintCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SplintCipher", aError);
}
