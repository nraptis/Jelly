#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aBuildDir="build"
aSwiftBuildDir="EncryptionTools-main/tests_swift/.build"
aPassCount=0
aFailCount=0

aTargets="
BenchmarkRun_RotateCipher
BenchmarkRun_RotateBlockCipher
BenchmarkRun_RotateMaskCipher
BenchmarkRun_RotateMaskBlockCipher
BenchmarkRun_SplintCipher
BenchmarkRun_SplintBlockCipher
BenchmarkRun_SplintByteBlockCipher
BenchmarkRun_SplintMaskCipher
BenchmarkRun_SplintMaskBlockCipher
BenchmarkRun_SplintMaskByteBlockCipher
BenchmarkRun_WeaveCipher
BenchmarkRun_WeaveBlockCipher
BenchmarkRun_WeaveByteBlockCipher
BenchmarkRun_WeaveMaskCipher
BenchmarkRun_WeaveMaskBlockCipher
BenchmarkRun_WeaveMaskByteBlockCipher
"

rm -rf "$aBuildDir"
rm -rf "$aSwiftBuildDir"
rm -rf BENCHMARK_LOGS
mkdir -p BENCHMARK_LOGS

cmake -S . -B "$aBuildDir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG"
cmake --build "$aBuildDir" --config Release -j4

for aTarget in $aTargets; do
  echo "Running $aTarget"
  if "$aBuildDir/$aTarget"; then
    echo "PASS $aTarget"
    aPassCount=$((aPassCount + 1))
  else
    echo "FAIL $aTarget"
    aFailCount=$((aFailCount + 1))
  fi
done

echo "Benchmark runs passed: $aPassCount"
echo "Benchmark runs failed: $aFailCount"

if [ "$aFailCount" -ne 0 ]; then
  exit 1
fi
