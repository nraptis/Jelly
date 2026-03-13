#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridHV16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_hv_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridHV16>();
      });
}

