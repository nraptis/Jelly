#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskH64.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_h_64", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridMaskH64>(0xA3u, 2);
      });
}

