#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Invert/InvertMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "invert_mask", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::InvertMaskCipher>(0xA3u);
      });
}
