#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridHH16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_hh_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridHH16>();
      });
}

