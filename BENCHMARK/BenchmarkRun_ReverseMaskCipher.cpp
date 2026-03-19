#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Reverse/ReverseMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "reverse_mask", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::ReverseMaskCipher>(0xA3u);
      });
}
