#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../src/Encryption/Ciphers/Pepper/PepperSubtractCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

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

const std::vector<unsigned char>& GetMask(std::size_t pLength) {
  static std::unordered_map<std::size_t, std::vector<unsigned char>> sMasks;
  auto aIt = sMasks.find(pLength);
  if (aIt == sMasks.end()) {
    aIt = sMasks.emplace(pLength, BuildTable(pLength, 0x3C91F04Du, 0x5Cu)).first;
  }
  return aIt->second;
}

}  // namespace

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_FlatSized(
      [](std::size_t pLength, std::size_t, peanutbutter::CryptMode)
          -> std::unique_ptr<peanutbutter::Crypt> {
        const std::vector<unsigned char>& aMask = GetMask(pLength);
        return std::make_unique<peanutbutter::PepperSubtractCipher>(
            aMask.data(), aMask.size());
      },
      "PepperSubtractCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("PepperSubtractCipher",
                                                   aError);
}
