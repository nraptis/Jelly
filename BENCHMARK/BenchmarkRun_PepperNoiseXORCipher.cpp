#include <unordered_map>
#include <vector>

#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Pepper/PepperNoiseXORCipher.hpp"

namespace {

std::vector<unsigned char> BuildTable(std::size_t pLength,
                                      std::uint32_t pSeed,
                                      unsigned char pZeroFallback) {
  std::vector<unsigned char> aTable(pLength);
  std::uint32_t aState = pSeed;
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    aState ^= (aState << 13);
    aState ^= (aState >> 17);
    aState ^= (aState << 5);
    const unsigned char aValue = static_cast<unsigned char>(aState & 0xFFu);
    aTable[aIndex] = (aValue == 0u) ? pZeroFallback : aValue;
  }
  return aTable;
}

}  // namespace

int main() {
  return peanutbutter::benchmark::RunFlatBenchmark(
      "pepper_noise_xor",
      [](std::size_t pLength, std::size_t, peanutbutter::CryptMode) {
        static std::unordered_map<std::size_t, std::vector<unsigned char>> sMasks;
        static std::unordered_map<std::size_t, std::vector<unsigned char>> sNoise;
        auto aMaskIt = sMasks.find(pLength);
        if (aMaskIt == sMasks.end()) {
          aMaskIt =
              sMasks.emplace(pLength, BuildTable(pLength, 0xA5B35713u, 0xA3u)).first;
        }
        auto aNoiseIt = sNoise.find(pLength);
        if (aNoiseIt == sNoise.end()) {
          aNoiseIt =
              sNoise.emplace(pLength, BuildTable(pLength, 0xC8E9124Du, 0x5Cu)).first;
        }
        const std::vector<unsigned char>& aMask = aMaskIt->second;
        const std::vector<unsigned char>& aNoise = aNoiseIt->second;
        return std::make_unique<peanutbutter::PepperNoiseXORCipher>(
            aMask.data(), aMask.size(), aNoise.data(), aNoise.size());
      });
}
