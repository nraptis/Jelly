#ifndef JELLY_BENCHMARK_BENCHMARK_BLOCK_EXECUTOR_HPP_
#define JELLY_BENCHMARK_BENCHMARK_BLOCK_EXECUTOR_HPP_

#include <cstddef>
#include <string>

#include "../src/Core/CipherFactory.hpp"

namespace peanutbutter::benchmark {

bool BenchmarkBlock(const BlockCipherFactory& pFactory,
                    int pBlockSize,
                    std::size_t pDataLength,
                    const std::string& pOutputFile,
                    std::string* pError = nullptr);

}  // namespace peanutbutter::benchmark

#endif  // JELLY_BENCHMARK_BENCHMARK_BLOCK_EXECUTOR_HPP_
