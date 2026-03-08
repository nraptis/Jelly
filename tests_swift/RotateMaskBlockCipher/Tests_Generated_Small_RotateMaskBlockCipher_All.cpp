#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Rotation/RotateMaskBlockCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

template <std::size_t tBlockSize>
bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* pMaskParam =
      swift_test_data::FindParam(pCase, "mask");
  const swift_test_data::Param* pShiftParam =
      swift_test_data::FindParam(pCase, "shift");
  if (pMaskParam == nullptr || pShiftParam == nullptr) {
    std::cerr << "Missing params at case " << pIndex << "\n";
    return false;
  }

  std::uint8_t aMask = 0;
  std::int16_t aShift = 0;
  if (!swift_test_data::ReadUInt8(*pMaskParam, aMask) ||
      !swift_test_data::ReadInt16(*pShiftParam, aShift)) {
    std::cerr << "Bad param encoding at case " << pIndex << "\n";
    return false;
  }

  jelly::RotateMaskBlockCipher<tBlockSize> aCipher(aMask, aShift);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

bool ExecuteCaseByBlockSize(const swift_test_data::TestCase& pCase,
                            std::size_t pIndex,
                            const char* pPath) {
  const swift_test_data::Param* pBlockSizeParam =
      swift_test_data::FindParam(pCase, "block_size");
  if (pBlockSizeParam == nullptr) {
    std::cerr << "Missing block_size at case " << pIndex << "\n";
    return false;
  }

  std::uint8_t aBlockSize = 0;
  if (!swift_test_data::ReadUInt8(*pBlockSizeParam, aBlockSize)) {
    std::cerr << "Bad block_size encoding at case " << pIndex << "\n";
    return false;
  }

  switch (aBlockSize) {
    case 8: return ExecuteCase<8>(pCase, pIndex, pPath);
    case 12: return ExecuteCase<12>(pCase, pIndex, pPath);
    case 16: return ExecuteCase<16>(pCase, pIndex, pPath);
    case 24: return ExecuteCase<24>(pCase, pIndex, pPath);
    case 32: return ExecuteCase<32>(pCase, pIndex, pPath);
    case 48: return ExecuteCase<48>(pCase, pIndex, pPath);
    default:
      std::cerr << "Unsupported block size at case " << pIndex << "\n";
      return false;
  }
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(
      argc, argv, "RotateMaskBlockCipher",
      [](const swift_test_data::TestCase& pCase, std::size_t pIndex,
         const char* pPath) { return ExecuteCaseByBlockSize(pCase, pIndex, pPath); });
}
