#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Rotation/RotateMaskByteBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "rotation_mask_byte_block",
      [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, jelly::CryptMode) {
              switch (pBlockSize) {
                case jelly::EB_BLOCK_SIZE_08:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_08>>(
                      0xDEu, -16));
                case jelly::EB_BLOCK_SIZE_12:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_12>>(
                      0xDEu, -16));
                case jelly::EB_BLOCK_SIZE_16:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_16>>(
                      0xDEu, -16));
                case jelly::EB_BLOCK_SIZE_24:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_24>>(
                      0xDEu, -16));
                case jelly::EB_BLOCK_SIZE_32:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_32>>(
                      0xDEu, -16));
                case jelly::EB_BLOCK_SIZE_48:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::RotateMaskByteBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_48>>(
                      0xDEu, -16));
              }
              return std::unique_ptr<jelly::Crypt>();
            });
      });
}
