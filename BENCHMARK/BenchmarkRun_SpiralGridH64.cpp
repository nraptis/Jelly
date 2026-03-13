#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridH64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_h_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridH64>(5);
      });
}

