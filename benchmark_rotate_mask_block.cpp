#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "src/Core/LayerCakeCryptDelegate.hpp"
#include "src/Jelly.hpp"
#include "src/Ciphers/Rotation/RotateMaskBlockCipher.hpp"

namespace {

constexpr std::size_t kOneMiB = 1024 * 1024;
constexpr std::size_t kCommonAlignment = 96;
constexpr std::size_t kLength = (kOneMiB / kCommonAlignment) * kCommonAlignment;
constexpr std::uint8_t kMask = 0xDE;
constexpr int kShift = -16;
constexpr int kRuns = 50000;
constexpr int kTrimCount = kRuns / 10;

struct BenchmarkSummary {
  const char* label;
  std::uint64_t checksum;
  double avg_us;
  double gib_per_s;
};

std::vector<unsigned char> MakeGarbledData() {
  std::mt19937 rng(0xC0FFEEu);
  std::uniform_int_distribution<int> dist(0, 255);
  std::vector<unsigned char> data(kLength);
  for (unsigned char& byte : data) {
    byte = static_cast<unsigned char>(dist(rng));
  }
  return data;
}

std::vector<double> TimeCipherRuns(jelly::LayerCakeCryptDelegate& pCipher,
                                   const std::vector<unsigned char>& pSource,
                                   std::vector<unsigned char>& pDestination) {
  std::vector<double> times;
  times.reserve(kRuns);
  for (int run = 0; run < kRuns; ++run) {
    const auto start = std::chrono::steady_clock::now();
    if (!pCipher.SealData(pSource.data(), pDestination.data(), pSource.size(),
                          jelly::CryptMode::kNormal)) {
      return {};
    }
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::micro> elapsed = end - start;
    times.push_back(elapsed.count());
  }
  return times;
}

double MiddleEightyAverage(std::vector<double> pTimes) {
  if (pTimes.size() <= static_cast<std::size_t>(kTrimCount * 2)) {
    return -1.0;
  }
  std::sort(pTimes.begin(), pTimes.end());
  const auto begin = pTimes.begin() + kTrimCount;
  const auto end = pTimes.end() - kTrimCount;
  const double sum = std::accumulate(begin, end, 0.0);
  return sum / static_cast<double>(end - begin);
}

std::uint64_t Checksum(const std::vector<unsigned char>& pData) {
  std::uint64_t hash = 1469598103934665603ull;
  for (unsigned char byte : pData) {
    hash ^= static_cast<std::uint64_t>(byte);
    hash *= 1099511628211ull;
  }
  return hash;
}

template <std::size_t BlockSize>
BenchmarkSummary BenchmarkSize(const char* pLabel) {
  std::vector<unsigned char> source = MakeGarbledData();
  std::vector<unsigned char> destination(kLength);

  jelly::RotateMaskBlockCipher<BlockSize> cipher(kMask, kShift);

  if (!cipher.SealData(source.data(), destination.data(), source.size(),
                       jelly::CryptMode::kNormal)) {
    throw std::runtime_error("cipher failed");
  }

  std::vector<double> times = TimeCipherRuns(cipher, source, destination);
  if (times.empty()) {
    throw std::runtime_error("benchmark failed");
  }

  const double avg_us = MiddleEightyAverage(times);
  if (avg_us < 0.0) {
    throw std::runtime_error("average failed");
  }

  const double total_bytes = static_cast<double>(kLength);
  const double gib_per_s =
      total_bytes / (avg_us / 1000000.0) / (1024.0 * 1024.0 * 1024.0);

  return BenchmarkSummary{pLabel, Checksum(destination), avg_us, gib_per_s};
}

void PrintSummary(const BenchmarkSummary& pSummary) {
  std::cout << pSummary.label << '\n';
  std::cout << "  checksum=" << pSummary.checksum << '\n';
  std::cout << "  avg_us=" << pSummary.avg_us << '\n';
  std::cout << "  gib_per_s=" << pSummary.gib_per_s << '\n';
}

}  // namespace

int main() {
  std::cout << "bytes=" << kLength << '\n';
  std::cout << "runs=" << kRuns << '\n';
  std::cout << "middle_percent=80\n";
  std::cout << "trim_each_side=" << kTrimCount << '\n';
  std::cout << std::fixed << std::setprecision(3);

  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_02>("block_size=2"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_04>("block_size=4"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_08>("block_size=8"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_12>("block_size=12"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_16>("block_size=16"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_24>("block_size=24"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_32>("block_size=32"));
  PrintSummary(BenchmarkSize<jelly::EB_BLOCK_SIZE_48>("block_size=48"));

  return 0;
}
