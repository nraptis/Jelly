#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Weave/WeaveMaskBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "weave_mask_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::WeaveMaskBlockCipher>(
                  pInnerBlockSize, 0x3Eu, 3, 1, 2);
            });
      });
}
