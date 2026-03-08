#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Rotation/RotateMaskCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* aMaskParam =
      swift_test_data::FindParam(pCase, "mask");
  const swift_test_data::Param* aShiftParam =
      swift_test_data::FindParam(pCase, "shift");
  if (aMaskParam == nullptr || aShiftParam == nullptr) {
    std::cerr << pPath << ": missing params at case " << pIndex << "\n";
    return false;
  }

  std::uint8_t aMask = 0;
  std::int16_t aShift = 0;
  if (!swift_test_data::ReadUInt8(*aMaskParam, aMask) ||
      !swift_test_data::ReadInt16(*aShiftParam, aShift)) {
    std::cerr << pPath << ": bad param encoding at case " << pIndex << "\n";
    return false;
  }

  jelly::RotateMaskCipher aCipher(aMask, aShift);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "RotateMaskCipher",
                                       ExecuteCase);
}
