#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Splint/SplintMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "splint_mask",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SplintMaskCipher>(0xA3u);
      });
}
