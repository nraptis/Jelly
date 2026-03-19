#ifndef JELLY_BENCHMARK_BENCHMARK_FLAT_EXECUTOR_HPP_
#define JELLY_BENCHMARK_BENCHMARK_FLAT_EXECUTOR_HPP_

#include <cstddef>
#include <string>

#include "../src/Core/CipherFactory.hpp"

#define RUN_COUNT 500
#define DISCARD_HI 25
#define DISCARD_LO 25

namespace peanutbutter::benchmark {

bool BenchmarkFlat(const CipherFactory& pFactory,
                   std::size_t pDataLength,
                   const std::string& pOutputFile,
                   std::string* pError = nullptr);

bool BenchmarkFlat(const SizedCipherFactory& pFactory,
                   std::size_t pDataLength,
                   const std::string& pOutputFile,
                   std::string* pError = nullptr);

}  // namespace peanutbutter::benchmark

#endif  // JELLY_BENCHMARK_BENCHMARK_FLAT_EXECUTOR_HPP_
