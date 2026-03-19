#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "src/Encryption/Ciphers/Invert/InvertCipher.hpp"
#include "src/Encryption/Ciphers/Invert/InvertMaskCipher.hpp"
#include "src/Encryption/Ciphers/Cascade/CascadeJumpXORCipher.hpp"
#include "src/Encryption/Ciphers/Cascade/CascadeXORCipher.hpp"
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
#include "src/Encryption/LayeredCrypt.hpp"
#include "src/PeanutButter.hpp"

namespace {

constexpr std::size_t kBlockSizes[] = {
    8, 12, 16,
    24, 32, 48,
};

constexpr unsigned char kQuickSmokePassword[16] = {
    0x63u, 0x61u, 0x74u, 0x64u, 0x6fu, 0x67u, 0x43u, 0x41u,
    0x54u, 0x44u, 0x4fu, 0x47u, 0x31u, 0x32u, 0x33u, 0x34u,
};

constexpr unsigned char kQuickSmokeJumpPassword[64] = {
    0x63u, 0x61u, 0x74u, 0x64u, 0x6fu, 0x67u, 0x43u, 0x41u,
    0x54u, 0x44u, 0x4fu, 0x47u, 0x31u, 0x32u, 0x33u, 0x34u,
    0x55u, 0x66u, 0x77u, 0x88u, 0x99u, 0xaau, 0xbbu, 0xccu,
    0xddu, 0xeeu, 0xffu, 0x10u, 0x21u, 0x32u, 0x43u, 0x54u,
    0x01u, 0x23u, 0x45u, 0x67u, 0x89u, 0xabu, 0xcdu, 0xefu,
    0xf0u, 0xdeu, 0xbcu, 0x9au, 0x78u, 0x56u, 0x34u, 0x12u,
    0x0fu, 0x1eu, 0x2du, 0x3cu, 0x4bu, 0x5au, 0x69u, 0x78u,
    0x87u, 0x96u, 0xa5u, 0xb4u, 0xc3u, 0xd2u, 0xe1u, 0xf0u,
};

constexpr unsigned char kQuickSmokeJumps[16] = {
    0u, 1u, 3u, 2u, 1u, 0u, 2u, 3u, 4u, 1u, 0u, 2u, 1u, 3u, 0u, 1u,
};

std::vector<unsigned char> BuildPepperTable(std::size_t pLength,
                                            std::uint32_t pSeed,
                                            unsigned char pZeroFallback) {
  std::vector<unsigned char> aTable(pLength);
  std::uint32_t aState = pSeed;
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    aState ^= (aState << 13);
    aState ^= (aState >> 17);
    aState ^= (aState << 5);
    const unsigned char aValue = static_cast<unsigned char>(aState & 0xFFu);
    aTable[aIndex] = (aValue == 0u) ? pZeroFallback : aValue;
  }
  return aTable;
}

void AddEveryCipher(peanutbutter::EncryptionLayer& pLayer,
                    const std::vector<unsigned char>& pPepperMask,
                    const std::vector<unsigned char>& pPepperNoise) {
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateCipher>(-16));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<peanutbutter::RotateBlockCipher>(aBlockSize, -16));
    pLayer.AddCipher(
        std::make_unique<peanutbutter::RotateBlockByteCipher>(aBlockSize, -16));
  }
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskCipher>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher08>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher12>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher16>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher24>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher32>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<peanutbutter::RotateMaskBlockCipher48>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<8>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<12>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<16>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<24>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<32>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   peanutbutter::RotateMaskByteBlockCipher<48>>(
      0xDEu, -16));

  pLayer.AddCipher(std::make_unique<peanutbutter::SplintCipher>());
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<peanutbutter::SplintBlockCipher>(aBlockSize));
    pLayer.AddCipher(std::make_unique<peanutbutter::SplintByteBlockCipher>(aBlockSize));
  }
  pLayer.AddCipher(std::make_unique<peanutbutter::SplintMaskCipher>(0xA3u));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<peanutbutter::SplintMaskBlockCipher>(aBlockSize, 0xA3u));
    pLayer.AddCipher(
        std::make_unique<peanutbutter::SplintMaskByteBlockCipher>(aBlockSize, 0xA3u));
  }

  pLayer.AddCipher(std::make_unique<peanutbutter::WeaveCipher>(3, 1, 2));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<peanutbutter::WeaveBlockCipher>(aBlockSize, 3, 1, 2));
    pLayer.AddCipher(
        std::make_unique<peanutbutter::WeaveByteBlockCipher>(aBlockSize, 3, 1, 2));
  }
  pLayer.AddCipher(std::make_unique<peanutbutter::WeaveMaskCipher>(0x3Eu, 3, 1, 2));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<peanutbutter::WeaveMaskBlockCipher>(
        aBlockSize, 0x3Eu, 3, 1, 2));
    pLayer.AddCipher(std::make_unique<peanutbutter::WeaveMaskByteBlockCipher>(
        aBlockSize, 0x3Eu, 3, 1, 2));
  }

  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseCipher>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher08>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher12>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher24>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher32>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockCipher48>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher08>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher12>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher24>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher32>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseBlockByteCipher48>());
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskCipher>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher08>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher12>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher24>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher32>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskBlockCipher48>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher08>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher12>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher24>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher32>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::ReverseMaskByteBlockCipher48>(0xA3u));

  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordAddCipher>(
      kQuickSmokePassword, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordSubtractCipher>(
      kQuickSmokePassword, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordXORCipher>(
      kQuickSmokePassword, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordJumpAddCipher>(
      kQuickSmokeJumpPassword, 64, kQuickSmokeJumps, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordJumpSubtractCipher>(
      kQuickSmokeJumpPassword, 64, kQuickSmokeJumps, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PasswordJumpXORCipher>(
      kQuickSmokeJumpPassword, 64, kQuickSmokeJumps, 16));

  pLayer.AddCipher(std::make_unique<peanutbutter::RippleCipher>(4));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<peanutbutter::RippleBlockCipher>(aBlockSize, 4));
  }
  pLayer.AddCipher(std::make_unique<peanutbutter::RippleMaskCipher>(0xA3u, 4));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<peanutbutter::RippleMaskBlockCipher>(aBlockSize, 0xA3u, 4));
  }

  pLayer.AddCipher(std::make_unique<peanutbutter::InvertCipher>());
  pLayer.AddCipher(std::make_unique<peanutbutter::InvertMaskCipher>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperAddCipher>(
      pPepperMask.data(), pPepperMask.size()));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperSubtractCipher>(
      pPepperMask.data(), pPepperMask.size()));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperXORCipher>(
      pPepperMask.data(), pPepperMask.size()));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperJumpXORCipher>(
      pPepperMask.data(), static_cast<int>(pPepperMask.size()),
      kQuickSmokeJumps, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperNoiseXORCipher>(
      pPepperMask.data(), pPepperMask.size(), pPepperNoise.data(),
      pPepperNoise.size()));
  pLayer.AddCipher(std::make_unique<peanutbutter::PepperJumpNoiseXORCipher>(
      pPepperMask.data(), static_cast<int>(pPepperMask.size()),
      pPepperNoise.data(), static_cast<int>(pPepperNoise.size()),
      kQuickSmokeJumps, 16));
  pLayer.AddCipher(
      std::make_unique<peanutbutter::PepperDualJumpNoiseXORCipher>(
          pPepperMask.data(), static_cast<int>(pPepperMask.size()),
          pPepperNoise.data(), static_cast<int>(pPepperNoise.size()),
          kQuickSmokeJumps, 16, kQuickSmokeJumps, 16));
  pLayer.AddCipher(std::make_unique<peanutbutter::CascadeXORCipher>(
      pPepperMask.data(), static_cast<int>(pPepperMask.size())));
  pLayer.AddCipher(std::make_unique<peanutbutter::CascadeJumpXORCipher>(
      pPepperMask.data(), static_cast<int>(pPepperMask.size()),
      kQuickSmokeJumps, 16));

  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridVV16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridVH16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridHV16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridHH16>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridVV64>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridVH64>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridHV64>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridHH64>());
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskVV16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskVH16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskHV16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskHH16>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskVV64>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskVH64>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskHV64>(0xA3u));
  pLayer.AddCipher(std::make_unique<peanutbutter::SwapGridMaskHH64>(0xA3u));

  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridH16>(2));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridV16>(2));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridH64>(5));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridV64>(5));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridMaskH16>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridMaskV16>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridMaskH64>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<peanutbutter::SpiralGridMaskV64>(0xA3u, 2));
}

void FillInput(std::vector<unsigned char>& pData) {
  std::uint32_t aState = 0xC001D00Du;
  for (std::size_t aIndex = 0; aIndex < pData.size(); ++aIndex) {
    aState ^= (aState << 13);
    aState ^= (aState >> 17);
    aState ^= (aState << 5);
    pData[aIndex] = static_cast<unsigned char>(aState & 0xFFu);
  }
}

std::size_t FindMismatch(const std::vector<unsigned char>& pLeft,
                         const std::vector<unsigned char>& pRight) {
  for (std::size_t aIndex = 0; aIndex < pLeft.size(); ++aIndex) {
    if (pLeft[aIndex] != pRight[aIndex]) {
      return aIndex;
    }
  }
  return pLeft.size();
}

bool RoundTrip(peanutbutter::CryptMode pMode,
               const peanutbutter::LayeredCrypt& pCrypt) {
  std::vector<unsigned char> aInput(BLOCK_SIZE_L3);
  std::vector<unsigned char> aWorker(BLOCK_SIZE_L3);
  std::vector<unsigned char> aEncrypted(BLOCK_SIZE_L3);
  std::vector<unsigned char> aRoundTrip(BLOCK_SIZE_L3);
  std::string aErrorMessage;

  FillInput(aInput);

  if (!pCrypt.SealData(aInput.data(), aWorker.data(), aEncrypted.data(),
                       aInput.size(), &aErrorMessage, pMode)) {
    std::cerr << "SealData failed mode=" << peanutbutter::GetCryptModeName(pMode)
              << " error=" << aErrorMessage << "\n";
    return false;
  }
  if (!pCrypt.UnsealData(aEncrypted.data(), aWorker.data(), aRoundTrip.data(),
                         aInput.size(), &aErrorMessage, pMode)) {
    std::cerr << "UnsealData failed mode="
              << peanutbutter::GetCryptModeName(pMode)
              << " error=" << aErrorMessage << "\n";
    return false;
  }

  if (!std::equal(aInput.begin(), aInput.end(), aRoundTrip.begin())) {
    const std::size_t aMismatch = FindMismatch(aInput, aRoundTrip);
    std::cerr << "Round-trip mismatch mode=" << peanutbutter::GetCryptModeName(pMode)
              << " index=" << aMismatch << " input=0x" << std::hex
              << static_cast<int>(aInput[aMismatch]) << " output=0x"
              << static_cast<int>(aRoundTrip[aMismatch]) << std::dec << "\n";
    return false;
  }

  return true;
}

}  // namespace

int main() {
  const std::vector<unsigned char> aPepperMaskL1 =
      BuildPepperTable(BLOCK_SIZE_L1, 0xA5B35713u, 0xA3u);
  const std::vector<unsigned char> aPepperNoiseL1 =
      BuildPepperTable(BLOCK_SIZE_L1, 0xC8E9124Du, 0x5Cu);
  const std::vector<unsigned char> aPepperMaskL2 =
      BuildPepperTable(BLOCK_SIZE_L2, 0xA5B35713u, 0xA3u);
  const std::vector<unsigned char> aPepperNoiseL2 =
      BuildPepperTable(BLOCK_SIZE_L2, 0xC8E9124Du, 0x5Cu);
  const std::vector<unsigned char> aPepperMaskL3 =
      BuildPepperTable(BLOCK_SIZE_L3, 0xA5B35713u, 0xA3u);
  const std::vector<unsigned char> aPepperNoiseL3 =
      BuildPepperTable(BLOCK_SIZE_L3, 0xC8E9124Du, 0x5Cu);

  peanutbutter::LayeredCrypt aCrypt;
  AddEveryCipher(aCrypt.Layer1(), aPepperMaskL1, aPepperNoiseL1);
  AddEveryCipher(aCrypt.Layer2(), aPepperMaskL2, aPepperNoiseL2);
  AddEveryCipher(aCrypt.Layer3(), aPepperMaskL3, aPepperNoiseL3);

  std::cout << "QuickSmokeTest: ciphers_per_layer=" << aCrypt.Layer1().CipherCount()
            << " bytes=" << BLOCK_SIZE_L3 << "\n";

  for (peanutbutter::CryptMode aMode : peanutbutter::GetAvailableCryptModes()) {
    if (!RoundTrip(aMode, aCrypt)) {
      return 1;
    }
    std::cout << "PASS mode=" << peanutbutter::GetCryptModeName(aMode) << "\n";
  }

  std::cout << "PASS QuickSmokeTest\n";
  return 0;
}
