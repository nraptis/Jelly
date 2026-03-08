#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Rotation/RotateCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* aShiftParam =
      swift_test_data::FindParam(pCase, "shift");
  if (aShiftParam == nullptr) {
    std::cerr << pPath << ": missing shift at case " << pIndex << "\n";
    return false;
  }

  std::int16_t aShift = 0;
  if (!swift_test_data::ReadInt16(*aShiftParam, aShift)) {
    std::cerr << pPath << ": bad shift encoding at case " << pIndex << "\n";
    return false;
  }

  jelly::RotateCipher aCipher(aShift);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "RotateCipher", ExecuteCase);
}
