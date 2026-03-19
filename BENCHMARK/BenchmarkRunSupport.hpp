#ifndef JELLY_BENCHMARK_BENCHMARK_RUN_SUPPORT_HPP_
#define JELLY_BENCHMARK_BENCHMARK_RUN_SUPPORT_HPP_

#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>

#include "BenchmarkBlockExecutor.hpp"
#include "BenchmarkFlatExecutor.hpp"
#include "../src/PeanutButter.hpp"

namespace peanutbutter::benchmark {

inline std::string TimestampForLogs() {
  const std::time_t aNow = std::time(nullptr);
  std::tm aLocalTime{};
#if defined(_WIN32)
  localtime_s(&aLocalTime, &aNow);
#else
  localtime_r(&aNow, &aLocalTime);
#endif

  char aBuffer[32];
  std::strftime(aBuffer, sizeof(aBuffer), "%Y_%m_%d_%H_%M", &aLocalTime);
  return aBuffer;
}

inline std::string LogPathFor(const std::string& pStem) {
  std::filesystem::create_directories("BENCHMARK_LOGS");
  return "BENCHMARK_LOGS/" + pStem + "_" + TimestampForLogs() + ".txt";
}

inline int RunFlatBenchmark(const std::string& pStem,
                            const CipherFactory& pFactory) {
  std::string aError;
  const std::string aPath = LogPathFor(pStem);
  if (!BenchmarkFlat(pFactory, BLOCK_SIZE_L3, aPath, &aError)) {
    std::cerr << aError << "\n";
    return 1;
  }
  std::cout << "PASS " << pStem << " -> " << aPath << "\n";
  return 0;
}

inline int RunFlatBenchmark(const std::string& pStem,
                            const SizedCipherFactory& pFactory) {
  std::string aError;
  const std::string aPath = LogPathFor(pStem);
  if (!BenchmarkFlat(pFactory, BLOCK_SIZE_L3, aPath, &aError)) {
    std::cerr << aError << "\n";
    return 1;
  }
  std::cout << "PASS " << pStem << " -> " << aPath << "\n";
  return 0;
}

template <typename tRun>
int RunForEveryBlockSize(const std::string& pStemPrefix, tRun pRun) {
  const int aBlockSizes[] = {static_cast<int>(8),
                             static_cast<int>(12),
                             static_cast<int>(16),
                             static_cast<int>(24),
                             static_cast<int>(32),
                             static_cast<int>(48)};

  for (int aBlockSize : aBlockSizes) {
    const std::string aStem =
        pStemPrefix + "_" + std::to_string(aBlockSize);
    if (pRun(aBlockSize, aStem) != 0) {
      return 1;
    }
  }
  return 0;
}

inline int RunBlockBenchmark(const std::string& pStem,
                             int pBlockSize,
                             const BlockCipherFactory& pFactory) {
  std::string aError;
  const std::string aPath = LogPathFor(pStem);
  if (!BenchmarkBlock(pFactory, pBlockSize, BLOCK_SIZE_L3, aPath,
                      &aError)) {
    std::cerr << pStem << "\n" << aError << "\n";
    return 1;
  }
  std::cout << "PASS " << pStem << " -> " << aPath << "\n";
  return 0;
}

}  // namespace peanutbutter::benchmark

#endif  // JELLY_BENCHMARK_BENCHMARK_RUN_SUPPORT_HPP_
