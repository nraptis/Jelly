#include <string>

#include "../../src/Ciphers/Pepper/PepperCipherXORNoise.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PepperCipherXORNoise>();
      },
      "PepperCipherXORNoise", &aError);
  return jelly::test_brew::ReportFlatResult("PepperCipherXORNoise", aError);
}
