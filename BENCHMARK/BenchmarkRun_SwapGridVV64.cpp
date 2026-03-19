#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SwapGrid/SwapGridVV64.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "swap_grid_vv_64", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridVV64>();
      });
}

