#include <cstdint>
#include <iostream>
#include <vector>

#include "../../src/Ciphers/Splint/SplintMaskCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  const swift_test_data::Param* aMaskParam =
      swift_test_data::FindParam(pCase, "mask");
  std::uint8_t aMask = 0;
  if (aMaskParam == nullptr || !swift_test_data::ReadUInt8(*aMaskParam, aMask)) {
    std::cerr << pPath << ": bad mask at case " << pIndex << "\n";
    return false;
  }

  jelly::SplintMaskCipher aCipher(aMask);
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "SplintMaskCipher",
                                       ExecuteCase);
}
