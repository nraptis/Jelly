#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "../src/Core/MaskChunkCursor.hpp"
#include "../src/Core/MaskNoiseApplyEngine.hpp"
#include "../src/Core/NoiseChunkCursor.hpp"
#include "../src/Core/SampleMixer.hpp"
#include "../src/Jelly.hpp"

namespace {

constexpr std::size_t kLength = jelly::SB_CIPHER_LENGTH_GRANULARITY * 2;
constexpr std::size_t kMaskLength = jelly::SB_CIPHER_LENGTH_GRANULARITY;
constexpr std::size_t kNoiseLength = jelly::SB_CIPHER_LENGTH_GRANULARITY * 3;

template <std::size_t tLength>
void FillData(std::array<unsigned char, tLength>& pData, std::uint32_t pSeed) {
  std::uint32_t aState = pSeed;
  for (std::size_t aIndex = 0; aIndex < pData.size(); ++aIndex) {
    aState ^= (aState << 13);
    aState ^= (aState >> 17);
    aState ^= (aState << 5);
    pData[aIndex] = static_cast<unsigned char>(aState & 0xFFu);
  }
}

template <std::size_t tLength>
bool Equal(const std::array<unsigned char, tLength>& pA,
           const std::array<unsigned char, tLength>& pB) {
  for (std::size_t aIndex = 0; aIndex < tLength; ++aIndex) {
    if (pA[aIndex] != pB[aIndex]) {
      return false;
    }
  }
  return true;
}

}  // namespace

int main() {
  std::array<unsigned char, kLength> aSource{};
  std::array<unsigned char, kLength> aDestination{};
  std::array<unsigned char, kLength> aRoundTrip{};
  std::array<unsigned char, kLength> aWorker{};
  std::array<unsigned char, kMaskLength> aMaskTable{};
  std::array<unsigned char, kNoiseLength> aNoiseTable{};
  FillData(aSource, 0x12345678u);
  FillData(aNoiseTable, 0xBADC0FFEu);
  for (std::size_t aIndex = 0; aIndex < aMaskTable.size(); ++aIndex) {
    aMaskTable[aIndex] = static_cast<unsigned char>((aIndex * 29u) & 0xFFu);
  }

  std::array<unsigned char, kLength> aMaskSequence{};
  jelly::MaskChunkCursor aMaskCursor(aMaskTable.data(), aMaskTable.size());
  aMaskCursor.FillNext(aMaskSequence.data(), aMaskSequence.size());

  std::string aError;
  jelly::SampleMixerNoise aMixer1;
  if (!jelly::MaskNoiseApplyEngine::ApplyMaskAndNoiseChunked(
          aSource.data(), aWorker.data(), aDestination.data(), aSource.size(),
          aMaskTable.data(), aMaskTable.size(), aNoiseTable.data(),
          aNoiseTable.size(), &aMixer1, &aError, jelly::CryptMode::kNormal)) {
    std::cerr << "ApplyMaskAndNoiseChunked failed: " << aError << "\n";
    return 1;
  }

  for (std::size_t aIndex = 0; aIndex < aSource.size(); ++aIndex) {
    const unsigned char aAntiMask = static_cast<unsigned char>(~aMaskSequence[aIndex]);
    if ((aSource[aIndex] & aAntiMask) != (aDestination[aIndex] & aAntiMask)) {
      std::cerr << "anti-mask mismatch at index " << aIndex << "\n";
      return 1;
    }
  }

  jelly::SampleMixerNoise aMixer2;
  if (!jelly::MaskNoiseApplyEngine::ApplyMaskAndNoiseChunked(
          aDestination.data(), aWorker.data(), aRoundTrip.data(), aSource.size(),
          aMaskTable.data(), aMaskTable.size(), aNoiseTable.data(),
          aNoiseTable.size(), &aMixer2, &aError, jelly::CryptMode::kNormal)) {
    std::cerr << "Round-trip ApplyMaskAndNoiseChunked failed: " << aError << "\n";
    return 1;
  }
  if (!Equal(aSource, aRoundTrip)) {
    std::cerr << "round-trip mismatch for mask+noise\n";
    return 1;
  }

  jelly::SampleMixer aMixer3;
  if (!jelly::MaskNoiseApplyEngine::ApplyMaskChunked(
          aSource.data(), aWorker.data(), aDestination.data(), aSource.size(),
          aMaskTable.data(), aMaskTable.size(), &aMixer3, &aError,
          jelly::CryptMode::kNormal)) {
    std::cerr << "ApplyMaskChunked failed: " << aError << "\n";
    return 1;
  }
  jelly::SampleMixer aMixer4;
  if (!jelly::MaskNoiseApplyEngine::ApplyMaskChunked(
          aDestination.data(), aWorker.data(), aRoundTrip.data(), aSource.size(),
          aMaskTable.data(), aMaskTable.size(), &aMixer4, &aError,
          jelly::CryptMode::kNormal)) {
    std::cerr << "Round-trip ApplyMaskChunked failed: " << aError << "\n";
    return 1;
  }
  if (!Equal(aSource, aRoundTrip)) {
    std::cerr << "round-trip mismatch for mask-only\n";
    return 1;
  }

  jelly::SampleMixerNoise aMixer5;
  if (!jelly::MaskNoiseApplyEngine::ApplyNoiseChunked(
          aSource.data(), aWorker.data(), aDestination.data(), aSource.size(),
          aNoiseTable.data(), aNoiseTable.size(), &aMixer5, &aError,
          jelly::CryptMode::kNormal)) {
    std::cerr << "ApplyNoiseChunked failed: " << aError << "\n";
    return 1;
  }
  jelly::SampleMixerNoise aMixer6;
  if (!jelly::MaskNoiseApplyEngine::ApplyNoiseChunked(
          aDestination.data(), aWorker.data(), aRoundTrip.data(), aSource.size(),
          aNoiseTable.data(), aNoiseTable.size(), &aMixer6, &aError,
          jelly::CryptMode::kNormal)) {
    std::cerr << "Round-trip ApplyNoiseChunked failed: " << aError << "\n";
    return 1;
  }
  if (!Equal(aSource, aRoundTrip)) {
    std::cerr << "round-trip mismatch for noise-only\n";
    return 1;
  }

  // Smoke that both cursors can stream.
  std::array<unsigned char, 32> aCursorOut{};
  jelly::NoiseChunkCursor aNoiseCursor(aNoiseTable.data(), aNoiseTable.size());
  aNoiseCursor.FillNext(aCursorOut.data(), aCursorOut.size());

  std::cout << "PASS TestMaskNoiseApplyEngine\n";
  return 0;
}
