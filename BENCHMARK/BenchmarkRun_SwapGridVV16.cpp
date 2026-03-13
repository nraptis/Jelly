#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridVV16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_vv_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridVV16>();
      });
}

