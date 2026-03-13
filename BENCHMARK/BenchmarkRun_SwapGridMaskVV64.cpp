#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskVV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_vv_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskVV64>(0xA3u);
      });
}

