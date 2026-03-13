#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Ripple/RippleBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "ripple_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::RippleBlockCipher>(pInnerBlockSize,
                                                                4);
            });
      });
}
