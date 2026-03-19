#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Ripple/RippleBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "ripple_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RippleBlockCipher>(pInnerBlockSize,
                                                                4);
            });
      });
}
