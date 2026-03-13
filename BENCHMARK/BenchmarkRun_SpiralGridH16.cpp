#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SpiralGrid/SpiralGridH16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "spiral_grid_h_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SpiralGridH16>(2);
      });
}

