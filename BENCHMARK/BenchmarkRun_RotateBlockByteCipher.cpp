#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Rotation/RotateBlockByteCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "rotation_block_byte", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, jelly::CryptMode) {
              return std::make_unique<jelly::RotateBlockByteCipher>(
                  pInnerBlockSize, -16);
            });
      });
}
