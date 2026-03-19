#include <string>

#include "../../src/Encryption/Ciphers/SwapGrid/SwapGridVV16.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridVV16>();
      },
      "SwapGridVV16", &aError);
  return peanutbutter::test_brew::ReportFlatResult("SwapGridVV16", aError);
}

