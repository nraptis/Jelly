#include <string>

#include "../../src/Ciphers/SwapGrid/SwapGridVH64.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridVH64>();
      },
      "SwapGridVH64", &aError);
  return jelly::test_brew::ReportFlatResult("SwapGridVH64", aError);
}

