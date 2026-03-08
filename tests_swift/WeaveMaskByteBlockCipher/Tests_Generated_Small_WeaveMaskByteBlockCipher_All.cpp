#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Weave/WeaveMaskByteBlockCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* aBlockSizeParam =
      swift_test_data::FindParam(pCase, "block_size");
  const swift_test_data::Param* aMaskParam =
      swift_test_data::FindParam(pCase, "mask");
  const swift_test_data::Param* aCountParam =
      swift_test_data::FindParam(pCase, "count");
  const swift_test_data::Param* aFrontParam =
      swift_test_data::FindParam(pCase, "front_stride");
  const swift_test_data::Param* aBackParam =
      swift_test_data::FindParam(pCase, "back_stride");
  std::uint8_t aBlockSize = 0;
  std::uint8_t aMask = 0;
  std::uint8_t aCount = 0;
  std::uint8_t aFrontStride = 0;
  std::uint8_t aBackStride = 0;
  if (aBlockSizeParam == nullptr || aMaskParam == nullptr ||
      aCountParam == nullptr || aFrontParam == nullptr || aBackParam == nullptr ||
      !swift_test_data::ReadUInt8(*aBlockSizeParam, aBlockSize) ||
      !swift_test_data::ReadUInt8(*aMaskParam, aMask) ||
      !swift_test_data::ReadUInt8(*aCountParam, aCount) ||
      !swift_test_data::ReadUInt8(*aFrontParam, aFrontStride) ||
      !swift_test_data::ReadUInt8(*aBackParam, aBackStride)) {
    std::cerr << pPath << ": bad params at case " << pIndex << "\n";
    return false;
  }

  jelly::WeaveMaskByteBlockCipher aCipher(aBlockSize, aMask, aCount,
                                          aFrontStride, aBackStride);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(
      argc, argv, "WeaveMaskByteBlockCipher", ExecuteCase);
}
