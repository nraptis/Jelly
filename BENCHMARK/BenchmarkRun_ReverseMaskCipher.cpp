#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Reverse/ReverseMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "reverse_mask", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::ReverseMaskCipher>(0xA3u);
      });
}
