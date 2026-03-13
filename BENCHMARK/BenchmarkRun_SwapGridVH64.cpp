#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridVH64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_vh_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridVH64>();
      });
}

