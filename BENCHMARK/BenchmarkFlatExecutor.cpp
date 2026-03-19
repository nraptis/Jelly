#include "BenchmarkFlatExecutor.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <unistd.h>

#include "../src/PeanutButter.hpp"

namespace peanutbutter::benchmark {

namespace {

struct FreeDeleter {
  void operator()(void* pValue) const {
    std::free(pValue);
  }
};

struct BufferPair {
  std::unique_ptr<void, FreeDeleter> mSourceOwner;
  std::unique_ptr<void, FreeDeleter> mWorkerOwner;
  std::unique_ptr<void, FreeDeleter> mDestinationOwner;
  unsigned char* mSource = nullptr;
  unsigned char* mWorker = nullptr;
  unsigned char* mDestination = nullptr;
  std::size_t mAllocationLength = 0;
  std::size_t mPageSize = 0;
};

struct SectionResult {
  const char* mLabel = "";
  std::size_t mDataLength = 0;
  struct TrialResult {
    std::vector<double> mAveragesUs;
    std::vector<double> mGiBPerSecond;
  };
  std::vector<TrialResult> mTrials;
};

void SetError(std::string* pError, const std::string& pMessage) {
  if (pError != nullptr) {
    *pError = pMessage;
  }
}

std::size_t ReadPageSize() {
  const long aValue = ::sysconf(_SC_PAGESIZE);
  if (aValue <= 0) {
    return 4096;
  }
  return static_cast<std::size_t>(aValue);
}

std::size_t RoundUpToPage(std::size_t pLength, std::size_t pPageSize) {
  if (pLength == 0) {
    return pPageSize;
  }
  const std::size_t aRemainder = pLength % pPageSize;
  if (aRemainder == 0) {
    return pLength;
  }
  return pLength + (pPageSize - aRemainder);
}

bool AllocatePageAligned(BufferPair& pBuffers,
                         std::size_t pLength,
                         std::string* pError) {
  pBuffers.mPageSize = ReadPageSize();
  pBuffers.mAllocationLength = RoundUpToPage(pLength, pBuffers.mPageSize);

  void* aSource = nullptr;
  void* aWorker = nullptr;
  void* aDestination = nullptr;
  if (posix_memalign(&aSource, pBuffers.mPageSize, pBuffers.mAllocationLength) !=
      0) {
    SetError(pError, "posix_memalign failed for source buffer");
    return false;
  }
  if (posix_memalign(&aWorker, pBuffers.mPageSize, pBuffers.mAllocationLength) !=
      0) {
    std::free(aSource);
    SetError(pError, "posix_memalign failed for worker buffer");
    return false;
  }
  if (posix_memalign(&aDestination, pBuffers.mPageSize,
                     pBuffers.mAllocationLength) != 0) {
    std::free(aSource);
    std::free(aWorker);
    SetError(pError, "posix_memalign failed for destination buffer");
    return false;
  }

  pBuffers.mSourceOwner.reset(aSource);
  pBuffers.mWorkerOwner.reset(aWorker);
  pBuffers.mDestinationOwner.reset(aDestination);
  pBuffers.mSource = static_cast<unsigned char*>(aSource);
  pBuffers.mWorker = static_cast<unsigned char*>(aWorker);
  pBuffers.mDestination = static_cast<unsigned char*>(aDestination);
  return true;
}

void FillRandom(unsigned char* pBuffer, std::size_t pLength) {
  std::mt19937 aGenerator(0xC0FFEEu);
  std::uniform_int_distribution<int> aDistribution(0, 255);
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    pBuffer[aIndex] = static_cast<unsigned char>(aDistribution(aGenerator));
  }
}

bool TimeMode(const CipherFactory& pFactory,
              const BufferPair& pBuffers,
              std::size_t pDataLength,
              CryptMode pMode,
              double& pAverageUs,
              double& pGiBPerSecond,
              std::string* pError) {
  std::vector<double> aTimes;
  aTimes.reserve(RUN_COUNT);

  std::unique_ptr<Crypt> aCipher = pFactory(0, pMode);
  if (!aCipher) {
    SetError(pError,
             std::string("cipher factory returned null for mode ") +
                 GetCryptModeName(pMode));
    return false;
  }

  for (int aRun = 0; aRun < RUN_COUNT; ++aRun) {
    const auto aStart = std::chrono::steady_clock::now();
    std::string aCipherError;
    if (!aCipher->SealData(pBuffers.mSource, pBuffers.mWorker,
                           pBuffers.mDestination, pDataLength,
                           &aCipherError, pMode)) {
      SetError(pError,
               std::string("SealData failed for mode ") +
                   GetCryptModeName(pMode) +
                   (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }
    const auto aEnd = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::micro> aElapsed = aEnd - aStart;
    aTimes.push_back(aElapsed.count());
  }

  if (aTimes.size() <= static_cast<std::size_t>(DISCARD_LO + DISCARD_HI)) {
    SetError(pError, "not enough benchmark samples after trimming");
    return false;
  }

  std::sort(aTimes.begin(), aTimes.end());
  const auto aBegin = aTimes.begin() + DISCARD_LO;
  const auto aEnd = aTimes.end() - DISCARD_HI;
  const double aSum = std::accumulate(aBegin, aEnd, 0.0);
  pAverageUs = aSum / static_cast<double>(aEnd - aBegin);
  pGiBPerSecond =
      static_cast<double>(pDataLength) / (pAverageUs / 1000000.0) /
      (1024.0 * 1024.0 * 1024.0);
  return true;
}

bool TimeMode(const SizedCipherFactory& pFactory,
              const BufferPair& pBuffers,
              std::size_t pDataLength,
              CryptMode pMode,
              double& pAverageUs,
              double& pGiBPerSecond,
              std::string* pError) {
  std::vector<double> aTimes;
  aTimes.reserve(RUN_COUNT);

  std::unique_ptr<Crypt> aCipher = pFactory(pDataLength, 0, pMode);
  if (!aCipher) {
    SetError(pError,
             std::string("cipher factory returned null for mode ") +
                 GetCryptModeName(pMode));
    return false;
  }

  for (int aRun = 0; aRun < RUN_COUNT; ++aRun) {
    const auto aStart = std::chrono::steady_clock::now();
    std::string aCipherError;
    if (!aCipher->SealData(pBuffers.mSource, pBuffers.mWorker,
                           pBuffers.mDestination, pDataLength,
                           &aCipherError, pMode)) {
      SetError(pError,
               std::string("SealData failed for mode ") +
                   GetCryptModeName(pMode) +
                   (aCipherError.empty() ? "" : ": " + aCipherError));
      return false;
    }
    const auto aEnd = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::micro> aElapsed = aEnd - aStart;
    aTimes.push_back(aElapsed.count());
  }

  if (aTimes.size() <= static_cast<std::size_t>(DISCARD_LO + DISCARD_HI)) {
    SetError(pError, "not enough benchmark samples after trimming");
    return false;
  }

  std::sort(aTimes.begin(), aTimes.end());
  const auto aBegin = aTimes.begin() + DISCARD_LO;
  const auto aEnd = aTimes.end() - DISCARD_HI;
  const double aSum = std::accumulate(aBegin, aEnd, 0.0);
  pAverageUs = aSum / static_cast<double>(aEnd - aBegin);
  pGiBPerSecond =
      static_cast<double>(pDataLength) / (pAverageUs / 1000000.0) /
      (1024.0 * 1024.0 * 1024.0);
  return true;
}

std::string FormatTimeUsCompact(double pTimeUs) {
  std::ostringstream aTime;
  aTime << std::fixed << std::setprecision(1);
  if (pTimeUs >= 100000.0) {
    aTime << (pTimeUs / 1000.0) << "K us";
  } else {
    aTime << pTimeUs << " us";
  }
  return aTime.str();
}

const char* GetBuildFlavor() {
#if defined(NDEBUG)
  return "RELEASE";
#else
  return "DEBUG";
#endif
}

const char* GetWorstSeverity(double pWorstUs) {
  if (pWorstUs > 10000.0) {
    return "Nuclear!";
  }
  if (pWorstUs > 5000.0) {
    return "Critical!";
  }
  if (pWorstUs > 2500.0) {
    return "Danger!";
  }
  return "OK";
}

std::vector<std::string> RenderTable(const std::vector<CryptMode>& pModes,
                                     const std::vector<double>& pAveragesUs,
                                     const std::vector<double>& pGiBPerSecond,
                                     const std::string& pTitle) {
  std::optional<double> aSoftwareUs;
  for (std::size_t aIndex = 0; aIndex < pModes.size(); ++aIndex) {
    if (pModes[aIndex] == CryptMode::kNormal) {
      aSoftwareUs = pAveragesUs[aIndex];
      break;
    }
  }

  constexpr int kModeWidth = 6;
  constexpr int kTimeWidth = 11;
  constexpr int kPctWidth = 7;
  constexpr int kGibWidth = 10;
  std::vector<std::string> aLines;
  std::ostringstream aLine;
  aLine << std::left << pTitle;
  aLines.push_back(aLine.str());
  aLine.str("");
  aLine.clear();
  aLine << std::fixed << std::setprecision(1);
  aLine << "+"
        << std::string(kModeWidth + 2, '-')
        << "+"
        << std::string(kTimeWidth + 2, '-')
        << "+"
        << std::string(kPctWidth + 2, '-')
        << "+"
        << std::string(kGibWidth + 2, '-')
        << "+";
  aLines.push_back(aLine.str());
  aLine.str("");
  aLine.clear();
  aLine << "| " << std::left << std::setw(kModeWidth) << "mode"
        << " | " << std::right << std::setw(kTimeWidth) << "mean_time"
        << " | " << std::right << std::setw(kPctWidth) << "pct_sw"
        << " | " << std::right << std::setw(kGibWidth) << "gib_per_s"
        << " |";
  aLines.push_back(aLine.str());
  aLine.str("");
  aLine.clear();
  aLine << "+"
        << std::string(kModeWidth + 2, '-')
        << "+"
        << std::string(kTimeWidth + 2, '-')
        << "+"
        << std::string(kPctWidth + 2, '-')
        << "+"
        << std::string(kGibWidth + 2, '-')
        << "+";
  aLines.push_back(aLine.str());

  for (std::size_t aIndex = 0; aIndex < pModes.size(); ++aIndex) {
    double aPercent = 0.0;
    if (aSoftwareUs.has_value() && aSoftwareUs.value() > 0.0) {
      aPercent = (aSoftwareUs.value() / pAveragesUs[aIndex]) * 100.0;
    }

    std::string aModeName = GetCryptModeName(pModes[aIndex]);
    if (pModes[aIndex] == CryptMode::kNormal) {
      aModeName = "Soft";
      aPercent = 100.0;
    }

    const std::string aTimeString = FormatTimeUsCompact(pAveragesUs[aIndex]);
    aLine.str("");
    aLine.clear();
    aLine << std::fixed << std::setprecision(1);
    aLine << "| " << std::left << std::setw(kModeWidth) << aModeName
          << " | " << std::right << std::setw(kTimeWidth)
          << aTimeString << " | " << std::right
          << std::setw(kPctWidth) << aPercent << " | " << std::right
          << std::setw(kGibWidth) << pGiBPerSecond[aIndex] << " |";
    aLines.push_back(aLine.str());
  }
  aLine.str("");
  aLine.clear();
  aLine << "+"
        << std::string(kModeWidth + 2, '-')
        << "+"
        << std::string(kTimeWidth + 2, '-')
        << "+"
        << std::string(kPctWidth + 2, '-')
        << "+"
        << std::string(kGibWidth + 2, '-')
        << "+";
  aLines.push_back(aLine.str());
  return aLines;
}

void WriteSection(std::ofstream& pStream,
                  const std::vector<CryptMode>& pModes,
                  const SectionResult& pSection) {
  pStream << pSection.mLabel << " (" << pSection.mDataLength << " bytes)\n";
  if (pSection.mTrials.empty()) {
    pStream << "\n";
    return;
  }

  const std::vector<std::string> aLeft =
      RenderTable(pModes, pSection.mTrials[0].mAveragesUs,
                  pSection.mTrials[0].mGiBPerSecond, "Trial 1");
  const std::vector<std::string> aRight =
      pSection.mTrials.size() > 1
          ? RenderTable(pModes, pSection.mTrials[1].mAveragesUs,
                        pSection.mTrials[1].mGiBPerSecond, "Trial 2")
          : std::vector<std::string>();

  const std::size_t aLeftWidth =
      aLeft.empty() ? 0 : aLeft.front().size();
  const std::size_t aLineCount = std::max(aLeft.size(), aRight.size());
  for (std::size_t aIndex = 0; aIndex < aLineCount; ++aIndex) {
    const std::string aLeftLine = aIndex < aLeft.size() ? aLeft[aIndex] : "";
    const std::string aRightLine = aIndex < aRight.size() ? aRight[aIndex] : "";
    pStream << std::left << std::setw(static_cast<int>(aLeftWidth)) << aLeftLine;
    if (!aRight.empty()) {
      pStream << "   " << aRightLine;
    }
    pStream << "\n";
  }
  pStream << "\n";
}

bool WriteReport(const std::string& pOutputFile,
                 const std::vector<CryptMode>& pModes,
                 const std::vector<SectionResult>& pSections,
                 std::string* pError) {
  std::ofstream aStream(pOutputFile);
  if (!aStream.is_open()) {
    SetError(pError, "failed to open benchmark output file");
    return false;
  }

  double aWorstUs = 0.0;
  for (const SectionResult& aSection : pSections) {
    for (const SectionResult::TrialResult& aTrial : aSection.mTrials) {
      for (double aAverageUs : aTrial.mAveragesUs) {
        if (aAverageUs > aWorstUs) {
          aWorstUs = aAverageUs;
        }
      }
    }
  }

  aStream << "[" << GetBuildFlavor() << "]"
          << " [" << FormatTimeUsCompact(aWorstUs) << "]"
          << " [" << GetWorstSeverity(aWorstUs) << "]\n\n";

  for (const SectionResult& aSection : pSections) {
    WriteSection(aStream, pModes, aSection);
  }

  return true;
}

}  // namespace

bool BenchmarkFlat(const CipherFactory& pFactory,
                   std::size_t pDataLength,
                   const std::string& pOutputFile,
                   std::string* pError) {
  if (pError != nullptr) {
    pError->clear();
  }
  if (!pFactory) {
    SetError(pError, "pFactory must not be empty");
    return false;
  }
  if ((pDataLength % BLOCK_GRANULARITY) != 0) {
    SetError(pError, "pDataLength must be a multiple of 192");
    return false;
  }

  std::vector<SectionResult> aSections;
  if (BLOCK_SIZE_L1 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L1 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L1;
    aSections.push_back(std::move(aSection));
  }
  if (BLOCK_SIZE_L2 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L2 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L2;
    aSections.push_back(std::move(aSection));
  }
  if (BLOCK_SIZE_L3 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L3 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L3;
    aSections.push_back(std::move(aSection));
  }
  if (aSections.empty()) {
    SectionResult aSection;
    aSection.mLabel = "Blocks";
    aSection.mDataLength = pDataLength;
    aSections.push_back(std::move(aSection));
  }

  const std::vector<CryptMode> aModes = GetAvailableCryptModes();
  for (SectionResult& aSection : aSections) {
    for (int aTrial = 0; aTrial < 2; ++aTrial) {
      BufferPair aBuffers;
      if (!AllocatePageAligned(aBuffers, aSection.mDataLength, pError)) {
        return false;
      }
      FillRandom(aBuffers.mSource, aSection.mDataLength);
      std::fill_n(aBuffers.mWorker, aBuffers.mAllocationLength, 0);
      std::fill_n(aBuffers.mDestination, aBuffers.mAllocationLength, 0);

      SectionResult::TrialResult aTrialResult;
      aTrialResult.mAveragesUs.reserve(aModes.size());
      aTrialResult.mGiBPerSecond.reserve(aModes.size());

      for (CryptMode aMode : aModes) {
        double aAverageUs = 0.0;
        double aGiB = 0.0;
        if (!TimeMode(pFactory, aBuffers, aSection.mDataLength, aMode,
                      aAverageUs, aGiB, pError)) {
          return false;
        }
        aTrialResult.mAveragesUs.push_back(aAverageUs);
        aTrialResult.mGiBPerSecond.push_back(aGiB);
      }

      aSection.mTrials.push_back(std::move(aTrialResult));
    }
  }

  return WriteReport(pOutputFile, aModes, aSections, pError);
}

bool BenchmarkFlat(const SizedCipherFactory& pFactory,
                   std::size_t pDataLength,
                   const std::string& pOutputFile,
                   std::string* pError) {
  if (pError != nullptr) {
    pError->clear();
  }
  if (!pFactory) {
    SetError(pError, "pFactory must not be empty");
    return false;
  }
  if ((pDataLength % BLOCK_GRANULARITY) != 0) {
    SetError(pError, "pDataLength must be a multiple of 192");
    return false;
  }

  std::vector<SectionResult> aSections;
  if (BLOCK_SIZE_L1 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L1 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L1;
    aSections.push_back(std::move(aSection));
  }
  if (BLOCK_SIZE_L2 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L2 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L2;
    aSections.push_back(std::move(aSection));
  }
  if (BLOCK_SIZE_L3 <= pDataLength) {
    SectionResult aSection;
    aSection.mLabel = "L3 Blocks";
    aSection.mDataLength = BLOCK_SIZE_L3;
    aSections.push_back(std::move(aSection));
  }
  if (aSections.empty()) {
    SectionResult aSection;
    aSection.mLabel = "Blocks";
    aSection.mDataLength = pDataLength;
    aSections.push_back(std::move(aSection));
  }

  const std::vector<CryptMode> aModes = GetAvailableCryptModes();
  for (SectionResult& aSection : aSections) {
    for (int aTrial = 0; aTrial < 2; ++aTrial) {
      BufferPair aBuffers;
      if (!AllocatePageAligned(aBuffers, aSection.mDataLength, pError)) {
        return false;
      }
      FillRandom(aBuffers.mSource, aSection.mDataLength);
      std::fill_n(aBuffers.mWorker, aBuffers.mAllocationLength, 0);
      std::fill_n(aBuffers.mDestination, aBuffers.mAllocationLength, 0);

      SectionResult::TrialResult aTrialResult;
      aTrialResult.mAveragesUs.reserve(aModes.size());
      aTrialResult.mGiBPerSecond.reserve(aModes.size());

      for (CryptMode aMode : aModes) {
        double aAverageUs = 0.0;
        double aGiB = 0.0;
        if (!TimeMode(pFactory, aBuffers, aSection.mDataLength, aMode,
                      aAverageUs, aGiB, pError)) {
          return false;
        }
        aTrialResult.mAveragesUs.push_back(aAverageUs);
        aTrialResult.mGiBPerSecond.push_back(aGiB);
      }

      aSection.mTrials.push_back(std::move(aTrialResult));
    }
  }

  return WriteReport(pOutputFile, aModes, aSections, pError);
}

}  // namespace peanutbutter::benchmark
