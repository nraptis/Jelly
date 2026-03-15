#include "MaskNoiseApplyEngine.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

#include "../Jelly.hpp"
#include "MaskChunkCursor.hpp"
#include "NoiseChunkCursor.hpp"

namespace jelly {

namespace {

constexpr std::size_t kChunkSize = 768;

void SetError(std::string* pErrorMessage, const char* pMessage) {
  if (pErrorMessage != nullptr) {
    *pErrorMessage = pMessage;
  }
}

bool ValidateCore(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage) {
  if (pErrorMessage != nullptr) {
    pErrorMessage->clear();
  }
  if (pLength == 0) {
    return true;
  }
  if ((pLength % SB_CIPHER_LENGTH_GRANULARITY) != 0) {
    SetError(pErrorMessage, "pLength must be a multiple of 192");
    return false;
  }
  if (pSource == nullptr || pWorker == nullptr || pDestination == nullptr) {
    SetError(pErrorMessage, "null pointer input");
    return false;
  }
  if (pSource == pWorker || pSource == pDestination || pWorker == pDestination) {
    SetError(pErrorMessage, "source/worker/destination must not alias");
    return false;
  }
  return true;
}

bool ValidateMask(const unsigned char* pMask,
                  std::size_t pMaskLength,
                  std::string* pErrorMessage) {
  if (pMask == nullptr || pMaskLength == 0) {
    SetError(pErrorMessage, "mask is required");
    return false;
  }
  if ((pMaskLength % SB_CIPHER_LENGTH_GRANULARITY) != 0) {
    SetError(pErrorMessage, "pMaskLength must be a multiple of 192");
    return false;
  }
  return true;
}

bool ValidateNoise(const unsigned char* pNoise,
                   std::size_t pNoiseLength,
                   std::string* pErrorMessage) {
  if (pNoise == nullptr || pNoiseLength == 0) {
    SetError(pErrorMessage, "noise is required");
    return false;
  }
  if ((pNoiseLength % SB_CIPHER_LENGTH_GRANULARITY) != 0) {
    SetError(pErrorMessage, "pNoiseLength must be a multiple of 192");
    return false;
  }
  return true;
}

void ApplyMaskExtractScalar(const unsigned char* pSource,
                            const std::uint8_t* pMask,
                            unsigned char* pMaskedOutput,
                            std::size_t pLength) {
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    pMaskedOutput[aIndex] =
        static_cast<unsigned char>(pSource[aIndex] & pMask[aIndex]);
  }
}

void ApplyComposeScalar(const unsigned char* pSource,
                        const std::uint8_t* pMask,
                        const std::uint8_t* pMixed,
                        unsigned char* pDestination,
                        std::size_t pLength) {
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    const std::uint8_t aMaskByte = pMask[aIndex];
    const std::uint8_t aAntiMaskByte = static_cast<std::uint8_t>(~aMaskByte);
    const std::uint8_t aSourceByte = pSource[aIndex];
    const std::uint8_t aMixedMasked = static_cast<std::uint8_t>(
        (aSourceByte ^ pMixed[aIndex]) & aMaskByte);
    pDestination[aIndex] =
        static_cast<unsigned char>((aSourceByte & aAntiMaskByte) | aMixedMasked);
  }
}

#if defined(__AVX2__) || defined(__SSSE3__)
void ApplyMaskExtractSimd(const unsigned char* pSource,
                          const std::uint8_t* pMask,
                          unsigned char* pMaskedOutput,
                          std::size_t pLength) {
#if defined(__AVX2__)
  std::size_t aIndex = 0;
  for (; aIndex + 32 <= pLength; aIndex += 32) {
    const __m256i aSource =
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pSource + aIndex));
    const __m256i aMask =
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pMask + aIndex));
    const __m256i aMasked = _mm256_and_si256(aSource, aMask);
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pMaskedOutput + aIndex),
                        aMasked);
  }
  ApplyMaskExtractScalar(pSource + aIndex, pMask + aIndex, pMaskedOutput + aIndex,
                         pLength - aIndex);
#else
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const __m128i aSource =
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource + aIndex));
    const __m128i aMask =
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(pMask + aIndex));
    const __m128i aMasked = _mm_and_si128(aSource, aMask);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pMaskedOutput + aIndex), aMasked);
  }
  ApplyMaskExtractScalar(pSource + aIndex, pMask + aIndex, pMaskedOutput + aIndex,
                         pLength - aIndex);
#endif
}

void ApplyComposeSimd(const unsigned char* pSource,
                      const std::uint8_t* pMask,
                      const std::uint8_t* pMixed,
                      unsigned char* pDestination,
                      std::size_t pLength) {
#if defined(__AVX2__)
  const __m256i aOnes = _mm256_set1_epi8(static_cast<char>(0xFF));
  std::size_t aIndex = 0;
  for (; aIndex + 32 <= pLength; aIndex += 32) {
    const __m256i aSource =
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pSource + aIndex));
    const __m256i aMask =
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pMask + aIndex));
    const __m256i aMixed =
        _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pMixed + aIndex));
    const __m256i aAntiMask = _mm256_xor_si256(aMask, aOnes);
    const __m256i aBase = _mm256_and_si256(aSource, aAntiMask);
    const __m256i aMixedMasked =
        _mm256_and_si256(_mm256_xor_si256(aSource, aMixed), aMask);
    const __m256i aResult = _mm256_or_si256(aBase, aMixedMasked);
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pDestination + aIndex),
                        aResult);
  }
  ApplyComposeScalar(pSource + aIndex, pMask + aIndex, pMixed + aIndex,
                     pDestination + aIndex, pLength - aIndex);
#else
  const __m128i aOnes = _mm_set1_epi8(static_cast<char>(0xFF));
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const __m128i aSource =
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource + aIndex));
    const __m128i aMask =
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(pMask + aIndex));
    const __m128i aMixed =
        _mm_loadu_si128(reinterpret_cast<const __m128i*>(pMixed + aIndex));
    const __m128i aAntiMask = _mm_xor_si128(aMask, aOnes);
    const __m128i aBase = _mm_and_si128(aSource, aAntiMask);
    const __m128i aMixedMasked = _mm_and_si128(_mm_xor_si128(aSource, aMixed), aMask);
    const __m128i aResult = _mm_or_si128(aBase, aMixedMasked);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination + aIndex), aResult);
  }
  ApplyComposeScalar(pSource + aIndex, pMask + aIndex, pMixed + aIndex,
                     pDestination + aIndex, pLength - aIndex);
#endif
}
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
void ApplyMaskExtractNeon(const unsigned char* pSource,
                          const std::uint8_t* pMask,
                          unsigned char* pMaskedOutput,
                          std::size_t pLength) {
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const uint8x16_t aSource = vld1q_u8(pSource + aIndex);
    const uint8x16_t aMask = vld1q_u8(pMask + aIndex);
    const uint8x16_t aMasked = vandq_u8(aSource, aMask);
    vst1q_u8(pMaskedOutput + aIndex, aMasked);
  }
  ApplyMaskExtractScalar(pSource + aIndex, pMask + aIndex, pMaskedOutput + aIndex,
                         pLength - aIndex);
}

void ApplyComposeNeon(const unsigned char* pSource,
                      const std::uint8_t* pMask,
                      const std::uint8_t* pMixed,
                      unsigned char* pDestination,
                      std::size_t pLength) {
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const uint8x16_t aSource = vld1q_u8(pSource + aIndex);
    const uint8x16_t aMask = vld1q_u8(pMask + aIndex);
    const uint8x16_t aMixed = vld1q_u8(pMixed + aIndex);
    const uint8x16_t aAntiMask = vmvnq_u8(aMask);
    const uint8x16_t aBase = vandq_u8(aSource, aAntiMask);
    const uint8x16_t aMixedMasked = vandq_u8(veorq_u8(aSource, aMixed), aMask);
    const uint8x16_t aResult = vorrq_u8(aBase, aMixedMasked);
    vst1q_u8(pDestination + aIndex, aResult);
  }
  ApplyComposeScalar(pSource + aIndex, pMask + aIndex, pMixed + aIndex,
                     pDestination + aIndex, pLength - aIndex);
}
#endif

bool ApplyInternal(const unsigned char* pSource,
                   unsigned char* pWorker,
                   unsigned char* pDestination,
                   std::size_t pLength,
                   const unsigned char* pMask,
                   std::size_t pMaskLength,
                   Mixer* pMixer,
                   std::string* pErrorMessage,
                   CryptMode pMode) {
  std::array<std::uint8_t, kChunkSize> aMaskChunk{};
  std::array<std::uint8_t, kChunkSize> aMixedChunk{};

  MaskChunkCursor aMaskCursor(pMask, pMaskLength);

  for (std::size_t aOffset = 0; aOffset < pLength; aOffset += kChunkSize) {
    const std::size_t aChunkLength = std::min(kChunkSize, pLength - aOffset);

    if (pMask != nullptr) {
      aMaskCursor.FillNext(aMaskChunk.data(), aChunkLength);
    } else {
      std::fill_n(aMaskChunk.data(), aChunkLength, static_cast<std::uint8_t>(0xFFu));
    }

    switch (pMode) {
      case CryptMode::kNormal:
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(), pWorker + aOffset,
                               aChunkLength);
        break;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        ApplyMaskExtractSimd(pSource + aOffset, aMaskChunk.data(), pWorker + aOffset,
                             aChunkLength);
#else
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(), pWorker + aOffset,
                               aChunkLength);
#endif
        break;
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        ApplyMaskExtractNeon(pSource + aOffset, aMaskChunk.data(), pWorker + aOffset,
                             aChunkLength);
#else
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(), pWorker + aOffset,
                               aChunkLength);
#endif
        break;
    }

    pMixer->Mix(pWorker + aOffset, aMixedChunk.data(), aChunkLength);

    switch (pMode) {
      case CryptMode::kNormal:
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(), aMixedChunk.data(),
                           pDestination + aOffset, aChunkLength);
        break;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        ApplyComposeSimd(pSource + aOffset, aMaskChunk.data(), aMixedChunk.data(),
                         pDestination + aOffset, aChunkLength);
#else
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(), aMixedChunk.data(),
                           pDestination + aOffset, aChunkLength);
#endif
        break;
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        ApplyComposeNeon(pSource + aOffset, aMaskChunk.data(), aMixedChunk.data(),
                         pDestination + aOffset, aChunkLength);
#else
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(), aMixedChunk.data(),
                           pDestination + aOffset, aChunkLength);
#endif
        break;
    }
  }

  if (pErrorMessage != nullptr) {
    pErrorMessage->clear();
  }
  return true;
}

bool ApplyInternalNoise(const unsigned char* pSource,
                        unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength,
                        const unsigned char* pMask,
                        std::size_t pMaskLength,
                        const unsigned char* pNoise,
                        std::size_t pNoiseLength,
                        MixerNoise* pMixer,
                        std::string* pErrorMessage,
                        CryptMode pMode) {
  std::array<std::uint8_t, kChunkSize> aMaskChunk{};
  std::array<std::uint8_t, kChunkSize> aNoiseChunk{};
  std::array<std::uint8_t, kChunkSize> aMixedChunk{};

  MaskChunkCursor aMaskCursor(pMask, pMaskLength);
  NoiseChunkCursor aNoiseCursor(pNoise, pNoiseLength);

  for (std::size_t aOffset = 0; aOffset < pLength; aOffset += kChunkSize) {
    const std::size_t aChunkLength = std::min(kChunkSize, pLength - aOffset);

    if (pMask != nullptr) {
      aMaskCursor.FillNext(aMaskChunk.data(), aChunkLength);
    } else {
      std::fill_n(aMaskChunk.data(), aChunkLength,
                  static_cast<std::uint8_t>(0xFFu));
    }
    aNoiseCursor.FillNext(aNoiseChunk.data(), aChunkLength);

    switch (pMode) {
      case CryptMode::kNormal:
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(),
                               pWorker + aOffset, aChunkLength);
        break;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        ApplyMaskExtractSimd(pSource + aOffset, aMaskChunk.data(),
                             pWorker + aOffset, aChunkLength);
#else
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(),
                               pWorker + aOffset, aChunkLength);
#endif
        break;
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        ApplyMaskExtractNeon(pSource + aOffset, aMaskChunk.data(),
                             pWorker + aOffset, aChunkLength);
#else
        ApplyMaskExtractScalar(pSource + aOffset, aMaskChunk.data(),
                               pWorker + aOffset, aChunkLength);
#endif
        break;
    }

    pMixer->Mix(pWorker + aOffset, aMixedChunk.data(),
                reinterpret_cast<unsigned char*>(aNoiseChunk.data()),
                aChunkLength);

    switch (pMode) {
      case CryptMode::kNormal:
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(),
                           aMixedChunk.data(), pDestination + aOffset,
                           aChunkLength);
        break;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        ApplyComposeSimd(pSource + aOffset, aMaskChunk.data(),
                         aMixedChunk.data(), pDestination + aOffset,
                         aChunkLength);
#else
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(),
                           aMixedChunk.data(), pDestination + aOffset,
                           aChunkLength);
#endif
        break;
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        ApplyComposeNeon(pSource + aOffset, aMaskChunk.data(),
                         aMixedChunk.data(), pDestination + aOffset,
                         aChunkLength);
#else
        ApplyComposeScalar(pSource + aOffset, aMaskChunk.data(),
                           aMixedChunk.data(), pDestination + aOffset,
                           aChunkLength);
#endif
        break;
    }
  }

  if (pErrorMessage != nullptr) {
    pErrorMessage->clear();
  }
  return true;
}

}  // namespace

bool MaskNoiseApplyEngine::ApplyMaskAndNoiseChunked(const unsigned char* pSource,
                                                    unsigned char* pWorker,
                                                    unsigned char* pDestination,
                                                    std::size_t pLength,
                                                    const unsigned char* pMask,
                                                    std::size_t pMaskLength,
                                                    const unsigned char* pNoise,
                                                    std::size_t pNoiseLength,
                                                    MixerNoise* pMixer,
                                                    std::string* pErrorMessage,
                                                    CryptMode pMode) {
  (void)pMode;
  if (!ValidateCore(pSource, pWorker, pDestination, pLength, pErrorMessage)) {
    return false;
  }
  if (pLength == 0) {
    return true;
  }
  if (pMixer == nullptr) {
    SetError(pErrorMessage, "null mixer input");
    return false;
  }
  if (!ValidateMask(pMask, pMaskLength, pErrorMessage) ||
      !ValidateNoise(pNoise, pNoiseLength, pErrorMessage)) {
    return false;
  }
  return ApplyInternalNoise(pSource, pWorker, pDestination, pLength, pMask,
                            pMaskLength, pNoise, pNoiseLength, pMixer,
                            pErrorMessage, pMode);
}

bool MaskNoiseApplyEngine::ApplyMaskChunked(const unsigned char* pSource,
                                            unsigned char* pWorker,
                                            unsigned char* pDestination,
                                            std::size_t pLength,
                                            const unsigned char* pMask,
                                            std::size_t pMaskLength,
                                            Mixer* pMixer,
                                            std::string* pErrorMessage,
                                            CryptMode pMode) {
  (void)pMode;
  if (!ValidateCore(pSource, pWorker, pDestination, pLength, pErrorMessage)) {
    return false;
  }
  if (pLength == 0) {
    return true;
  }
  if (pMixer == nullptr) {
    SetError(pErrorMessage, "null mixer input");
    return false;
  }
  if (!ValidateMask(pMask, pMaskLength, pErrorMessage)) {
    return false;
  }
  return ApplyInternal(pSource, pWorker, pDestination, pLength, pMask,
                       pMaskLength, pMixer, pErrorMessage, pMode);
}

bool MaskNoiseApplyEngine::ApplyNoiseChunked(const unsigned char* pSource,
                                             unsigned char* pWorker,
                                             unsigned char* pDestination,
                                             std::size_t pLength,
                                             const unsigned char* pNoise,
                                             std::size_t pNoiseLength,
                                             MixerNoise* pMixer,
                                             std::string* pErrorMessage,
                                             CryptMode pMode) {
  (void)pMode;
  if (!ValidateCore(pSource, pWorker, pDestination, pLength, pErrorMessage)) {
    return false;
  }
  if (pLength == 0) {
    return true;
  }
  if (pMixer == nullptr) {
    SetError(pErrorMessage, "null mixer input");
    return false;
  }
  if (!ValidateNoise(pNoise, pNoiseLength, pErrorMessage)) {
    return false;
  }
  return ApplyInternalNoise(pSource, pWorker, pDestination, pLength, nullptr, 0,
                            pNoise, pNoiseLength, pMixer, pErrorMessage, pMode);
}

}  // namespace jelly
