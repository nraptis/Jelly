#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "src/Ciphers/Invert/InvertCipher.hpp"
#include "src/Ciphers/Invert/InvertMaskCipher.hpp"
#include "src/Ciphers/Password/PasswordCipher.hpp"
#include "src/Ciphers/Pepper/PepperCipherXOR.hpp"
#include "src/Ciphers/Pepper/PepperCipherXORNoise.hpp"
#include "src/Ciphers/Reverse/ReverseBlockByteCipher.hpp"
#include "src/Ciphers/Reverse/ReverseBlockCipher.hpp"
#include "src/Ciphers/Reverse/ReverseCipher.hpp"
#include "src/Ciphers/Reverse/ReverseMaskBlockCipher.hpp"
#include "src/Ciphers/Reverse/ReverseMaskByteBlockCipher.hpp"
#include "src/Ciphers/Reverse/ReverseMaskCipher.hpp"
#include "src/Ciphers/Ripple/RippleBlockCipher.hpp"
#include "src/Ciphers/Ripple/RippleCipher.hpp"
#include "src/Ciphers/Ripple/RippleMaskBlockCipher.hpp"
#include "src/Ciphers/Ripple/RippleMaskCipher.hpp"
#include "src/Ciphers/Rotation/RotateBlockByteCipher.hpp"
#include "src/Ciphers/Rotation/RotateBlockCipher.hpp"
#include "src/Ciphers/Rotation/RotateCipher.hpp"
#include "src/Ciphers/Rotation/RotateMaskBlockCipher.hpp"
#include "src/Ciphers/Rotation/RotateMaskByteBlockCipher.hpp"
#include "src/Ciphers/Rotation/RotateMaskCipher.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridH16.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridH64.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridMaskH16.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridMaskH64.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridMaskV16.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridMaskV64.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridV16.hpp"
#include "src/Ciphers/SpiralGrid/SpiralGridV64.hpp"
#include "src/Ciphers/Splint/SplintBlockCipher.hpp"
#include "src/Ciphers/Splint/SplintByteBlockCipher.hpp"
#include "src/Ciphers/Splint/SplintCipher.hpp"
#include "src/Ciphers/Splint/SplintMaskBlockCipher.hpp"
#include "src/Ciphers/Splint/SplintMaskByteBlockCipher.hpp"
#include "src/Ciphers/Splint/SplintMaskCipher.hpp"
#include "src/Ciphers/SwapGrid/SwapGridHH16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridHH64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridHV16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridHV64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskHH16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskHH64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskHV16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskHV64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskVH16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskVH64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskVV16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridMaskVV64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridVH16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridVH64.hpp"
#include "src/Ciphers/SwapGrid/SwapGridVV16.hpp"
#include "src/Ciphers/SwapGrid/SwapGridVV64.hpp"
#include "src/Ciphers/Weave/WeaveBlockCipher.hpp"
#include "src/Ciphers/Weave/WeaveByteBlockCipher.hpp"
#include "src/Ciphers/Weave/WeaveCipher.hpp"
#include "src/Ciphers/Weave/WeaveMaskBlockCipher.hpp"
#include "src/Ciphers/Weave/WeaveMaskByteBlockCipher.hpp"
#include "src/Ciphers/Weave/WeaveMaskCipher.hpp"
#include "src/Core/EncryptionLayer.hpp"
#include "src/Core/CryptMode.hpp"
#include "src/Jelly.hpp"

namespace {

constexpr std::size_t kBlockSizes[] = {
    jelly::EB_BLOCK_SIZE_08, jelly::EB_BLOCK_SIZE_12, jelly::EB_BLOCK_SIZE_16,
    jelly::EB_BLOCK_SIZE_24, jelly::EB_BLOCK_SIZE_32, jelly::EB_BLOCK_SIZE_48,
};

void AddEveryCipher(jelly::EncryptionLayer& pLayer) {
  pLayer.AddCipher(std::make_unique<jelly::RotateCipher>(-16));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<jelly::RotateBlockCipher>(aBlockSize, -16));
    pLayer.AddCipher(
        std::make_unique<jelly::RotateBlockByteCipher>(aBlockSize, -16));
  }
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskCipher>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher08>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher12>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher16>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher24>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher32>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<jelly::RotateMaskBlockCipher48>(0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_08>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_12>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_16>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_24>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_32>>(
      0xDEu, -16));
  pLayer.AddCipher(std::make_unique<
                   jelly::RotateMaskByteBlockCipher<jelly::EB_BLOCK_SIZE_48>>(
      0xDEu, -16));

  pLayer.AddCipher(std::make_unique<jelly::SplintCipher>());
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<jelly::SplintBlockCipher>(aBlockSize));
    pLayer.AddCipher(std::make_unique<jelly::SplintByteBlockCipher>(aBlockSize));
  }
  pLayer.AddCipher(std::make_unique<jelly::SplintMaskCipher>(0xA3u));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<jelly::SplintMaskBlockCipher>(aBlockSize, 0xA3u));
    pLayer.AddCipher(
        std::make_unique<jelly::SplintMaskByteBlockCipher>(aBlockSize, 0xA3u));
  }

  pLayer.AddCipher(std::make_unique<jelly::WeaveCipher>(3, 1, 2));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<jelly::WeaveBlockCipher>(aBlockSize, 3, 1, 2));
    pLayer.AddCipher(
        std::make_unique<jelly::WeaveByteBlockCipher>(aBlockSize, 3, 1, 2));
  }
  pLayer.AddCipher(std::make_unique<jelly::WeaveMaskCipher>(0x3Eu, 3, 1, 2));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<jelly::WeaveMaskBlockCipher>(
        aBlockSize, 0x3Eu, 3, 1, 2));
    pLayer.AddCipher(std::make_unique<jelly::WeaveMaskByteBlockCipher>(
        aBlockSize, 0x3Eu, 3, 1, 2));
  }

  pLayer.AddCipher(std::make_unique<jelly::ReverseCipher>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher08>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher12>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher16>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher24>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher32>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockCipher48>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher08>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher12>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher16>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher24>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher32>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseBlockByteCipher48>());
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskCipher>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher08>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher12>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher24>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher32>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskBlockCipher48>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher08>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher12>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher24>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher32>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::ReverseMaskByteBlockCipher48>(0xA3u));

  pLayer.AddCipher(std::make_unique<jelly::PasswordCipher>("catdog"));

  pLayer.AddCipher(std::make_unique<jelly::RippleCipher>(4));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(std::make_unique<jelly::RippleBlockCipher>(aBlockSize, 4));
  }
  pLayer.AddCipher(std::make_unique<jelly::RippleMaskCipher>(0xA3u, 4));
  for (std::size_t aBlockSize : kBlockSizes) {
    pLayer.AddCipher(
        std::make_unique<jelly::RippleMaskBlockCipher>(aBlockSize, 0xA3u, 4));
  }

  pLayer.AddCipher(std::make_unique<jelly::InvertCipher>());
  pLayer.AddCipher(std::make_unique<jelly::InvertMaskCipher>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::PepperCipherXOR>());
  pLayer.AddCipher(std::make_unique<jelly::PepperCipherXORNoise>());

  pLayer.AddCipher(std::make_unique<jelly::SwapGridVV16>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridVH16>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridHV16>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridHH16>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridVV64>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridVH64>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridHV64>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridHH64>());
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskVV16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskVH16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskHV16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskHH16>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskVV64>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskVH64>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskHV64>(0xA3u));
  pLayer.AddCipher(std::make_unique<jelly::SwapGridMaskHH64>(0xA3u));

  pLayer.AddCipher(std::make_unique<jelly::SpiralGridH16>(2));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridV16>(2));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridH64>(5));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridV64>(5));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridMaskH16>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridMaskV16>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridMaskH64>(0xA3u, 2));
  pLayer.AddCipher(std::make_unique<jelly::SpiralGridMaskV64>(0xA3u, 2));
}

void FillInput(std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY>& pData) {
  std::uint32_t aState = 0xC001D00Du;
  for (std::size_t aIndex = 0; aIndex < pData.size(); ++aIndex) {
    aState ^= (aState << 13);
    aState ^= (aState >> 17);
    aState ^= (aState << 5);
    pData[aIndex] = static_cast<unsigned char>(aState & 0xFFu);
  }
}

std::size_t FindMismatch(
    const std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY>& pLeft,
    const std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY>& pRight) {
  for (std::size_t aIndex = 0; aIndex < pLeft.size(); ++aIndex) {
    if (pLeft[aIndex] != pRight[aIndex]) {
      return aIndex;
    }
  }
  return pLeft.size();
}

bool RoundTrip(jelly::CryptMode pMode,
               const jelly::EncryptionLayer& pLayer1,
               const jelly::EncryptionLayer& pLayer2,
               const jelly::EncryptionLayer& pLayer3) {
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aInput{};
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aWorker{};
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aStage1{};
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aStage2{};
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aEncrypted{};
  std::array<unsigned char, jelly::SB_CIPHER_LENGTH_GRANULARITY> aRoundTrip{};

  FillInput(aInput);

  if (!pLayer1.SealData(aInput.data(), aWorker.data(), aStage1.data(),
                        aInput.size(), pMode)) {
    std::cerr << "SealData failed at layer1 mode=" << jelly::GetCryptModeName(pMode)
              << "\n";
    return false;
  }
  if (!pLayer2.SealData(aStage1.data(), aWorker.data(), aStage2.data(),
                        aInput.size(), pMode)) {
    std::cerr << "SealData failed at layer2 mode=" << jelly::GetCryptModeName(pMode)
              << "\n";
    return false;
  }
  if (!pLayer3.SealData(aStage2.data(), aWorker.data(), aEncrypted.data(),
                        aInput.size(), pMode)) {
    std::cerr << "SealData failed at layer3 mode=" << jelly::GetCryptModeName(pMode)
              << "\n";
    return false;
  }

  if (!pLayer3.UnsealData(aEncrypted.data(), aWorker.data(), aStage2.data(),
                          aInput.size(), pMode)) {
    std::cerr << "UnsealData failed at layer3 mode="
              << jelly::GetCryptModeName(pMode) << "\n";
    return false;
  }
  if (!pLayer2.UnsealData(aStage2.data(), aWorker.data(), aStage1.data(),
                          aInput.size(), pMode)) {
    std::cerr << "UnsealData failed at layer2 mode="
              << jelly::GetCryptModeName(pMode) << "\n";
    return false;
  }
  if (!pLayer1.UnsealData(aStage1.data(), aWorker.data(), aRoundTrip.data(),
                          aInput.size(), pMode)) {
    std::cerr << "UnsealData failed at layer1 mode="
              << jelly::GetCryptModeName(pMode) << "\n";
    return false;
  }

  if (!std::equal(aInput.begin(), aInput.end(), aRoundTrip.begin())) {
    const std::size_t aMismatch = FindMismatch(aInput, aRoundTrip);
    std::cerr << "Round-trip mismatch mode=" << jelly::GetCryptModeName(pMode)
              << " index=" << aMismatch << " input=0x" << std::hex
              << static_cast<int>(aInput[aMismatch]) << " output=0x"
              << static_cast<int>(aRoundTrip[aMismatch]) << std::dec << "\n";
    return false;
  }

  return true;
}

}  // namespace

int main() {
  jelly::EncryptionLayer aLayer1;
  jelly::EncryptionLayer aLayer2;
  jelly::EncryptionLayer aLayer3;
  AddEveryCipher(aLayer1);
  AddEveryCipher(aLayer2);
  AddEveryCipher(aLayer3);

  std::cout << "QuickSmokeTest: ciphers_per_layer=" << aLayer1.CipherCount()
            << " bytes=" << jelly::SB_CIPHER_LENGTH_GRANULARITY << "\n";

  for (jelly::CryptMode aMode : jelly::GetAvailableCryptModes()) {
    if (!RoundTrip(aMode, aLayer1, aLayer2, aLayer3)) {
      return 1;
    }
    std::cout << "PASS mode=" << jelly::GetCryptModeName(aMode) << "\n";
  }

  std::cout << "PASS QuickSmokeTest\n";
  return 0;
}
