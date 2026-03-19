#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Reverse/ReverseCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "reverse", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::ReverseCipher>();
      });
}
