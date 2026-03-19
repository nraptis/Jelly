#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Reverse/ReverseBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "reverse_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, peanutbutter::CryptMode) {
              switch (pBlockSize) {
                case 8:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<8>>());
                case 12:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<12>>());
                case 16:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<16>>());
                case 24:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<24>>());
                case 32:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<32>>());
                case 48:
                  return std::unique_ptr<peanutbutter::Crypt>(
                      std::make_unique<
                          peanutbutter::ReverseBlockCipher<48>>());
              }
              return std::unique_ptr<peanutbutter::Crypt>();
            });
      });
}
