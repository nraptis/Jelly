#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Weave/WeaveCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "weave",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::WeaveCipher>(3, 1, 2);
      });
}
