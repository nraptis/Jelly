#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Weave/WeaveBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "weave_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::WeaveBlockCipher>(pInnerBlockSize,
                                                               3, 1, 2);
            });
      });
}
