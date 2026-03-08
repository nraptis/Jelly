#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Rotation/RotateMaskCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "rotate_mask",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::RotateMaskCipher>(0xDEu, -16);
      });
}
