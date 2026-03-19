#include "TestBrewExecutor.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../src/PeanutButter.hpp"
#include "TestBrewGenerator.hpp"

namespace peanutbutter {

namespace {

bool HasError(const std::string* pError) {
  return pError != nullptr && !pError->empty();
}

void SetError(std::string* pError, const std::string& pMessage) {
  if (pError != nullptr && pError->empty()) {
    *pError = pMessage;
  }
}

const unsigned char* ReadPtr(const std::vector<unsigned char>& pBytes,
                             unsigned char* pDummy) {
  if (pBytes.empty()) {
    *pDummy = 0;
    return pDummy;
  }
  return pBytes.data();
}

unsigned char* WritePtr(std::vector<unsigned char>& pBytes,
                        unsigned char* pDummy) {
  if (pBytes.empty()) {
    *pDummy = 0;
    return pDummy;
  }
  return pBytes.data();
}

std::size_t FirstMismatch(const std::vector<unsigned char>& pLeft,
                          const std::vector<unsigned char>& pRight) {
  const std::size_t aCount = std::min(pLeft.size(), pRight.size());
  for (std::size_t aIndex = 0; aIndex < aCount; ++aIndex) {
    if (pLeft[aIndex] != pRight[aIndex]) {
      return aIndex;
    }
  }
  return aCount;
}

std::string FormatWindow(const std::vector<unsigned char>& pBytes,
                         std::size_t pIndex) {
  std::ostringstream aStream;
  const std::size_t aStart = pIndex > 8 ? pIndex - 8 : 0;
  const std::size_t aEnd = std::min(pBytes.size(), pIndex + 9);
  aStream << "[";
  for (std::size_t aOffset = aStart; aOffset < aEnd; ++aOffset) {
    if (aOffset != aStart) {
      aStream << " ";
    }
    if (aOffset == pIndex) {
      aStream << "<";
    }
    aStream << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(pBytes[aOffset]);
    if (aOffset == pIndex) {
      aStream << ">";
    }
  }
  aStream << "]";
  return aStream.str();
}

std::string FormatFailure(const std::string& pName,
                          CryptMode pMode,
                          const std::string& pBucket,
                          std::size_t pCaseIndex,
                          const std::vector<unsigned char>& pInput,
                          const std::vector<unsigned char>& pEncrypted,
                          const std::vector<unsigned char>& pRoundTrip,
                          std::size_t pMismatchIndex) {
  std::ostringstream aStream;
  aStream << pName << " mode=" << GetCryptModeName(pMode) << " failed in "
          << pBucket << " at case " << pCaseIndex << "\n";
  aStream << "length=" << pInput.size() << " mismatch_index=" << pMismatchIndex
          << "\n";
  if (pMismatchIndex < pInput.size()) {
    aStream << "input_byte=0x" << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(pInput[pMismatchIndex]) << "\n";
  }
  if (pMismatchIndex < pRoundTrip.size()) {
    aStream << "round_trip_byte=0x" << std::hex << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(pRoundTrip[pMismatchIndex]) << "\n";
  }
  aStream << "input_window=" << FormatWindow(pInput, pMismatchIndex) << "\n";
  aStream << "encrypted_window=" << FormatWindow(pEncrypted, pMismatchIndex)
          << "\n";
  aStream << "round_trip_window=" << FormatWindow(pRoundTrip, pMismatchIndex);
  return aStream.str();
}

bool RunRoundTripCase(const CipherFactory& pFactory,
                      const std::string& pName,
                      const std::string& pBucket,
                      std::size_t pCaseIndex,
                      const std::vector<unsigned char>& pInput,
                      std::string* pError) {
  if ((pInput.size() % BLOCK_GRANULARITY) != 0) {
    SetError(pError, pName + " failed in " + pBucket + " at case " +
                         std::to_string(pCaseIndex) +
                         ": input length is not a multiple of 192");
    return false;
  }
  for (CryptMode aMode : GetAvailableCryptModes()) {
    std::unique_ptr<Crypt> aCipher = pFactory(pCaseIndex, aMode);
    if (!aCipher) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + ": null cipher");
      return false;
    }

    std::vector<unsigned char> aEncrypted(pInput.size());
    std::vector<unsigned char> aRoundTrip(pInput.size());
    std::vector<unsigned char> aWorker(pInput.size());
    unsigned char aInputDummy = 0;
    unsigned char aWorkerDummy = 0;
    unsigned char aEncryptedDummy = 0;
    unsigned char aRoundTripDummy = 0;

    std::string aCipherError;
    if (!aCipher->SealData(ReadPtr(pInput, &aInputDummy),
                           WritePtr(aWorker, &aWorkerDummy),
                           WritePtr(aEncrypted, &aEncryptedDummy),
                           pInput.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during encrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    aCipherError.clear();
    if (!aCipher->UnsealData(ReadPtr(aEncrypted, &aEncryptedDummy),
                             WritePtr(aWorker, &aWorkerDummy),
                             WritePtr(aRoundTrip, &aRoundTripDummy),
                             aEncrypted.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during decrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    if (aRoundTrip != pInput) {
      const std::size_t aMismatchIndex = FirstMismatch(pInput, aRoundTrip);
      SetError(pError, FormatFailure(pName, aMode, pBucket, pCaseIndex, pInput,
                                     aEncrypted, aRoundTrip, aMismatchIndex));
      return false;
    }
  }

  return true;
}

bool RunRoundTripCase(const SizedCipherFactory& pFactory,
                      const std::string& pName,
                      const std::string& pBucket,
                      std::size_t pCaseIndex,
                      const std::vector<unsigned char>& pInput,
                      std::string* pError) {
  if ((pInput.size() % BLOCK_GRANULARITY) != 0) {
    SetError(pError, pName + " failed in " + pBucket + " at case " +
                         std::to_string(pCaseIndex) +
                         ": input length is not a multiple of 192");
    return false;
  }
  for (CryptMode aMode : GetAvailableCryptModes()) {
    std::unique_ptr<Crypt> aCipher =
        pFactory(pInput.size(), pCaseIndex, aMode);
    if (!aCipher) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + ": null cipher");
      return false;
    }

    std::vector<unsigned char> aEncrypted(pInput.size());
    std::vector<unsigned char> aRoundTrip(pInput.size());
    std::vector<unsigned char> aWorker(pInput.size());
    unsigned char aInputDummy = 0;
    unsigned char aWorkerDummy = 0;
    unsigned char aEncryptedDummy = 0;
    unsigned char aRoundTripDummy = 0;

    std::string aCipherError;
    if (!aCipher->SealData(ReadPtr(pInput, &aInputDummy),
                           WritePtr(aWorker, &aWorkerDummy),
                           WritePtr(aEncrypted, &aEncryptedDummy),
                           pInput.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during encrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    aCipherError.clear();
    if (!aCipher->UnsealData(ReadPtr(aEncrypted, &aEncryptedDummy),
                             WritePtr(aWorker, &aWorkerDummy),
                             WritePtr(aRoundTrip, &aRoundTripDummy),
                             aEncrypted.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during decrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    if (aRoundTrip != pInput) {
      const std::size_t aMismatchIndex = FirstMismatch(pInput, aRoundTrip);
      SetError(pError, FormatFailure(pName, aMode, pBucket, pCaseIndex, pInput,
                                     aEncrypted, aRoundTrip, aMismatchIndex));
      return false;
    }
  }

  return true;
}

bool RunRoundTripCase(const BlockCipherFactory& pFactory,
                      int pBlockSize,
                      const std::string& pName,
                      const std::string& pBucket,
                      std::size_t pCaseIndex,
                      const std::vector<unsigned char>& pInput,
                      std::string* pError) {
  if ((pInput.size() % BLOCK_GRANULARITY) != 0) {
    SetError(pError, pName + " block_size=" + std::to_string(pBlockSize) +
                         " failed in " + pBucket + " at case " +
                         std::to_string(pCaseIndex) +
                         ": input length is not a multiple of 192");
    return false;
  }
  for (CryptMode aMode : GetAvailableCryptModes()) {
    std::unique_ptr<Crypt> aCipher =
        pFactory(pBlockSize, pCaseIndex, aMode);
    if (!aCipher) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " block_size=" + std::to_string(pBlockSize) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + ": null cipher");
      return false;
    }

    std::vector<unsigned char> aEncrypted(pInput.size());
    std::vector<unsigned char> aRoundTrip(pInput.size());
    std::vector<unsigned char> aWorker(pInput.size());
    unsigned char aInputDummy = 0;
    unsigned char aWorkerDummy = 0;
    unsigned char aEncryptedDummy = 0;
    unsigned char aRoundTripDummy = 0;

    std::string aCipherError;
    if (!aCipher->SealData(ReadPtr(pInput, &aInputDummy),
                           WritePtr(aWorker, &aWorkerDummy),
                           WritePtr(aEncrypted, &aEncryptedDummy),
                           pInput.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " block_size=" + std::to_string(pBlockSize) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during encrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    aCipherError.clear();
    if (!aCipher->UnsealData(ReadPtr(aEncrypted, &aEncryptedDummy),
                             WritePtr(aWorker, &aWorkerDummy),
                             WritePtr(aRoundTrip, &aRoundTripDummy),
                             aEncrypted.size(), &aCipherError, aMode)) {
      SetError(pError, pName + " mode=" + GetCryptModeName(aMode) +
                          " block_size=" + std::to_string(pBlockSize) +
                          " failed in " + pBucket + " at case " +
                          std::to_string(pCaseIndex) + " during decrypt" +
                          (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }

    if (aRoundTrip != pInput) {
      const std::size_t aMismatchIndex = FirstMismatch(pInput, aRoundTrip);
      SetError(pError, FormatFailure(pName, aMode, pBucket, pCaseIndex, pInput,
                                     aEncrypted, aRoundTrip, aMismatchIndex));
      return false;
    }
  }

  return true;
}

bool RunFlatBucket(const CipherFactory& pFactory,
                   const std::string& pName,
                   const std::string& pBucket,
                   std::size_t pCaseCount,
                   std::size_t pMinLength,
                   std::size_t pMaxLength,
                   std::string* pError) {
  for (std::size_t aCaseIndex = 0; aCaseIndex < pCaseCount; ++aCaseIndex) {
    if (HasError(pError)) {
      return false;
    }
    const std::size_t aRange = pMaxLength - pMinLength + 1;
    const std::size_t aLength =
        TestBrewGenerator::NormalizeLength(pMinLength + (aCaseIndex % aRange));
    const std::vector<unsigned char> aInput =
        TestBrewGenerator::GenerateBytes(aLength);
    if (!RunRoundTripCase(pFactory, pName, pBucket, aCaseIndex, aInput,
                          pError)) {
      return false;
    }
  }
  return true;
}

bool RunFlatBucket(const SizedCipherFactory& pFactory,
                   const std::string& pName,
                   const std::string& pBucket,
                   std::size_t pCaseCount,
                   std::size_t pMinLength,
                   std::size_t pMaxLength,
                   std::string* pError) {
  for (std::size_t aCaseIndex = 0; aCaseIndex < pCaseCount; ++aCaseIndex) {
    if (HasError(pError)) {
      return false;
    }
    const std::size_t aRange = pMaxLength - pMinLength + 1;
    const std::size_t aLength =
        TestBrewGenerator::NormalizeLength(pMinLength + (aCaseIndex % aRange));
    const std::vector<unsigned char> aInput =
        TestBrewGenerator::GenerateBytes(aLength);
    if (!RunRoundTripCase(pFactory, pName, pBucket, aCaseIndex, aInput,
                          pError)) {
      return false;
    }
  }
  return true;
}

bool RunBlockBucket(const BlockCipherFactory& pFactory,
                    int pBlockSize,
                    const std::string& pName,
                    const std::string& pBucket,
                    std::size_t pCaseCount,
                    std::size_t pMinBlockCount,
                    std::size_t pMaxBlockCount,
                    std::string* pError) {
  if (pBlockSize <= 0) {
    SetError(pError, pName + " failed: bad block size");
    return false;
  }

  for (std::size_t aCaseIndex = 0; aCaseIndex < pCaseCount; ++aCaseIndex) {
    if (HasError(pError)) {
      return false;
    }
    const std::size_t aRange = pMaxBlockCount - pMinBlockCount + 1;
    const std::size_t aTargetLength = TestBrewGenerator::NormalizeLength(
        static_cast<std::size_t>(pBlockSize) *
        (pMinBlockCount + (aCaseIndex % aRange)));
    const std::size_t aBlockCount =
        aTargetLength / static_cast<std::size_t>(pBlockSize);
    const std::vector<unsigned char> aInput =
        TestBrewGenerator::GenerateBytesBlock(pBlockSize,
                                              static_cast<int>(aBlockCount));
    if (!RunRoundTripCase(pFactory, pBlockSize, pName, pBucket, aCaseIndex,
                          aInput, pError)) {
      return false;
    }
  }
  return true;
}

}  // namespace

void ExecuteTestBrew_Flat(const CipherFactory& pFactory,
                          const std::string& pName,
                          std::string* pError) {
  if (pError != nullptr) {
    pError->clear();
  }
  RunFlatBucket(pFactory, pName, "Small", 5000, 192, 3072, pError);
  RunFlatBucket(pFactory, pName, "Medium", 500, 192, 4032, pError);
  RunFlatBucket(pFactory, pName, "Large", 50, 4224, 8064, pError);
}

void ExecuteTestBrew_FlatSized(const SizedCipherFactory& pFactory,
                               const std::string& pName,
                               std::string* pError) {
  if (!pFactory) {
    SetError(pError, pName + ": factory must not be empty");
    return;
  }
  if (!RunFlatBucket(pFactory, pName, "1000_small_random", 1000, 0, 16, pError) ||
      !RunFlatBucket(pFactory, pName, "250_medium_random", 250, 16, 4096,
                     pError) ||
      !RunFlatBucket(pFactory, pName, "50_large_random", 50, 4096, 8192,
                     pError)) {
    return;
  }
}

void ExecuteTestBrew_Block(const BlockCipherFactory& pFactory,
                           int pBlockSize,
                           const std::string& pName,
                           std::string* pError) {
  if (pError != nullptr) {
    pError->clear();
  }
  RunBlockBucket(pFactory, pBlockSize, pName, "Small_Block", 5000, 1, 16,
                 pError);
  RunBlockBucket(pFactory, pBlockSize, pName, "Medium_Block", 500, 1, 21,
                 pError);
  RunBlockBucket(pFactory, pBlockSize, pName, "Large_Block", 50, 22, 42,
                 pError);
}

}  // namespace peanutbutter
