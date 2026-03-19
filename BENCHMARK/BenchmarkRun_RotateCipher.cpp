#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Rotation/RotateCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "rotate",
      [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RotateCipher>(-16);
      });
}
