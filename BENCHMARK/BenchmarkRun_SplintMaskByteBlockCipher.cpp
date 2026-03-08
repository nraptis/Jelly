#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Splint/SplintMaskByteBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "splint_mask_byte_block",
      [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::SplintMaskByteBlockCipher>(
                  pInnerBlockSize, 0xA3u);
            });
      });
}
