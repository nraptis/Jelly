#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Pepper/PepperCipherXORNoise.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "pepper_xor_noise", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PepperCipherXORNoise>();
      });
}
