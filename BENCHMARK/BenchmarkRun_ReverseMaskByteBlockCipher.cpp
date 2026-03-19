#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Reverse/ReverseMaskByteBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "reverse_mask_byte_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, peanutbutter::CryptMode) {
              switch (pBlockSize) {
                case 8:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           8>>(
                      0xA3u));
                case 12:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           12>>(
                      0xA3u));
                case 16:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           16>>(
                      0xA3u));
                case 24:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           24>>(
                      0xA3u));
                case 32:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           32>>(
                      0xA3u));
                case 48:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskByteBlockCipher<
                                                           48>>(
                      0xA3u));
              }
              return std::unique_ptr<peanutbutter::Crypt>();
            });
      });
}
