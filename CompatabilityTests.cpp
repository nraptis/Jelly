#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "src/Encryption/Ciphers/Cascade/CascadeJumpXORCipher.hpp"
#include "src/Encryption/Ciphers/Cascade/CascadeXORCipher.hpp"
#include "src/Encryption/Ciphers/Invert/InvertCipher.hpp"
#include "src/Encryption/Ciphers/Invert/InvertMaskCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordAddCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordJumpAddCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordJumpSubtractCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordJumpXORCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordSubtractCipher.hpp"
#include "src/Encryption/Ciphers/Password/PasswordXORCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperAddCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperDualJumpNoiseXORCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperJumpNoiseXORCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperJumpXORCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperNoiseXORCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperSubtractCipher.hpp"
#include "src/Encryption/Ciphers/Pepper/PepperXORCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseBlockByteCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseBlockCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseMaskBlockCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseMaskByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Reverse/ReverseMaskCipher.hpp"
#include "src/Encryption/Ciphers/Ripple/RippleBlockCipher.hpp"
#include "src/Encryption/Ciphers/Ripple/RippleCipher.hpp"
#include "src/Encryption/Ciphers/Ripple/RippleMaskBlockCipher.hpp"
#include "src/Encryption/Ciphers/Ripple/RippleMaskCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateBlockByteCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateBlockCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateMaskBlockCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateMaskByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Rotation/RotateMaskCipher.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridH16.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridH64.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskH16.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskH64.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskV16.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridMaskV64.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridV16.hpp"
#include "src/Encryption/Ciphers/SpiralGrid/SpiralGridV64.hpp"
#include "src/Encryption/Ciphers/Splint/SplintBlockCipher.hpp"
#include "src/Encryption/Ciphers/Splint/SplintByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Splint/SplintCipher.hpp"
#include "src/Encryption/Ciphers/Splint/SplintMaskBlockCipher.hpp"
#include "src/Encryption/Ciphers/Splint/SplintMaskByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Splint/SplintMaskCipher.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridHH16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridHH64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridHV16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridHV64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskHH16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskHH64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskHV16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskHV64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskVH16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskVH64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskVV16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridMaskVV64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridVH16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridVH64.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridVV16.hpp"
#include "src/Encryption/Ciphers/SwapGrid/SwapGridVV64.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveBlockCipher.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveCipher.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveMaskBlockCipher.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveMaskByteBlockCipher.hpp"
#include "src/Encryption/Ciphers/Weave/WeaveMaskCipher.hpp"
#include "src/Encryption/CryptMode.hpp"
#include "src/Encryption/Crypt.hpp"
#include "src/PeanutButter.hpp"
#include "src/Tables/Tables.hpp"

namespace {

constexpr std::size_t kGuardSize = 64u;
constexpr std::size_t kBlockSizes[] = {8u, 12u, 16u, 24u, 32u, 48u};
constexpr std::size_t kParamLengths[] = {16u, 32u, 48u, 192u, BLOCK_SIZE_L1};

using TableDescriptor = peanutbutter::tables::Tables::TableDescriptor;
using CipherBuilder = std::function<std::unique_ptr<peanutbutter::Crypt>()>;

struct NamedCipherFactory {
  std::string mName;
  CipherBuilder mBuild;
};

struct GuardedBuffer {
  explicit GuardedBuffer(std::size_t pLength, unsigned char pFill)
      : mStorage(pLength + (kGuardSize * 2u), pFill),
        mLength(pLength) {}

  unsigned char* Data() { return mStorage.data() + kGuardSize; }
  const unsigned char* Data() const { return mStorage.data() + kGuardSize; }

  bool GuardsIntact(unsigned char pFill) const {
    for (std::size_t aIndex = 0; aIndex < kGuardSize; ++aIndex) {
      if (mStorage[aIndex] != pFill) {
        return false;
      }
      if (mStorage[kGuardSize + mLength + aIndex] != pFill) {
        return false;
      }
    }
    return true;
  }

  std::vector<unsigned char> mStorage;
  std::size_t mLength = 0u;
};

const TableDescriptor& SelectTableWithMinSize(std::size_t pStartIndex,
                                              std::size_t pMinimumLength) {
  const auto& aTables = peanutbutter::tables::Tables::All();
  for (std::size_t aOffset = 0; aOffset < aTables.size(); ++aOffset) {
    const TableDescriptor& aTable =
        aTables[(pStartIndex + aOffset) % aTables.size()];
    if (aTable.mSize >= pMinimumLength) {
      return aTable;
    }
  }
  return aTables.front();
}

std::string FormatFailurePrefix(const std::string& pCipherName,
                                const TableDescriptor& pSourceTable,
                                peanutbutter::CryptMode pMode) {
  std::ostringstream aStream;
  aStream << pCipherName << " source=" << pSourceTable.mName
          << " length=" << pSourceTable.mSize
          << " mode=" << peanutbutter::GetCryptModeName(pMode);
  return aStream.str();
}

bool RunRoundTrip(const std::string& pCipherName,
                  const TableDescriptor& pSourceTable,
                  peanutbutter::Crypt& pCipher,
                  std::string* pError,
                  std::size_t& pCaseCount) {
  for (peanutbutter::CryptMode aMode : peanutbutter::GetAvailableCryptModes()) {
    GuardedBuffer aSource(pSourceTable.mSize, 0xA5u);
    GuardedBuffer aWorkerSeal(pSourceTable.mSize, 0xB6u);
    GuardedBuffer aWorkerUnseal(pSourceTable.mSize, 0xC7u);
    GuardedBuffer aEncrypted(pSourceTable.mSize, 0xD8u);
    GuardedBuffer aRoundTrip(pSourceTable.mSize, 0xE9u);

    std::memcpy(aSource.Data(), pSourceTable.mData, pSourceTable.mSize);
    std::vector<unsigned char> aOriginal(aSource.Data(),
                                         aSource.Data() + pSourceTable.mSize);

    std::string aCipherError;
    if (!pCipher.SealData(aSource.Data(), aWorkerSeal.Data(), aEncrypted.Data(),
                          pSourceTable.mSize, &aCipherError, aMode)) {
      *pError = FormatFailurePrefix(pCipherName, pSourceTable, aMode) +
                " seal failed" +
                (aCipherError.empty() ? std::string() : ": " + aCipherError);
      return false;
    }

    if (!aSource.GuardsIntact(0xA5u) || !aWorkerSeal.GuardsIntact(0xB6u) ||
        !aEncrypted.GuardsIntact(0xD8u)) {
      *pError = FormatFailurePrefix(pCipherName, pSourceTable, aMode) +
                " guard overwrite during seal";
      return false;
    }
    if (!std::equal(aOriginal.begin(), aOriginal.end(), aSource.Data())) {
      *pError = FormatFailurePrefix(pCipherName, pSourceTable, aMode) +
                " modified source during seal";
      return false;
    }

    aCipherError.clear();
    if (!pCipher.UnsealData(aEncrypted.Data(), aWorkerUnseal.Data(),
                            aRoundTrip.Data(), pSourceTable.mSize, &aCipherError,
                            aMode)) {
      *pError = FormatFailurePrefix(pCipherName, pSourceTable, aMode) +
                " unseal failed" +
                (aCipherError.empty() ? std::string() : ": " + aCipherError);
      return false;
    }

    if (!aEncrypted.GuardsIntact(0xD8u) || !aWorkerUnseal.GuardsIntact(0xC7u) ||
        !aRoundTrip.GuardsIntact(0xE9u)) {
      *pError = FormatFailurePrefix(pCipherName, pSourceTable, aMode) +
                " guard overwrite during unseal";
      return false;
    }

    if (!std::equal(aOriginal.begin(), aOriginal.end(), aRoundTrip.Data())) {
      std::size_t aMismatchIndex = 0u;
      while (aMismatchIndex < aOriginal.size() &&
             aOriginal[aMismatchIndex] == aRoundTrip.Data()[aMismatchIndex]) {
        ++aMismatchIndex;
      }
      std::ostringstream aStream;
      aStream << FormatFailurePrefix(pCipherName, pSourceTable, aMode)
              << " round-trip mismatch at index " << aMismatchIndex;
      if (aMismatchIndex < aOriginal.size()) {
        aStream << " input=0x" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(aOriginal[aMismatchIndex]) << " output=0x"
                << std::setw(2)
                << static_cast<int>(aRoundTrip.Data()[aMismatchIndex]);
      }
      *pError = aStream.str();
      return false;
    }

    ++pCaseCount;
  }
  return true;
}

void AddFixedCipherFactories(std::vector<NamedCipherFactory>& pFactories) {
  pFactories.push_back({"RotateCipher",
                        [] { return std::make_unique<peanutbutter::RotateCipher>(-16); }});
  for (std::size_t aBlockSize : kBlockSizes) {
    pFactories.push_back(
        {"RotateBlockCipher_" + std::to_string(aBlockSize),
         [aBlockSize] {
           return std::make_unique<peanutbutter::RotateBlockCipher>(aBlockSize,
                                                                    -16);
         }});
    pFactories.push_back(
        {"RotateBlockByteCipher_" + std::to_string(aBlockSize),
         [aBlockSize] {
           return std::make_unique<peanutbutter::RotateBlockByteCipher>(
               aBlockSize, -16);
         }});
  }
  pFactories.push_back({"RotateMaskCipher",
                        [] { return std::make_unique<peanutbutter::RotateMaskCipher>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_8",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher08>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_12",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher12>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_16",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher16>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_24",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher24>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_32",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher32>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskBlockCipher_48",
                        [] { return std::make_unique<peanutbutter::RotateMaskBlockCipher48>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_8",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<8>>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_12",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<12>>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_16",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<16>>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_24",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<24>>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_32",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<32>>(0xDEu, -16); }});
  pFactories.push_back({"RotateMaskByteBlockCipher_48",
                        [] { return std::make_unique<peanutbutter::RotateMaskByteBlockCipher<48>>(0xDEu, -16); }});

  pFactories.push_back({"SplintCipher",
                        [] { return std::make_unique<peanutbutter::SplintCipher>(); }});
  for (std::size_t aBlockSize : kBlockSizes) {
    pFactories.push_back({"SplintBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::SplintBlockCipher>(aBlockSize);
                          }});
    pFactories.push_back({"SplintByteBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::SplintByteBlockCipher>(aBlockSize);
                          }});
    pFactories.push_back({"SplintMaskBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::SplintMaskBlockCipher>(aBlockSize, 0xA3u);
                          }});
    pFactories.push_back({"SplintMaskByteBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::SplintMaskByteBlockCipher>(aBlockSize, 0xA3u);
                          }});
  }
  pFactories.push_back({"SplintMaskCipher",
                        [] { return std::make_unique<peanutbutter::SplintMaskCipher>(0xA3u); }});

  pFactories.push_back({"WeaveCipher",
                        [] { return std::make_unique<peanutbutter::WeaveCipher>(3, 1, 2); }});
  for (std::size_t aBlockSize : kBlockSizes) {
    pFactories.push_back({"WeaveBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::WeaveBlockCipher>(aBlockSize, 3, 1, 2);
                          }});
    pFactories.push_back({"WeaveByteBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::WeaveByteBlockCipher>(aBlockSize, 3, 1, 2);
                          }});
    pFactories.push_back({"WeaveMaskBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::WeaveMaskBlockCipher>(aBlockSize, 0x3Eu, 3, 1, 2);
                          }});
    pFactories.push_back({"WeaveMaskByteBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::WeaveMaskByteBlockCipher>(aBlockSize, 0x3Eu, 3, 1, 2);
                          }});
  }
  pFactories.push_back({"WeaveMaskCipher",
                        [] { return std::make_unique<peanutbutter::WeaveMaskCipher>(0x3Eu, 3, 1, 2); }});

  pFactories.push_back({"ReverseCipher",
                        [] { return std::make_unique<peanutbutter::ReverseCipher>(); }});
  pFactories.push_back({"ReverseBlockCipher_8",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher08>(); }});
  pFactories.push_back({"ReverseBlockCipher_12",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher12>(); }});
  pFactories.push_back({"ReverseBlockCipher_16",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher16>(); }});
  pFactories.push_back({"ReverseBlockCipher_24",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher24>(); }});
  pFactories.push_back({"ReverseBlockCipher_32",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher32>(); }});
  pFactories.push_back({"ReverseBlockCipher_48",
                        [] { return std::make_unique<peanutbutter::ReverseBlockCipher48>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_8",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher08>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_12",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher12>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_16",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher16>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_24",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher24>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_32",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher32>(); }});
  pFactories.push_back({"ReverseBlockByteCipher_48",
                        [] { return std::make_unique<peanutbutter::ReverseBlockByteCipher48>(); }});
  pFactories.push_back({"ReverseMaskCipher",
                        [] { return std::make_unique<peanutbutter::ReverseMaskCipher>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_8",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher08>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_12",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher12>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_16",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher16>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_24",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher24>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_32",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher32>(0xA3u); }});
  pFactories.push_back({"ReverseMaskBlockCipher_48",
                        [] { return std::make_unique<peanutbutter::ReverseMaskBlockCipher48>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_8",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher08>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_12",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher12>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_16",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher16>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_24",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher24>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_32",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher32>(0xA3u); }});
  pFactories.push_back({"ReverseMaskByteBlockCipher_48",
                        [] { return std::make_unique<peanutbutter::ReverseMaskByteBlockCipher48>(0xA3u); }});

  pFactories.push_back({"RippleCipher",
                        [] { return std::make_unique<peanutbutter::RippleCipher>(4); }});
  pFactories.push_back({"RippleMaskCipher",
                        [] { return std::make_unique<peanutbutter::RippleMaskCipher>(0xA3u, 4); }});
  for (std::size_t aBlockSize : kBlockSizes) {
    pFactories.push_back({"RippleBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::RippleBlockCipher>(aBlockSize, 4);
                          }});
    pFactories.push_back({"RippleMaskBlockCipher_" + std::to_string(aBlockSize),
                          [aBlockSize] {
                            return std::make_unique<peanutbutter::RippleMaskBlockCipher>(aBlockSize, 0xA3u, 4);
                          }});
  }

  pFactories.push_back({"InvertCipher",
                        [] { return std::make_unique<peanutbutter::InvertCipher>(); }});
  pFactories.push_back({"InvertMaskCipher",
                        [] { return std::make_unique<peanutbutter::InvertMaskCipher>(0xA3u); }});

  pFactories.push_back({"SwapGridVV16",
                        [] { return std::make_unique<peanutbutter::SwapGridVV16>(); }});
  pFactories.push_back({"SwapGridVH16",
                        [] { return std::make_unique<peanutbutter::SwapGridVH16>(); }});
  pFactories.push_back({"SwapGridHV16",
                        [] { return std::make_unique<peanutbutter::SwapGridHV16>(); }});
  pFactories.push_back({"SwapGridHH16",
                        [] { return std::make_unique<peanutbutter::SwapGridHH16>(); }});
  pFactories.push_back({"SwapGridVV64",
                        [] { return std::make_unique<peanutbutter::SwapGridVV64>(); }});
  pFactories.push_back({"SwapGridVH64",
                        [] { return std::make_unique<peanutbutter::SwapGridVH64>(); }});
  pFactories.push_back({"SwapGridHV64",
                        [] { return std::make_unique<peanutbutter::SwapGridHV64>(); }});
  pFactories.push_back({"SwapGridHH64",
                        [] { return std::make_unique<peanutbutter::SwapGridHH64>(); }});
  pFactories.push_back({"SwapGridMaskVV16",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskVV16>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskVH16",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskVH16>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskHV16",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskHV16>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskHH16",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskHH16>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskVV64",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskVV64>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskVH64",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskVH64>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskHV64",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskHV64>(0xA3u); }});
  pFactories.push_back({"SwapGridMaskHH64",
                        [] { return std::make_unique<peanutbutter::SwapGridMaskHH64>(0xA3u); }});

  pFactories.push_back({"SpiralGridH16",
                        [] { return std::make_unique<peanutbutter::SpiralGridH16>(2); }});
  pFactories.push_back({"SpiralGridV16",
                        [] { return std::make_unique<peanutbutter::SpiralGridV16>(2); }});
  pFactories.push_back({"SpiralGridH64",
                        [] { return std::make_unique<peanutbutter::SpiralGridH64>(5); }});
  pFactories.push_back({"SpiralGridV64",
                        [] { return std::make_unique<peanutbutter::SpiralGridV64>(5); }});
  pFactories.push_back({"SpiralGridMaskH16",
                        [] { return std::make_unique<peanutbutter::SpiralGridMaskH16>(0xA3u, 2); }});
  pFactories.push_back({"SpiralGridMaskV16",
                        [] { return std::make_unique<peanutbutter::SpiralGridMaskV16>(0xA3u, 2); }});
  pFactories.push_back({"SpiralGridMaskH64",
                        [] { return std::make_unique<peanutbutter::SpiralGridMaskH64>(0xA3u, 5); }});
  pFactories.push_back({"SpiralGridMaskV64",
                        [] { return std::make_unique<peanutbutter::SpiralGridMaskV64>(0xA3u, 5); }});
}

bool RunFixedCipherSweep(std::size_t pIterations,
                         std::size_t& pCaseCount,
                         std::string* pError) {
  std::vector<NamedCipherFactory> aFactories;
  AddFixedCipherFactories(aFactories);
  const auto& aTables = peanutbutter::tables::Tables::All();
  for (std::size_t aIteration = 0; aIteration < pIterations; ++aIteration) {
    peanutbutter::tables::Tables::Seed(0x600D0000u +
                                       static_cast<std::uint32_t>(aIteration));
    for (const auto& aTable : aTables) {
      for (const auto& aFactory : aFactories) {
        std::unique_ptr<peanutbutter::Crypt> aCipher = aFactory.mBuild();
        if (!RunRoundTrip(aFactory.mName, aTable, *aCipher, pError, pCaseCount)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool RunVariableCipherSweep(std::size_t pIterations,
                            std::size_t& pCaseCount,
                            std::string* pError) {
  const auto& aTables = peanutbutter::tables::Tables::All();

  for (std::size_t aIteration = 0; aIteration < pIterations; ++aIteration) {
    peanutbutter::tables::Tables::Seed(0x700D0000u +
                                       static_cast<std::uint32_t>(aIteration));
    for (std::size_t aSourceIndex = 0; aSourceIndex < aTables.size(); ++aSourceIndex) {
      const auto& aSource = aTables[aSourceIndex];

      for (std::size_t aPasswordLength : kParamLengths) {
        const auto& aPasswordTable =
            SelectTableWithMinSize(aSourceIndex + 1u, aPasswordLength);
        {
          auto aCipher = std::make_unique<peanutbutter::PasswordAddCipher>(
              aPasswordTable.mData, static_cast<int>(aPasswordLength));
          if (!RunRoundTrip("PasswordAddCipher(password=" +
                                std::to_string(aPasswordLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }
        {
          auto aCipher =
              std::make_unique<peanutbutter::PasswordSubtractCipher>(
                  aPasswordTable.mData, static_cast<int>(aPasswordLength));
          if (!RunRoundTrip("PasswordSubtractCipher(password=" +
                                std::to_string(aPasswordLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }
        {
          auto aCipher = std::make_unique<peanutbutter::PasswordXORCipher>(
              aPasswordTable.mData, static_cast<int>(aPasswordLength));
          if (!RunRoundTrip("PasswordXORCipher(password=" +
                                std::to_string(aPasswordLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }

        for (std::size_t aJumpLength : kParamLengths) {
          const auto& aJumpTable =
              SelectTableWithMinSize(aSourceIndex + 2u, aJumpLength);
          {
            auto aCipher = std::make_unique<peanutbutter::PasswordJumpAddCipher>(
                aPasswordTable.mData, static_cast<int>(aPasswordLength),
                aJumpTable.mData, static_cast<int>(aJumpLength));
            if (!RunRoundTrip("PasswordJumpAddCipher(password=" +
                                  std::to_string(aPasswordLength) + ",jump=" +
                                  std::to_string(aJumpLength) + ")",
                              aSource, *aCipher, pError, pCaseCount)) {
              return false;
            }
          }
          {
            auto aCipher =
                std::make_unique<peanutbutter::PasswordJumpSubtractCipher>(
                    aPasswordTable.mData, static_cast<int>(aPasswordLength),
                    aJumpTable.mData, static_cast<int>(aJumpLength));
            if (!RunRoundTrip("PasswordJumpSubtractCipher(password=" +
                                  std::to_string(aPasswordLength) + ",jump=" +
                                  std::to_string(aJumpLength) + ")",
                              aSource, *aCipher, pError, pCaseCount)) {
              return false;
            }
          }
          {
            auto aCipher = std::make_unique<peanutbutter::PasswordJumpXORCipher>(
                aPasswordTable.mData, static_cast<int>(aPasswordLength),
                aJumpTable.mData, static_cast<int>(aJumpLength));
            if (!RunRoundTrip("PasswordJumpXORCipher(password=" +
                                  std::to_string(aPasswordLength) + ",jump=" +
                                  std::to_string(aJumpLength) + ")",
                              aSource, *aCipher, pError, pCaseCount)) {
              return false;
            }
          }
        }
      }

      for (std::size_t aMaskLength : kParamLengths) {
        const auto& aMaskTable =
            SelectTableWithMinSize(aSourceIndex + 3u, aMaskLength);
        {
          auto aCipher = std::make_unique<peanutbutter::PepperAddCipher>(
              aMaskTable.mData, aMaskLength);
          if (!RunRoundTrip("PepperAddCipher(mask=" +
                                std::to_string(aMaskLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }
        {
          auto aCipher = std::make_unique<peanutbutter::PepperSubtractCipher>(
              aMaskTable.mData, aMaskLength);
          if (!RunRoundTrip("PepperSubtractCipher(mask=" +
                                std::to_string(aMaskLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }
        {
          auto aCipher = std::make_unique<peanutbutter::PepperXORCipher>(
              aMaskTable.mData, aMaskLength);
          if (!RunRoundTrip("PepperXORCipher(mask=" +
                                std::to_string(aMaskLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }
        {
          auto aCipher = std::make_unique<peanutbutter::CascadeXORCipher>(
              aMaskTable.mData, static_cast<int>(aMaskLength));
          if (!RunRoundTrip("CascadeXORCipher(mask=" +
                                std::to_string(aMaskLength) + ")",
                            aSource, *aCipher, pError, pCaseCount)) {
            return false;
          }
        }

        for (std::size_t aNoiseLength : kParamLengths) {
          const auto& aNoiseTable =
              SelectTableWithMinSize(aSourceIndex + 4u, aNoiseLength);
          {
            auto aCipher = std::make_unique<peanutbutter::PepperNoiseXORCipher>(
                aMaskTable.mData, aMaskLength, aNoiseTable.mData, aNoiseLength);
            if (!RunRoundTrip("PepperNoiseXORCipher(mask=" +
                                  std::to_string(aMaskLength) + ",noise=" +
                                  std::to_string(aNoiseLength) + ")",
                              aSource, *aCipher, pError, pCaseCount)) {
              return false;
            }
          }

          for (std::size_t aJumpLength : kParamLengths) {
            const auto& aJumpTable =
                SelectTableWithMinSize(aSourceIndex + 5u, aJumpLength);
            {
              auto aCipher =
                  std::make_unique<peanutbutter::PepperJumpXORCipher>(
                      aMaskTable.mData, static_cast<int>(aMaskLength),
                      aJumpTable.mData, static_cast<int>(aJumpLength));
              if (!RunRoundTrip("PepperJumpXORCipher(mask=" +
                                    std::to_string(aMaskLength) + ",jump=" +
                                    std::to_string(aJumpLength) + ")",
                                aSource, *aCipher, pError, pCaseCount)) {
                return false;
              }
            }
            {
              auto aCipher =
                  std::make_unique<peanutbutter::CascadeJumpXORCipher>(
                      aMaskTable.mData, static_cast<int>(aMaskLength),
                      aJumpTable.mData, static_cast<int>(aJumpLength));
              if (!RunRoundTrip("CascadeJumpXORCipher(mask=" +
                                    std::to_string(aMaskLength) + ",jump=" +
                                    std::to_string(aJumpLength) + ")",
                                aSource, *aCipher, pError, pCaseCount)) {
                return false;
              }
            }
            {
              auto aCipher =
                  std::make_unique<peanutbutter::PepperJumpNoiseXORCipher>(
                      aMaskTable.mData, static_cast<int>(aMaskLength),
                      aNoiseTable.mData, static_cast<int>(aNoiseLength),
                      aJumpTable.mData, static_cast<int>(aJumpLength));
              if (!RunRoundTrip("PepperJumpNoiseXORCipher(mask=" +
                                    std::to_string(aMaskLength) + ",noise=" +
                                    std::to_string(aNoiseLength) + ",jump=" +
                                    std::to_string(aJumpLength) + ")",
                                aSource, *aCipher, pError, pCaseCount)) {
                return false;
              }
            }
            for (std::size_t aNoiseJumpLength : kParamLengths) {
              const auto& aNoiseJumpTable =
                  SelectTableWithMinSize(aSourceIndex + 6u, aNoiseJumpLength);
              auto aCipher =
                  std::make_unique<peanutbutter::PepperDualJumpNoiseXORCipher>(
                      aMaskTable.mData, static_cast<int>(aMaskLength),
                      aNoiseTable.mData, static_cast<int>(aNoiseLength),
                      aJumpTable.mData, static_cast<int>(aJumpLength),
                      aNoiseJumpTable.mData,
                      static_cast<int>(aNoiseJumpLength));
              if (!RunRoundTrip("PepperDualJumpNoiseXORCipher(mask=" +
                                    std::to_string(aMaskLength) + ",noise=" +
                                    std::to_string(aNoiseLength) +
                                    ",mask_jump=" +
                                    std::to_string(aJumpLength) +
                                    ",noise_jump=" +
                                    std::to_string(aNoiseJumpLength) + ")",
                                aSource, *aCipher, pError, pCaseCount)) {
                return false;
              }
            }
          }
        }
      }
    }
  }
  return true;
}

}  // namespace

int main(int pArgc, char** pArgv) {
  std::size_t aIterations = 1u;
  if (pArgc > 1) {
    const long aParsed = std::strtol(pArgv[1], nullptr, 10);
    if (aParsed > 0) {
      aIterations = static_cast<std::size_t>(aParsed);
    }
  }

  std::size_t aCaseCount = 0u;
  std::string aError;

  if (!RunFixedCipherSweep(aIterations, aCaseCount, &aError) ||
      !RunVariableCipherSweep(aIterations, aCaseCount, &aError)) {
    std::cerr << aError << "\n";
    return 1;
  }

  std::cout << "CompatabilityTests passed cases=" << aCaseCount
            << " iterations=" << aIterations << "\n";
  return 0;
}
