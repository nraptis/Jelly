#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Password/PasswordCipher.hpp"

int main() {
  return jelly::benchmark::RunFlatBenchmark(
      "password", [](std::size_t, jelly::CryptMode) {
        return std::make_unique<jelly::PasswordCipher>("catdog");
      });
}
