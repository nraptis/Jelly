#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Splint/SplintMaskByteBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "splint_mask_byte_block",
      [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::SplintMaskByteBlockCipher>(
                  pInnerBlockSize, 0xA3u);
            });
      });
}
