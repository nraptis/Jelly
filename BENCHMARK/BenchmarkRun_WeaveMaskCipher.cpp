#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Weave/WeaveMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "weave_mask",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::WeaveMaskCipher>(0x3Eu, 3, 1, 2);
      });
}
