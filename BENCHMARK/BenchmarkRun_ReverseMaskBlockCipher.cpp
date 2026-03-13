#include "BenchmarkRunSupport.hpp"

#include "../src/Ciphers/Reverse/ReverseMaskBlockCipher.hpp"

int main() {
  return jelly::benchmark::RunForEveryBlockSize(
      "reverse_mask_block", [](int pBlockSize, const std::string& pStem) {
        return jelly::benchmark::RunBlockBenchmark(
            pStem, pBlockSize,
            [pBlockSize](int, std::size_t, jelly::CryptMode) {
              switch (pBlockSize) {
                case jelly::EB_BLOCK_SIZE_08:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_08>>(
                      0xA3u));
                case jelly::EB_BLOCK_SIZE_12:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_12>>(
                      0xA3u));
                case jelly::EB_BLOCK_SIZE_16:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_16>>(
                      0xA3u));
                case jelly::EB_BLOCK_SIZE_24:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_24>>(
                      0xA3u));
                case jelly::EB_BLOCK_SIZE_32:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_32>>(
                      0xA3u));
                case jelly::EB_BLOCK_SIZE_48:
                  return std::unique_ptr<jelly::Crypt>(std::make_unique<
                                                       jelly::ReverseMaskBlockCipher<
                                                           jelly::EB_BLOCK_SIZE_48>>(
                      0xA3u));
              }
              return std::unique_ptr<jelly::Crypt>();
            });
      });
}
