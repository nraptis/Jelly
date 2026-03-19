#ifndef PEANUTBUTTER_PASSWORD_XOR_CIPHER_HPP_
#define PEANUTBUTTER_PASSWORD_XOR_CIPHER_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

#include "../../Crypt.hpp"
#include "../../../PeanutButter.hpp"

namespace peanutbutter {

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; password length must be a positive multiple of 16.

class PasswordXORCipher final : public Crypt {
 public:
  PasswordXORCipher(const unsigned char* pPassword, int pPasswordLength)
      : mPasswordBytes(BuildPasswordBytes(pPassword, pPasswordLength)) {}

  explicit PasswordXORCipher(const std::vector<unsigned char>& pPasswordBytes)
      : PasswordXORCipher(pPasswordBytes.data(),
                          static_cast<int>(pPasswordBytes.size())) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode);
  }

 private:
  static std::vector<unsigned char> BuildPasswordBytes(const unsigned char* pPassword,
                                                       int pPasswordLength) {
    if (pPassword == nullptr || pPasswordLength <= 0 || (pPasswordLength % 16) != 0) {
      return {};
    }
    return std::vector<unsigned char>(pPassword, pPassword + pPasswordLength);
  }

  static void SetError(std::string* pErrorMessage, const char* pMessage) {
    if (pErrorMessage != nullptr) {
      *pErrorMessage = pMessage;
    }
  }

  bool Apply(const unsigned char* pSource,
             unsigned char* pWorker,
             unsigned char* pDestination,
             std::size_t pLength,
             std::string* pErrorMessage,
             CryptMode pMode) const {
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
    if (pSource == nullptr || pWorker == nullptr || pDestination == nullptr) {
      SetError(pErrorMessage, "source/worker/destination must not be null");
      return false;
    }
    if (pSource == pDestination || pSource == pWorker || pDestination == pWorker) {
      SetError(pErrorMessage, "source/worker/destination must not alias");
      return false;
    }
    if (mPasswordBytes.empty()) {
      SetError(pErrorMessage, "password length must be a positive multiple of 16");
      return false;
    }

    ExpandPasswordToWorker(pWorker, pLength);

    switch (pMode) {
      case CryptMode::kNormal:
        ApplySoftware(pSource, pWorker, pDestination, pLength);
        return true;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        ApplySimd(pSource, pWorker, pDestination, pLength);
        return true;
#else
        SetError(pErrorMessage, "SIMD mode is not available");
        return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        ApplyNeon(pSource, pWorker, pDestination, pLength);
        return true;
#else
        SetError(pErrorMessage, "NEON mode is not available");
        return false;
#endif
    }
    return false;
  }

  void ExpandPasswordToWorker(unsigned char* pWorker, std::size_t pLength) const {
    const std::size_t aPasswordLength = mPasswordBytes.size();
    const std::size_t aFirst = std::min(aPasswordLength, pLength);
    std::memcpy(pWorker, mPasswordBytes.data(), aFirst);
    std::size_t aFilled = aFirst;
    while (aFilled < pLength) {
      const std::size_t aCopy = std::min(aFilled, pLength - aFilled);
      std::memcpy(pWorker + aFilled, pWorker, aCopy);
      aFilled += aCopy;
    }
  }

  static void ApplySoftware(const unsigned char* pSource,
                            const unsigned char* pWorker,
                            unsigned char* pDestination,
                            std::size_t pLength) {
    for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
      pDestination[aIndex] =
          static_cast<unsigned char>(pSource[aIndex] ^ pWorker[aIndex]);
    }
  }

#if defined(__AVX2__) || defined(__SSSE3__)
  static void ApplySimd(const unsigned char* pSource,
                        const unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength) {
    std::size_t aIndex = 0;
#if defined(__AVX2__)
    for (; aIndex + 32 <= pLength; aIndex += 32) {
      const __m256i aSource =
          _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pSource + aIndex));
      const __m256i aPassword =
          _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pWorker + aIndex));
      _mm256_storeu_si256(reinterpret_cast<__m256i*>(pDestination + aIndex),
                          _mm256_xor_si256(aSource, aPassword));
    }
#endif
    for (; aIndex + 16 <= pLength; aIndex += 16) {
      const __m128i aSource =
          _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource + aIndex));
      const __m128i aPassword =
          _mm_loadu_si128(reinterpret_cast<const __m128i*>(pWorker + aIndex));
      _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination + aIndex),
                       _mm_xor_si128(aSource, aPassword));
    }
    for (; aIndex < pLength; ++aIndex) {
      pDestination[aIndex] =
          static_cast<unsigned char>(pSource[aIndex] ^ pWorker[aIndex]);
    }
  }
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  static void ApplyNeon(const unsigned char* pSource,
                        const unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength) {
    std::size_t aIndex = 0;
    for (; aIndex + 16 <= pLength; aIndex += 16) {
      vst1q_u8(pDestination + aIndex,
               veorq_u8(vld1q_u8(pSource + aIndex), vld1q_u8(pWorker + aIndex)));
    }
    for (; aIndex < pLength; ++aIndex) {
      pDestination[aIndex] =
          static_cast<unsigned char>(pSource[aIndex] ^ pWorker[aIndex]);
    }
  }
#endif

  std::vector<unsigned char> mPasswordBytes;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PASSWORD_XOR_CIPHER_HPP_
