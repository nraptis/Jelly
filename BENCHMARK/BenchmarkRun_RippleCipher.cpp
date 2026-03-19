#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Ripple/RippleCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "ripple", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RippleCipher>(4);
      });
}
