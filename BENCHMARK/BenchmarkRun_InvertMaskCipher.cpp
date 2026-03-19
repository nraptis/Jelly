#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Invert/InvertMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "invert_mask", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::InvertMaskCipher>(0xA3u);
      });
}
