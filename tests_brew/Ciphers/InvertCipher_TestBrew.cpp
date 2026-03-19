#include <string>

#include "../../src/Encryption/Ciphers/Invert/InvertCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::InvertCipher>();
      },
      "InvertCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("InvertCipher", aError);
}
