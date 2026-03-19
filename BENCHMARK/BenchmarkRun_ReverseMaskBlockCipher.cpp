#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Reverse/ReverseMaskBlockCipher.hpp"

int main() {
  return peanutbutter::benchmark::RunForEveryBlockSize(
      "reverse_mask_block", [](int pBlockSize, const std::string& pStem) {
        return peanutbutter::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, peanutbutter::CryptMode) {
              switch (pBlockSize) {
                case 8:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           8>>(
                      0xA3u));
                case 12:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           12>>(
                      0xA3u));
                case 16:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           16>>(
                      0xA3u));
                case 24:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           24>>(
                      0xA3u));
                case 32:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           32>>(
                      0xA3u));
                case 48:
                  return std::unique_ptr<peanutbutter::Crypt>(std::make_unique<
                                                       peanutbutter::ReverseMaskBlockCipher<
                                                           48>>(
                      0xA3u));
              }
              return std::unique_ptr<peanutbutter::Crypt>();
            });
      });
}
