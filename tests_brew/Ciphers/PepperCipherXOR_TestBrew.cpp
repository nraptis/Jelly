#include <string>

#include "../../src/Ciphers/Pepper/PepperCipherXOR.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PepperCipherXOR>();
      },
      "PepperCipherXOR", &aError);
  return jelly::test_brew::ReportFlatResult("PepperCipherXOR", aError);
}
