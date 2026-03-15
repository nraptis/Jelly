#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aBuildDir="build"
aSwiftBuildDir="EncryptionTools-main/tests_swift/.build"
aPassCount=0
aFailCount=0
aTargets=()

rm -rf "$aBuildDir"
rm -rf "$aSwiftBuildDir"
rm -rf BENCHMARK_LOGS
mkdir -p BENCHMARK_LOGS

cmake -S . -B "$aBuildDir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG"
cmake --build "$aBuildDir" --config Release -j4

while IFS= read -r aTarget; do
  if [ -n "$aTarget" ]; then
    aTargets+=("$aTarget")
  fi
done <<EOF
$(awk '
  /^\s*add_benchmark_target\(/ {
    gsub(/^[[:space:]]*add_benchmark_target\(/, "", $0)
    gsub(/\)[[:space:]]*$/, "", $0)
    print $0
  }
' CMakeLists.txt)
EOF

if [ ${#aTargets[@]} -eq 0 ]; then
  echo "FAIL No benchmark targets found in CMakeLists.txt"
  exit 1
fi

for aTarget in "${aTargets[@]}"; do
  aBinary="$aBuildDir/$aTarget"
  echo "Running $aTarget"
  if [ ! -x "$aBinary" ]; then
    echo "MISSING $aTarget (binary not built)"
    aFailCount=$((aFailCount + 1))
    continue
  fi

  if "$aBinary"; then
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
