#include <string>

#include "../../src/Ciphers/SwapGrid/SwapGridHV64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridHV64>();
      },
      "SwapGridHV64", &aError);
  return jelly::test_brew::ReportFlatResult("SwapGridHV64", aError);
}

