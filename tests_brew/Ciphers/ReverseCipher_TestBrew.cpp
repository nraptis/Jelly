#include <string>

#include "../../src/Ciphers/Reverse/ReverseCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::ReverseCipher>();
      },
      "ReverseCipher", &aError);
  return jelly::test_brew::ReportFlatResult("ReverseCipher", aError);
}
