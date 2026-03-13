#include "MaskedPassword.hpp"

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

namespace jelly {

namespace {

bool ValidateCommon(unsigned char* pBytes,
                    const unsigned char* pPassword,
                    unsigned char* pWorker,
                    std::size_t pLength) {
  if (pLength == 0) {
    return true;
  }
  if (pBytes == nullptr || pPassword == nullptr || pWorker == nullptr) {
    return false;
  }
  if ((pLength % 48u) != 0u) {
    return false;
  }
  if (pBytes == pWorker || pPassword == pWorker) {
    return false;
  }
  return true;
}

bool ValidateArrayMask(unsigned char* pBytes,
                       const unsigned char* pPassword,
                       unsigned char* pWorker,
                       unsigned char* pMask,
                       std::size_t pLength) {
  if (!ValidateCommon(pBytes, pPassword, pWorker, pLength)) {
    return false;
  }
  if (pLength == 0) {
    return true;
  }
  if (pMask == nullptr) {
    return false;
  }
  if (pMask == pWorker) {
    return false;
  }
  return true;
}

bool ApplyMaskedPassword(unsigned char* pBytes,
                         const unsigned char* pPassword,
                         unsigned char* pWorker,
                         unsigned char pMask,
                         std::size_t pLength) {
#if defined(__AVX2__)
  const __m256i aMask = _mm256_set1_epi8(static_cast<char>(pMask));
  const __m256i aAntiMask =
      _mm256_xor_si256(aMask, _mm256_set1_epi8(static_cast<char>(0xff)));
  std::size_t aIndex = 0;
  for (; aIndex + 32 <= pLength; aIndex += 32) {
    const __m256i aBytes = _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(pBytes + aIndex));
    const __m256i aPassword = _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(pPassword + aIndex));
    const __m256i aWorker =
        _mm256_xor_si256(_mm256_and_si256(aBytes, aMask), aPassword);
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pWorker + aIndex), aWorker);
    const __m256i aDestination = _mm256_or_si256(
        _mm256_and_si256(aBytes, aAntiMask), _mm256_and_si256(aWorker, aMask));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pBytes + aIndex),
                        aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & pMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~pMask)) | (aWorker & pMask));
  }
  return true;
#elif defined(__SSSE3__)
  const __m128i aMask = _mm_set1_epi8(static_cast<char>(pMask));
  const __m128i aAntiMask =
      _mm_xor_si128(aMask, _mm_set1_epi8(static_cast<char>(0xff)));
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const __m128i aBytes = _mm_loadu_si128(
        reinterpret_cast<const __m128i*>(pBytes + aIndex));
    const __m128i aPassword = _mm_loadu_si128(
        reinterpret_cast<const __m128i*>(pPassword + aIndex));
    const __m128i aWorker =
        _mm_xor_si128(_mm_and_si128(aBytes, aMask), aPassword);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pWorker + aIndex), aWorker);
    const __m128i aDestination = _mm_or_si128(
        _mm_and_si128(aBytes, aAntiMask), _mm_and_si128(aWorker, aMask));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pBytes + aIndex), aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & pMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~pMask)) | (aWorker & pMask));
  }
  return true;
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
  const uint8x16_t aMask = vdupq_n_u8(pMask);
  const uint8x16_t aAntiMask = vmvnq_u8(aMask);
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const uint8x16_t aBytes = vld1q_u8(pBytes + aIndex);
    const uint8x16_t aPassword = vld1q_u8(pPassword + aIndex);
    const uint8x16_t aWorker = veorq_u8(vandq_u8(aBytes, aMask), aPassword);
    vst1q_u8(pWorker + aIndex, aWorker);
    const uint8x16_t aDestination =
        vorrq_u8(vandq_u8(aBytes, aAntiMask), vandq_u8(aWorker, aMask));
    vst1q_u8(pBytes + aIndex, aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & pMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~pMask)) | (aWorker & pMask));
  }
  return true;
#else
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & pMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~pMask)) | (aWorker & pMask));
  }
  return true;
#endif
}

bool ApplyMaskPasswordAray(unsigned char* pBytes,
                           const unsigned char* pPassword,
                           unsigned char* pWorker,
                           unsigned char* pMask,
                           std::size_t pLength) {
#if defined(__AVX2__)
  const __m256i aOnes = _mm256_set1_epi8(static_cast<char>(0xff));
  std::size_t aIndex = 0;
  for (; aIndex + 32 <= pLength; aIndex += 32) {
    const __m256i aBytes = _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(pBytes + aIndex));
    const __m256i aPassword = _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(pPassword + aIndex));
    const __m256i aMask = _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(pMask + aIndex));
    const __m256i aWorker =
        _mm256_xor_si256(_mm256_and_si256(aBytes, aMask), aPassword);
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pWorker + aIndex), aWorker);
    const __m256i aAntiMask = _mm256_xor_si256(aMask, aOnes);
    const __m256i aDestination = _mm256_or_si256(
        _mm256_and_si256(aBytes, aAntiMask), _mm256_and_si256(aWorker, aMask));
    _mm256_storeu_si256(reinterpret_cast<__m256i*>(pBytes + aIndex),
                        aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aMask = pMask[aIndex];
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & aMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~aMask)) | (aWorker & aMask));
  }
  return true;
#elif defined(__SSSE3__)
  const __m128i aOnes = _mm_set1_epi8(static_cast<char>(0xff));
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const __m128i aBytes = _mm_loadu_si128(
        reinterpret_cast<const __m128i*>(pBytes + aIndex));
    const __m128i aPassword = _mm_loadu_si128(
        reinterpret_cast<const __m128i*>(pPassword + aIndex));
    const __m128i aMask = _mm_loadu_si128(
        reinterpret_cast<const __m128i*>(pMask + aIndex));
    const __m128i aWorker =
        _mm_xor_si128(_mm_and_si128(aBytes, aMask), aPassword);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pWorker + aIndex), aWorker);
    const __m128i aAntiMask = _mm_xor_si128(aMask, aOnes);
    const __m128i aDestination = _mm_or_si128(
        _mm_and_si128(aBytes, aAntiMask), _mm_and_si128(aWorker, aMask));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(pBytes + aIndex), aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aMask = pMask[aIndex];
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & aMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~aMask)) | (aWorker & aMask));
  }
  return true;
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
  std::size_t aIndex = 0;
  for (; aIndex + 16 <= pLength; aIndex += 16) {
    const uint8x16_t aBytes = vld1q_u8(pBytes + aIndex);
    const uint8x16_t aPassword = vld1q_u8(pPassword + aIndex);
    const uint8x16_t aMask = vld1q_u8(pMask + aIndex);
    const uint8x16_t aWorker = veorq_u8(vandq_u8(aBytes, aMask), aPassword);
    vst1q_u8(pWorker + aIndex, aWorker);
    const uint8x16_t aAntiMask = vmvnq_u8(aMask);
    const uint8x16_t aDestination =
        vorrq_u8(vandq_u8(aBytes, aAntiMask), vandq_u8(aWorker, aMask));
    vst1q_u8(pBytes + aIndex, aDestination);
  }
  for (; aIndex < pLength; ++aIndex) {
    const unsigned char aMask = pMask[aIndex];
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & aMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~aMask)) | (aWorker & aMask));
  }
  return true;
#else
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    const unsigned char aMask = pMask[aIndex];
    const unsigned char aByte = pBytes[aIndex];
    const unsigned char aWorker =
        static_cast<unsigned char>((aByte & aMask) ^ pPassword[aIndex]);
    pWorker[aIndex] = aWorker;
    pBytes[aIndex] = static_cast<unsigned char>(
        (aByte & static_cast<unsigned char>(~aMask)) | (aWorker & aMask));
  }
  return true;
#endif
}

}  // namespace

bool MaskPasswordAray(unsigned char* pBytes,
                      const unsigned char* pPassword,
                      unsigned char* pWorker,
                      unsigned char* pMask,
                      std::size_t pLength) {
  if (!ValidateArrayMask(pBytes, pPassword, pWorker, pMask, pLength)) {
    return false;
  }
  return ApplyMaskPasswordAray(pBytes, pPassword, pWorker, pMask, pLength);
}

bool MaskedPassword(unsigned char* pBytes,
                    const unsigned char* pPassword,
                    unsigned char* pWorker,
                    unsigned char pMask,
                    std::size_t pLength) {
  if (!ValidateCommon(pBytes, pPassword, pWorker, pLength)) {
    return false;
  }
  return ApplyMaskedPassword(pBytes, pPassword, pWorker, pMask, pLength);
}

}  // namespace jelly
