#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Splint/SplintMaskBlockCipher.hpp"
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
  std::uint8_t aBlockSize = 0;
  std::uint8_t aMask = 0;
  if (aBlockSizeParam == nullptr || aMaskParam == nullptr ||
      !swift_test_data::ReadUInt8(*aBlockSizeParam, aBlockSize) ||
      !swift_test_data::ReadUInt8(*aMaskParam, aMask)) {
    std::cerr << pPath << ": bad params at case " << pIndex << "\n";
    return false;
  }

  jelly::SplintMaskBlockCipher aCipher(aBlockSize, aMask);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "SplintMaskBlockCipher",
                                       ExecuteCase);
}
