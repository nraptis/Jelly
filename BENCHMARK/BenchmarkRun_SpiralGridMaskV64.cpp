#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridMaskV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_v_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskV64>(0xA3u, 2);
      });
}

