#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/SwapGrid/SwapGridVH64.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "swap_grid_vh_64", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SwapGridVH64>();
      });
}

