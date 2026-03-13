#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Reverse/ReverseCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "reverse", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::ReverseCipher>();
      });
}
