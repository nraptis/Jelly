#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Ripple/RippleMaskBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "ripple_mask_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RippleMaskBlockCipher>(
                  pInnerBlockSize, 0xA3u, 4);
            });
      });
}
