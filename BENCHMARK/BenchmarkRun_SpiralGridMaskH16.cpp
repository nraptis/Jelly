#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridMaskH16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_h_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridMaskH16>(0xA3u, 2);
      });
}

