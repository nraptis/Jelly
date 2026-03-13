#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridHV64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_hv_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridHV64>();
      });
}

