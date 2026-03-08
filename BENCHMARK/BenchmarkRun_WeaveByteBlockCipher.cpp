#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Weave/WeaveByteBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "weave_byte_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::WeaveByteBlockCipher>(
                  pInnerBlockSize, 3, 1, 2);
            });
      });
}
