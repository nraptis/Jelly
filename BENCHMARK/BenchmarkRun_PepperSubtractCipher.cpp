#include <unordered_map>
#include <vector>

#include "BenchmarkRunSupport.hpp"

#include "../src/Encryption/Ciphers/Pepper/PepperSubtractCipher.hpp"

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
      "pepper_subtract",
      [](std::size_t pLength, std::size_t, peanutbutter::CryptMode) {
        static std::unordered_map<std::size_t, std::vector<unsigned char>> sMasks;
        auto aIt = sMasks.find(pLength);
        if (aIt == sMasks.end()) {
          aIt = sMasks.emplace(pLength, BuildTable(pLength, 0x3C91F04Du, 0x5Cu)).first;
        }
        const std::vector<unsigned char>& aMask = aIt->second;
        return std::make_unique<peanutbutter::PepperSubtractCipher>(
            aMask.data(), aMask.size());
      });
}
