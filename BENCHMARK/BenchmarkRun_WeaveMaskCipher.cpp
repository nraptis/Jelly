#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Weave/WeaveMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "weave_mask",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::WeaveMaskCipher>(0x3Eu, 3, 1, 2);
      });
}
