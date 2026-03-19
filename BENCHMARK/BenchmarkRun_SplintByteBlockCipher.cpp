#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Splint/SplintByteBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "splint_byte_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::SplintByteBlockCipher>(
                  pInnerBlockSize);
            });
      });
}
