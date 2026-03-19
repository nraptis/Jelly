#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridHV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridHV16>();
      },
      "SwapGridHV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridHV16", aError);
}

