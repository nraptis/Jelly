#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskHV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_hv_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskHV64>(0xA3u);
      });
}

