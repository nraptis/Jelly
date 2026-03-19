#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aIterations="${1:-1}"
aBuildDir="build"

rm -rf "$aBuildDir"

cmake -S . -B "$aBuildDir" -DCMAKE_BUILD_TYPE=Release
cmake --build "$aBuildDir" -j4
"$aBuildDir/CompatabilityTests" "$aIterations"
