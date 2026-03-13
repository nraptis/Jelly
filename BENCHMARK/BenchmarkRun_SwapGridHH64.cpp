#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridHH64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_hh_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridHH64>();
      });
}

