#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Weave/WeaveBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "weave_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::WeaveBlockCipher>(pInnerBlockSize,
                                                               3, 1, 2);
            });
      });
}
