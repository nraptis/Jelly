#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskVH16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_vh_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskVH16>(0xA3u);
      });
}

