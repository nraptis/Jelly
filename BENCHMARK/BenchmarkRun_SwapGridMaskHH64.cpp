#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskHH64.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_hh_64", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskHH64>(0xA3u);
      });
}

