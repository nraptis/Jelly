#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Password/PasswordXORCipher.hpp"

namespace {

constexpr unsigned char kBenchmarkPassword[16] = {
    0x63u, 0x61u, 0x74u, 0x64u, 0x6fu, 0x67u, 0x43u, 0x41u,
    0x54u, 0x44u, 0x4fu, 0x47u, 0x31u, 0x32u, 0x33u, 0x34u,
};

}  // namespace

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "password_xor", [](std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::PasswordXORCipher>(kBenchmarkPassword,
                                                                 16);
      });
}
