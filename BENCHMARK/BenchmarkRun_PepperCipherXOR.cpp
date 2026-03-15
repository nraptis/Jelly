#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Pepper/PepperCipherXOR.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "pepper_xor", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PepperCipherXOR>();
      });
}
