#include <string>

#include "../../src/Encryption/Ciphers/Reverse/ReverseCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::ReverseCipher>();
      },
      "ReverseCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("ReverseCipher", aError);
}
