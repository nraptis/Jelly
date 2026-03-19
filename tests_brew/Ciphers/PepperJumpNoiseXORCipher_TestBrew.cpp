#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../src/Encryption/Ciphers/Pepper/PepperJumpNoiseXORCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

std::vector<unsigned char> BuildTable(std::size_t pCaseIndex,
                                      std::uint32_t pSalt) {
  const std::size_t aLength = 16u * (4u + (pCaseIndex % 8u));
  std::vector<unsigned char> aTable(aLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, pSalt);
  for (std::size_t aIndex = 0; aIndex < aLength; ++aIndex) {
    aBits = (aBits * 1103515245u) + 12345u;
    aTable[aIndex] = static_cast<unsigned char>((aBits >> 16) & 0xffu);
  }
  return aTable;
}

std::vector<unsigned char> BuildJumps(std::size_t pCaseIndex,
                                      std::size_t pMaskLength) {
  const std::size_t aChunkCount = pMaskLength / 16u;
  const std::size_t aJumpLength = 8u + (pCaseIndex % 8u);
  std::vector<unsigned char> aJumps(aJumpLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, 0x8B3u);
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
    aIt = sMasks.emplace(pCaseIndex, BuildTable(pCaseIndex, 0x8B1u)).first;
  }
  return aIt->second;
}

const std::vector<unsigned char>& GetNoise(std::size_t pCaseIndex) {
  static std::unordered_map<std::size_t, std::vector<unsigned char>> sNoise;
  auto aIt = sNoise.find(pCaseIndex);
  if (aIt == sNoise.end()) {
    aIt = sNoise.emplace(pCaseIndex, BuildTable(pCaseIndex, 0x8B2u)).first;
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
        const std::vector<unsigned char>& aNoise = GetNoise(pCaseIndex);
        const std::vector<unsigned char>& aJumps = GetJumps(pCaseIndex);
        return std::make_unique<peanutbutter::PepperJumpNoiseXORCipher>(
            aMask.data(), static_cast<int>(aMask.size()), aNoise.data(),
            static_cast<int>(aNoise.size()), aJumps.data(),
            static_cast<int>(aJumps.size()));
      },
      "PepperJumpNoiseXORCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("PepperJumpNoiseXORCipher",
                                                   aError);
}
