#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Splint/SplintBlockCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* aBlockSizeParam =
      swift_test_data::FindParam(pCase, "block_size");
  std::uint8_t aBlockSize = 0;
  if (aBlockSizeParam == nullptr ||
      !swift_test_data::ReadUInt8(*aBlockSizeParam, aBlockSize)) {
    std::cerr << pPath << ": bad block_size at case " << pIndex << "\n";
    return false;
  }

  jelly::SplintBlockCipher aCipher(aBlockSize);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "SplintBlockCipher",
                                       ExecuteCase);
}
