#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../src/Encryption/Ciphers/Pepper/PepperJumpXORCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

std::vector<unsigned char> BuildMask(std::size_t pCaseIndex) {
  const std::size_t aLength = 16u * (4u + (pCaseIndex % 8u));
  std::vector<unsigned char> aMask(aLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, 0x8A1u);
  for (std::size_t aIndex = 0; aIndex < aLength; ++aIndex) {
    aBits = (aBits * 1103515245u) + 12345u;
    aMask[aIndex] = static_cast<unsigned char>((aBits >> 16) & 0xffu);
  }
  return aMask;
}

std::vector<unsigned char> BuildJumps(std::size_t pCaseIndex,
                                      std::size_t pMaskLength) {
  const std::size_t aChunkCount = pMaskLength / 16u;
  const std::size_t aJumpLength = 8u + (pCaseIndex % 8u);
  std::vector<unsigned char> aJumps(aJumpLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, 0x8A2u);
  for (std::size_t aIndex = 0; aIndex < aJumpLength; ++aIndex) {
    aBits = (aBits * 1664525u) + 1013904223u;
    aJumps[aIndex] = static_cast<unsigned char>((aBits >> 24) % aChunkCount);
  }
  return aJumps;
}

const std::vector<unsigned char>& GetMask(std::size_t pCaseIndex) {
  static std::unordered_map<std::size_t, std::vector<unsigned char>> sMasks;
  auto aIt = sMasks.find(pCaseIndex);
  if (aIt == sMasks.end()) {
    aIt = sMasks.emplace(pCaseIndex, BuildMask(pCaseIndex)).first;
  }
  return aIt->second;
}

const std::vector<unsigned char>& GetJumps(std::size_t pCaseIndex) {
  static std::unordered_map<std::size_t, std::vector<unsigned char>> sJumps;
  auto aIt = sJumps.find(pCaseIndex);
  if (aIt == sJumps.end()) {
    const std::vector<unsigned char>& aMask = GetMask(pCaseIndex);
    aIt = sJumps.emplace(pCaseIndex, BuildJumps(pCaseIndex, aMask.size())).first;
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
        const std::vector<unsigned char>& aJumps = GetJumps(pCaseIndex);
        return std::make_unique<peanutbutter::PepperJumpXORCipher>(
            aMask.data(), static_cast<int>(aMask.size()), aJumps.data(),
            static_cast<int>(aJumps.size()));
      },
      "PepperJumpXORCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("PepperJumpXORCipher",
                                                   aError);
}
