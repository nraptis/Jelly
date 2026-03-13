#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridVV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_vv_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridVV64>();
      });
}

