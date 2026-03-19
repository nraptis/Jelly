#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SpiralGrid/SpiralGridV64.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "spiral_grid_v_64", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SpiralGridV64>(5);
      });
}
