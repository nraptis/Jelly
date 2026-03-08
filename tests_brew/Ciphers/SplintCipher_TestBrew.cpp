#include <string>

#include "../../src/Ciphers/Splint/SplintCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SplintCipher>();
      },
      "SplintCipher", &aError);
  return jelly::test_brew::ReportFlatResult("SplintCipher", aError);
}
