#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Ripple/RippleCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "ripple", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::RippleCipher>(4);
      });
}
