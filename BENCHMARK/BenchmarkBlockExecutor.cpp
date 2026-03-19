#include "BenchmarkBlockExecutor.hpp"

#include "BenchmarkFlatExecutor.hpp"
#include "../src/PeanutButter.hpp"

namespace peanutbutter::benchmark {

bool BenchmarkBlock(const BlockCipherFactory& pFactory,
                    int pBlockSize,
                    std::size_t pDataLength,
                    const std::string& pOutputFile,
                    std::string* pError) {
  if (!pFactory) {
    if (pError != nullptr) {
      *pError = "pFactory must not be empty";
    }
    return false;
  }
  if ((pDataLength % BLOCK_GRANULARITY) != 0) {
    if (pError != nullptr) {
      *pError = "pDataLength must be a multiple of 192";
    }
    return false;
  }
  return BenchmarkFlat(
      [&](std::size_t pCaseIndex, CryptMode pMode) {
        return pFactory(pBlockSize, pCaseIndex, pMode);
      },
      pDataLength, pOutputFile, pError);
}

}  // namespace peanutbutter::benchmark
