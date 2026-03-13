#include <string>

#include "../../src/Ciphers/SwapGrid/SwapGridVV16.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridVV16>();
      },
      "SwapGridVV16", &aError);
  return jelly::test_brew::ReportFlatResult("SwapGridVV16", aError);
}

