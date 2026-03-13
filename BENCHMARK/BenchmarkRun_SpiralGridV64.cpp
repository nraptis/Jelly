#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_v_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridV64>(5);
      });
}
