#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "../src/Core/MaskedPassword.hpp"

namespace {

constexpr std::size_t kLength = 48;
constexpr std::size_t kCasesPerMask = 20;

void FillRandom(std::vector<unsigned char>& pBytes, std::mt19937_64& pRng) {
  std::uniform_int_distribution<int> aDist(0, 255);
  for (unsigned char& aByte : pBytes) {
    aByte = static_cast<unsigned char>(aDist(pRng));
  }
}

}  // namespace

int main() {
  std::random_device aSeed;
  const std::uint64_t aSeedValue =
      (static_cast<std::uint64_t>(aSeed()) << 32) ^
      static_cast<std::uint64_t>(aSeed());
  std::mt19937_64 aRng(aSeedValue);
  std::vector<unsigned char> aBytes(kLength);
  std::vector<unsigned char> aOriginal(kLength);
  std::vector<unsigned char> aPassword(kLength);
  std::vector<unsigned char> aWorker(kLength);
  std::vector<unsigned char> aMaskArray(kLength);

  std::size_t aTotal = 0;
  for (int aMask = 0; aMask <= 255; ++aMask) {
    for (std::size_t aCase = 0; aCase < kCasesPerMask; ++aCase) {
      FillRandom(aBytes, aRng);
      FillRandom(aPassword, aRng);
      aOriginal = aBytes;

      if (!jelly::MaskedPassword(aBytes.data(), aPassword.data(), aWorker.data(),
                                 static_cast<unsigned char>(aMask), kLength)) {
        std::cerr << "MaskedPassword forward failed at mask=0x"
                  << std::hex << std::setw(2) << std::setfill('0') << aMask
                  << std::dec << " case=" << aCase << "\n";
        return 1;
      }

      if (!jelly::MaskedPassword(aBytes.data(), aPassword.data(), aWorker.data(),
                                 static_cast<unsigned char>(aMask), kLength)) {
        std::cerr << "MaskedPassword backward failed at mask=0x"
                  << std::hex << std::setw(2) << std::setfill('0') << aMask
                  << std::dec << " case=" << aCase << "\n";
        return 1;
      }

      if (aBytes != aOriginal) {
        std::cerr << "Round-trip mismatch at mask=0x" << std::hex
                  << std::setw(2) << std::setfill('0') << aMask << std::dec
                  << " case=" << aCase << "\n";
        return 1;
      }
      ++aTotal;

      aBytes = aOriginal;
      std::fill(aMaskArray.begin(), aMaskArray.end(),
                static_cast<unsigned char>(aMask));

      if (!jelly::MaskPasswordAray(aBytes.data(), aPassword.data(),
                                   aWorker.data(), aMaskArray.data(),
                                   kLength)) {
        std::cerr << "MaskPasswordAray forward failed at mask=0x"
                  << std::hex << std::setw(2) << std::setfill('0') << aMask
                  << std::dec << " case=" << aCase << "\n";
        return 1;
      }

      if (!jelly::MaskPasswordAray(aBytes.data(), aPassword.data(),
                                   aWorker.data(), aMaskArray.data(),
                                   kLength)) {
        std::cerr << "MaskPasswordAray backward failed at mask=0x"
                  << std::hex << std::setw(2) << std::setfill('0') << aMask
                  << std::dec << " case=" << aCase << "\n";
        return 1;
      }

      if (aBytes != aOriginal) {
        std::cerr << "MaskPasswordAray round-trip mismatch at mask=0x"
                  << std::hex << std::setw(2) << std::setfill('0') << aMask
                  << std::dec << " case=" << aCase << "\n";
        return 1;
      }
      ++aTotal;
    }
  }

  std::cout << "TestMaskedPassword passed (" << aTotal << " cases)\n";
  return 0;
}
