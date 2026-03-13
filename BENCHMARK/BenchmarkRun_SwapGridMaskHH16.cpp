#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/SwapGrid/SwapGridMaskHH16.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "swap_grid_mask_hh_16", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SwapGridMaskHH16>(0xA3u);
      });
}

