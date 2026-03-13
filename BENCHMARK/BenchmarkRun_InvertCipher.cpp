#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Invert/InvertCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "invert", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::InvertCipher>();
      });
}
