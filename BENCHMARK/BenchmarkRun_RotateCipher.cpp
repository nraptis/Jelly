#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Rotation/RotateCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "rotate",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::RotateCipher>(-16);
      });
}
