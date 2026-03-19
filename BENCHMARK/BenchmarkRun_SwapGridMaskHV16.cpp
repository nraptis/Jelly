#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SwapGrid/SwapGridMaskHV16.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "swap_grid_mask_hv_16", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridMaskHV16>(0xA3u);
      });
}

