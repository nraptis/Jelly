#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Reverse/ReverseBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "reverse_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, jelly::CryptMode) {
              switch (pBlockSize) {
                case jelly::EB_BLOCK_SIZE_08:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_08>>());
                case jelly::EB_BLOCK_SIZE_12:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_12>>());
                case jelly::EB_BLOCK_SIZE_16:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_16>>());
                case jelly::EB_BLOCK_SIZE_24:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_24>>());
                case jelly::EB_BLOCK_SIZE_32:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_32>>());
                case jelly::EB_BLOCK_SIZE_48:
                  return std::unique_ptr<jelly::Crypt>(
                      std::make_unique<
                          jelly::ReverseBlockCipher<jelly::EB_BLOCK_SIZE_48>>());
              }
              return std::unique_ptr<jelly::Crypt>();
            });
      });
}
