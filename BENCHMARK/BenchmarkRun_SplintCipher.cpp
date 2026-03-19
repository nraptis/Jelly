#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Splint/SplintCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "splint",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::SplintCipher>();
      });
}
