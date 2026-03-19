#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SwapGrid/SwapGridMaskVV16.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "swap_grid_mask_vv_16", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskVV16>(0xA3u);
      });
}

