#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SpiralGrid/SpiralGridV16.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "spiral_grid_v_16", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridV16>(2);
      });
}

