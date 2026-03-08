#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Splint/SplintMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "splint_mask",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SplintMaskCipher>(0xA3u);
      });
}
