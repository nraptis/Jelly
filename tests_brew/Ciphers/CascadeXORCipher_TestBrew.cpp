#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../src/Encryption/Ciphers/Cascade/CascadeXORCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

std::vector<unsigned char> BuildMask(std::size_t pCaseIndex) {
  const std::size_t aLength = 16u * (4u + (pCaseIndex % 8u));
  std::vector<unsigned char> aMask(aLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, 0x8C1u);
  for (std::size_t aIndex = 0; aIndex < aLength; ++aIndex) {
    aBits = (aBits * 1103515245u) + 12345u;
    aMask[aIndex] = static_cast<unsigned char>((aBits >> 16) & 0xffu);
  }
  return aMask;
}

const std::vector<unsigned char>& GetMask(std::size_t pCaseIndex) {
  static std::unordered_map<std::size_t, std::vector<unsigned char>> sMasks;
  auto aIt = sMasks.find(pCaseIndex);
  if (aIt == sMasks.end()) {
    aIt = sMasks.emplace(pCaseIndex, BuildMask(pCaseIndex)).first;
  }
  return aIt->second;
}

}  // namespace

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode)
          -> std::unique_ptr<peanutbutter::Crypt> {
        const std::vector<unsigned char>& aMask = GetMask(pCaseIndex);
        return std::make_unique<peanutbutter::CascadeXORCipher>(
            aMask.data(), static_cast<int>(aMask.size()));
      },
      "CascadeXORCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("CascadeXORCipher", aError);
}
