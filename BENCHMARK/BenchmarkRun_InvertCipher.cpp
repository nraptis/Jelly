#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Invert/InvertCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "invert", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::InvertCipher>();
      });
}
