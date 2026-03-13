#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridMaskV16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_v_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskV16>(0xA3u, 2);
      });
}

