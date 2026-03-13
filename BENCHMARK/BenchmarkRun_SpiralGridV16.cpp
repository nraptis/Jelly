#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridV16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_v_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridV16>(2);
      });
}

