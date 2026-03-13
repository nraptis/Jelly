#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Ripple/RippleMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "ripple_mask", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::RippleMaskCipher>(0xA3u, 4);
      });
}
