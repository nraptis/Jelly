#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskHV16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_hv_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskHV16>(0xA3u);
      });
}

