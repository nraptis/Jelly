#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskH16.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "spiral_grid_mask_h_16", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridMaskH16>(0xA3u, 2);
      });
}

