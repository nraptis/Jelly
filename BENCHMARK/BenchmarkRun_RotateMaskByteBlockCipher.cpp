#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Rotation/RotateMaskByteBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "rotation_mask_byte_block",
      [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, peanutbutter::CryptMode) {
              switch (pBlockSize) {
                case 8:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           8>>(
                      0xDEu, -16));
                case 12:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           12>>(
                      0xDEu, -16));
                case 16:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           16>>(
                      0xDEu, -16));
                case 24:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           24>>(
                      0xDEu, -16));
                case 32:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           32>>(
                      0xDEu, -16));
                case 48:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::RotateMaskByteBlockCipher<
                                                           48>>(
                      0xDEu, -16));
              }
              return std::unique_ptr<peanutbutter::Crypt>();
            });
      });
}
