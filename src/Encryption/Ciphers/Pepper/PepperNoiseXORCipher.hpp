#ifndef PEANUTBUTTER_PEPPER_NOISE_XOR_CIPHER_HPP_
#define PEANUTBUTTER_PEPPER_NOISE_XOR_CIPHER_HPP_

#include <cstddef>
#include <cstdint>
#include <string>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

#include "../../Crypt.hpp"
#include "../../../PeanutButter.hpp"

namespace peanutbutter {

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; mask and noise lengths must be positive multiples of 16.

class PepperNoiseXORCipher final : public Crypt {
 public:
  PepperNoiseXORCipher(const unsigned char* pMask,
                       std::size_t pMaskLength,
                       const unsigned char* pNoise,
                       std::size_t pNoiseLength)
      : mMask(pMask),
        mMaskLength(pMaskLength),
        mNoise(pNoise),
        mNoiseLength(pNoiseLength) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    (void)pWorker;
    return Apply(pSource, pDestination, pLength, pErrorMessage, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    (void)pWorker;
    return Apply(pSource, pDestination, pLength, pErrorMessage, pMode);
  }

 private:
  static void SetError(std::string* pErrorMessage, const char* pMessage) {
    if (pErrorMessage != nullptr) {
      *pErrorMessage = pMessage;
    }
  }

  bool Validate(const unsigned char* pSource,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage) const {
    if (pErrorMessage != nullptr) {
      pErrorMessage->clear();
    }
    if (pLength == 0u) {
      return true;
    }
    if ((pLength % BLOCK_GRANULARITY) != 0u) {
      SetError(pErrorMessage, "pLength must be a multiple of BLOCK_GRANULARITY");
      return false;
    }
    if (pSource == nullptr || pDestination == nullptr || mMask == nullptr ||
        mNoise == nullptr) {
      SetError(pErrorMessage, "source/destination/mask/noise must not be null");
      return false;
    }
    if (pSource == pDestination) {
      SetError(pErrorMessage, "source and destination must not alias");
      return false;
    }
    if (mMaskLength == 0u || mNoiseLength == 0u ||
        (mMaskLength % 16u) != 0u || (mNoiseLength % 16u) != 0u) {
      SetError(
          pErrorMessage,
          "mask and noise lengths must be positive multiples of 16");
      return false;
    }
    return true;
  }

  bool Apply(const unsigned char* pSource,
             unsigned char* pDestination,
             std::size_t pLength,
             std::string* pErrorMessage,
             CryptMode pMode) const {
    if (!Validate(pSource, pDestination, pLength, pErrorMessage)) {
      return pLength == 0u;
    }
    if (pLength == 0u) {
      return true;
    }
    const std::size_t aChunkCount = pLength >> 4;
    const std::size_t aMaskChunkCount = mMaskLength >> 4;
    const std::size_t aNoiseChunkCount = mNoiseLength >> 4;
    std::size_t aMaskChunkIndex = 0u;
    std::size_t aNoiseChunkIndex = 0u;

    for (std::size_t aChunk = 0; aChunk < aChunkCount; ++aChunk) {
      const unsigned char* aSourcePtr = pSource + (aChunk << 4);
      const unsigned char* aMaskPtr = mMask + (aMaskChunkIndex << 4);
      const unsigned char* aNoisePtr = mNoise + (aNoiseChunkIndex << 4);
      unsigned char* aDestinationPtr = pDestination + (aChunk << 4);
      switch (pMode) {
        case CryptMode::kNormal:
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                aSourcePtr[aIndex] ^ aMaskPtr[aIndex] ^ aNoisePtr[aIndex]);
          }
          break;
      case CryptMode::kSimd:
#if defined(__AVX2__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_xor_si128(
                  _mm_xor_si128(
                      _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                      _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aNoisePtr))));
          break;
#elif defined(__SSSE3__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_xor_si128(
                  _mm_xor_si128(
                      _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                      _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aNoisePtr))));
          break;
#else
          SetError(pErrorMessage, "SIMD mode is not available");
          return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
          vst1q_u8(
              aDestinationPtr,
              veorq_u8(veorq_u8(vld1q_u8(aSourcePtr), vld1q_u8(aMaskPtr)),
                       vld1q_u8(aNoisePtr)));
          break;
#else
          SetError(pErrorMessage, "NEON mode is not available");
          return false;
#endif
      }
      ++aMaskChunkIndex;
      ++aNoiseChunkIndex;
      if (aMaskChunkIndex == aMaskChunkCount) {
        aMaskChunkIndex = 0u;
      }
      if (aNoiseChunkIndex == aNoiseChunkCount) {
        aNoiseChunkIndex = 0u;
      }
    }
    return true;
  }

  const unsigned char* mMask = nullptr;
  std::size_t mMaskLength = 0;
  const unsigned char* mNoise = nullptr;
  std::size_t mNoiseLength = 0;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PEPPER_NOISE_XOR_CIPHER_HPP_
