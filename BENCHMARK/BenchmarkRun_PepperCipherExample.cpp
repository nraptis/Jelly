#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Pepper/PepperCipherExample.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "pepper_example",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PepperCipherExample>();
      });
}
