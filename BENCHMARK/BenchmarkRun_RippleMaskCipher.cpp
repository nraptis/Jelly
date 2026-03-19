#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Ripple/RippleMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "ripple_mask", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RippleMaskCipher>(0xA3u, 4);
      });
}
