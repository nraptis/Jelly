#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskV16.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_v_16", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridMaskV16>(0xA3u, 2);
      });
}

