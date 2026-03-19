#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Rotation/RotateMaskCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "rotate_mask",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RotateMaskCipher>(0xDEu, -16);
      });
}
