#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridMaskH64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_h_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskH64>(0xA3u, 2);
      });
}

