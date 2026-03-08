#include <iostream>
#include <vector>

#include "../../src/Ciphers/Splint/SplintCipher.hpp"
#include "../SwiftTestData.hpp"
#include "../TestSwiftExecutor.hpp"

namespace {

bool ExecuteCase(const swift_test_data::TestCase& pCase,
                 std::size_t pIndex,
                 const char* pPath) {
  jelly::SplintCipher aCipher;
  return test_swift_executor::ExecuteRoundTrip(
      aCipher, pCase.mInput, pCase.mExpected, pPath, pIndex);
}

}  // namespace

int main(int argc, char** argv) {
  return test_swift_executor::RunFiles(argc, argv, "SplintCipher", ExecuteCase);
}
