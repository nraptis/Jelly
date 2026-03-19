#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SwapGrid/SwapGridHV64.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "swap_grid_hv_64", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridHV64>();
      });
}

