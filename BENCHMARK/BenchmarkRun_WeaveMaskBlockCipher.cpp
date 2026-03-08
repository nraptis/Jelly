#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Weave/WeaveMaskBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "weave_mask_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::WeaveMaskBlockCipher>(
                  pInnerBlockSize, 0x3Eu, 3, 1, 2);
            });
      });
}
