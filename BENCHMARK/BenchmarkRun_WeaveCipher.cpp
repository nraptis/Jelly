#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Weave/WeaveCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "weave",
      [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::WeaveCipher>(3, 1, 2);
      });
}
