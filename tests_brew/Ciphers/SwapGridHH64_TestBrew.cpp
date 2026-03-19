#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridHH64.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridHH64>();
      },
      "SwapGridHH64", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridHH64", aError);
}

