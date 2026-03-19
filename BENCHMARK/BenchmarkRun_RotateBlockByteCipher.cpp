#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Rotation/RotateBlockByteCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "rotation_block_byte", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [](int pInnerBlockSize, std::size_t, peanutbutter::CryptMode) {
              return std::make_unique<peanutbutter::RotateBlockByteCipher>(
                  pInnerBlockSize, -16);
            });
      });
}
