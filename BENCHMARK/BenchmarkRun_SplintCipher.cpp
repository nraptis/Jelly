#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Splint/SplintCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "splint",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::SplintCipher>();
      });
}
