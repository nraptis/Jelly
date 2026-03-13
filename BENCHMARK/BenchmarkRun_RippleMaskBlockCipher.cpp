#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Ripple/RippleMaskBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "ripple_mask_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::RippleMaskBlockCipher>(
                  pInnerBlockSize, 0xA3u, 4);
            });
      });
}
