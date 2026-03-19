cmake_minimum_required(VERSION 3.20)

project(JellySwiftGeneratedTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64|i.86)$")
  add_compile_options(-mssse3)
endif()

add_library(jelly INTERFACE)
target_include_directories(jelly INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(layered_crypt STATIC
  src/Encryption/Crypt.cpp
  src/Encryption/EncryptionLayer.cpp
  src/Encryption/LayeredCrypt.cpp
  src/Encryption/MaskLibrary.cpp
  src/Encryption/PassthroughCrypt.cpp
)
target_include_directories(layered_crypt PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(layered_crypt PUBLIC jelly)

add_library(peanut_tables STATIC
  src/Tables/Tables.cpp
)
target_include_directories(peanut_tables PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(peanut_tables PUBLIC jelly)

set(HAVE_SWIFT_GENERATED_TESTS FALSE)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/SwiftTestData.cpp)
  set(HAVE_SWIFT_GENERATED_TESTS TRUE)
  add_library(swift_test_data STATIC
    tests_swift/SwiftTestData.cpp
  )
  target_include_directories(swift_test_data PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift
  )
endif()

add_library(test_brew_support STATIC
  tests_brew/TestBrewExecutor.cpp
  tests_brew/TestBrewGenerator.cpp
)
target_include_directories(test_brew_support PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/tests_brew
)
target_link_libraries(test_brew_support PUBLIC jelly)

add_library(benchmark_support STATIC
  BENCHMARK/BenchmarkFlatExecutor.cpp
  BENCHMARK/BenchmarkBlockExecutor.cpp
)
target_include_directories(benchmark_support PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/BENCHMARK
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(benchmark_support PUBLIC jelly)

function(add_benchmark_target name)
  add_executable(${name}
    BENCHMARK/${name}.cpp
  )
  target_link_libraries(${name} PRIVATE benchmark_support)
  target_include_directories(${name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/BENCHMARK
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
endfunction()

function(add_cipher_family_target family)
  add_executable(${family}_all
    tests_swift/${family}/Tests_Generated_Small_${family}_All.cpp
  )
  target_link_libraries(${family}_all PRIVATE jelly swift_test_data)
  target_include_directories(${family}_all PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift
  )
endfunction()

function(add_cipher_family_test family)
  add_test(
    NAME ${family}_generated
    COMMAND ${family}_all ${ARGN}
  )
endfunction()

function(add_test_brew_target name)
  add_executable(${name}
    tests_brew/Ciphers/${name}.cpp
  )
  target_link_libraries(${name} PRIVATE test_brew_support)
  target_include_directories(${name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_brew
  )
endfunction()

if(HAVE_SWIFT_GENERATED_TESTS)
  add_cipher_family_target(RotateCipher)
  add_cipher_family_target(RotateBlockCipher)
  add_cipher_family_target(RotateMaskCipher)
  add_cipher_family_target(RotateMaskBlockCipher)
  add_cipher_family_target(SplintCipher)
  add_cipher_family_target(SplintBlockCipher)
  add_cipher_family_target(SplintByteBlockCipher)
  add_cipher_family_target(SplintMaskCipher)
  add_cipher_family_target(SplintMaskBlockCipher)
  add_cipher_family_target(SplintMaskByteBlockCipher)
  add_cipher_family_target(WeaveCipher)
  add_cipher_family_target(WeaveBlockCipher)
  add_cipher_family_target(WeaveByteBlockCipher)
  add_cipher_family_target(WeaveMaskCipher)
  add_cipher_family_target(WeaveMaskBlockCipher)
  add_cipher_family_target(WeaveMaskByteBlockCipher)

  set(BLOCK_DATA_SUFFIXES 8 12 16 24 32 48)

  function(add_block_family_generated_test family)
    set(args)
    foreach(size IN LISTS BLOCK_DATA_SUFFIXES)
      list(APPEND args
        ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/${family}/Tests_Generated_Small_${family}_${size}.dat
      )
    endforeach()
    add_cipher_family_test(${family} ${args})
  endfunction()

  add_cipher_family_test(
    RotateCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/RotateCipher/Tests_Generated_Small_RotateCipher.dat
  )
  add_block_family_generated_test(RotateBlockCipher)
  add_cipher_family_test(
    RotateMaskCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/RotateMaskCipher/Tests_Generated_Small_RotateMaskCipher.dat
  )
  add_block_family_generated_test(RotateMaskBlockCipher)

  add_cipher_family_test(
    SplintCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/SplintCipher/Tests_Generated_Small_SplintCipher.dat
  )
  add_block_family_generated_test(SplintBlockCipher)
  add_block_family_generated_test(SplintByteBlockCipher)
  add_cipher_family_test(
    SplintMaskCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/SplintMaskCipher/Tests_Generated_Small_SplintMaskCipher.dat
  )
  add_block_family_generated_test(SplintMaskBlockCipher)
  add_block_family_generated_test(SplintMaskByteBlockCipher)

  add_cipher_family_test(
    WeaveCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/WeaveCipher/Tests_Generated_Small_WeaveCipher.dat
  )
  add_block_family_generated_test(WeaveBlockCipher)
  add_block_family_generated_test(WeaveByteBlockCipher)
  add_cipher_family_test(
    WeaveMaskCipher
    ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift/WeaveMaskCipher/Tests_Generated_Small_WeaveMaskCipher.dat
  )
  add_block_family_generated_test(WeaveMaskBlockCipher)
  add_block_family_generated_test(WeaveMaskByteBlockCipher)
endif()

add_test_brew_target(RotateCipher_TestBrew)
add_test_brew_target(RotateBlockCipher_TestBrew)
add_test_brew_target(RotateBlockByteCipher_TestBrew)
add_test_brew_target(RotateMaskCipher_TestBrew)
add_test_brew_target(RotateMaskBlockCipher_TestBrew)
add_test_brew_target(RotateMaskByteBlockCipher_TestBrew)
add_test_brew_target(SplintCipher_TestBrew)
add_test_brew_target(SplintBlockCipher_TestBrew)
add_test_brew_target(SplintByteBlockCipher_TestBrew)
add_test_brew_target(SplintMaskCipher_TestBrew)
add_test_brew_target(SplintMaskBlockCipher_TestBrew)
add_test_brew_target(SplintMaskByteBlockCipher_TestBrew)
add_test_brew_target(WeaveCipher_TestBrew)
add_test_brew_target(WeaveBlockCipher_TestBrew)
add_test_brew_target(WeaveByteBlockCipher_TestBrew)
add_test_brew_target(WeaveMaskCipher_TestBrew)
add_test_brew_target(WeaveMaskBlockCipher_TestBrew)
add_test_brew_target(WeaveMaskByteBlockCipher_TestBrew)
add_test_brew_target(ReverseCipher_TestBrew)
add_test_brew_target(ReverseBlockCipher_TestBrew)
add_test_brew_target(ReverseBlockByteCipher_TestBrew)
add_test_brew_target(ReverseMaskCipher_TestBrew)
add_test_brew_target(ReverseMaskBlockCipher_TestBrew)
add_test_brew_target(ReverseMaskByteBlockCipher_TestBrew)
add_test_brew_target(PasswordAddCipher_TestBrew)
add_test_brew_target(PasswordSubtractCipher_TestBrew)
add_test_brew_target(PasswordXORCipher_TestBrew)
add_test_brew_target(PasswordJumpAddCipher_TestBrew)
add_test_brew_target(PasswordJumpSubtractCipher_TestBrew)
add_test_brew_target(PasswordJumpXORCipher_TestBrew)
add_test_brew_target(RippleCipher_TestBrew)
add_test_brew_target(RippleBlockCipher_TestBrew)
add_test_brew_target(RippleMaskCipher_TestBrew)
add_test_brew_target(RippleMaskBlockCipher_TestBrew)
add_test_brew_target(InvertCipher_TestBrew)
add_test_brew_target(InvertMaskCipher_TestBrew)
add_test_brew_target(PepperAddCipher_TestBrew)
add_test_brew_target(PepperSubtractCipher_TestBrew)
add_test_brew_target(PepperXORCipher_TestBrew)
add_test_brew_target(PepperJumpXORCipher_TestBrew)
add_test_brew_target(PepperNoiseXORCipher_TestBrew)
add_test_brew_target(PepperJumpNoiseXORCipher_TestBrew)
add_test_brew_target(PepperDualJumpNoiseXORCipher_TestBrew)
add_test_brew_target(CascadeXORCipher_TestBrew)
add_test_brew_target(CascadeJumpXORCipher_TestBrew)
add_test_brew_target(SwapGridVV16_TestBrew)
add_test_brew_target(SwapGridVH16_TestBrew)
add_test_brew_target(SwapGridHV16_TestBrew)
add_test_brew_target(SwapGridHH16_TestBrew)
add_test_brew_target(SwapGridVV64_TestBrew)
add_test_brew_target(SwapGridVH64_TestBrew)
add_test_brew_target(SwapGridHV64_TestBrew)
add_test_brew_target(SwapGridHH64_TestBrew)
add_test_brew_target(SwapGridMaskVV16_TestBrew)
add_test_brew_target(SwapGridMaskVH16_TestBrew)
add_test_brew_target(SwapGridMaskHV16_TestBrew)
add_test_brew_target(SwapGridMaskHH16_TestBrew)
add_test_brew_target(SwapGridMaskVV64_TestBrew)
add_test_brew_target(SwapGridMaskVH64_TestBrew)
add_test_brew_target(SwapGridMaskHV64_TestBrew)
add_test_brew_target(SwapGridMaskHH64_TestBrew)
add_test_brew_target(SpiralGridH16_TestBrew)
add_test_brew_target(SpiralGridV16_TestBrew)
add_test_brew_target(SpiralGridH64_TestBrew)
add_test_brew_target(SpiralGridV64_TestBrew)
add_test_brew_target(SpiralGridMaskH16_TestBrew)
add_test_brew_target(SpiralGridMaskV16_TestBrew)
add_test_brew_target(SpiralGridMaskH64_TestBrew)
add_test_brew_target(SpiralGridMaskV64_TestBrew)

add_executable(QuickSmokeTest
  QuickSmokeTest.cpp
)
target_include_directories(QuickSmokeTest PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(QuickSmokeTest PRIVATE layered_crypt)
add_test(NAME QuickSmokeTest COMMAND QuickSmokeTest)

add_executable(CompatabilityTests
  CompatabilityTests.cpp
)
target_include_directories(CompatabilityTests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(CompatabilityTests PRIVATE layered_crypt peanut_tables)

add_benchmark_target(BenchmarkRun_RotateCipher)
add_benchmark_target(BenchmarkRun_RotateBlockCipher)
add_benchmark_target(BenchmarkRun_RotateBlockByteCipher)
add_benchmark_target(BenchmarkRun_RotateMaskCipher)
add_benchmark_target(BenchmarkRun_RotateMaskBlockCipher)
add_benchmark_target(BenchmarkRun_RotateMaskByteBlockCipher)
add_benchmark_target(BenchmarkRun_SplintCipher)
add_benchmark_target(BenchmarkRun_SplintBlockCipher)
add_benchmark_target(BenchmarkRun_SplintByteBlockCipher)
add_benchmark_target(BenchmarkRun_SplintMaskCipher)
add_benchmark_target(BenchmarkRun_SplintMaskBlockCipher)
add_benchmark_target(BenchmarkRun_SplintMaskByteBlockCipher)
add_benchmark_target(BenchmarkRun_WeaveCipher)
add_benchmark_target(BenchmarkRun_WeaveBlockCipher)
add_benchmark_target(BenchmarkRun_WeaveByteBlockCipher)
add_benchmark_target(BenchmarkRun_WeaveMaskCipher)
add_benchmark_target(BenchmarkRun_WeaveMaskBlockCipher)
add_benchmark_target(BenchmarkRun_WeaveMaskByteBlockCipher)
add_benchmark_target(BenchmarkRun_ReverseCipher)
add_benchmark_target(BenchmarkRun_ReverseBlockCipher)
add_benchmark_target(BenchmarkRun_ReverseBlockByteCipher)
add_benchmark_target(BenchmarkRun_ReverseMaskCipher)
add_benchmark_target(BenchmarkRun_ReverseMaskBlockCipher)
add_benchmark_target(BenchmarkRun_ReverseMaskByteBlockCipher)
add_benchmark_target(BenchmarkRun_PasswordAddCipher)
add_benchmark_target(BenchmarkRun_PasswordSubtractCipher)
add_benchmark_target(BenchmarkRun_PasswordXORCipher)
add_benchmark_target(BenchmarkRun_PasswordJumpAddCipher)
add_benchmark_target(BenchmarkRun_PasswordJumpSubtractCipher)
add_benchmark_target(BenchmarkRun_PasswordJumpXORCipher)
add_benchmark_target(BenchmarkRun_RippleCipher)
add_benchmark_target(BenchmarkRun_RippleBlockCipher)
add_benchmark_target(BenchmarkRun_RippleMaskCipher)
add_benchmark_target(BenchmarkRun_RippleMaskBlockCipher)
add_benchmark_target(BenchmarkRun_InvertCipher)
add_benchmark_target(BenchmarkRun_InvertMaskCipher)
add_benchmark_target(BenchmarkRun_PepperAddCipher)
add_benchmark_target(BenchmarkRun_PepperSubtractCipher)
add_benchmark_target(BenchmarkRun_PepperXORCipher)
add_benchmark_target(BenchmarkRun_PepperJumpXORCipher)
add_benchmark_target(BenchmarkRun_PepperNoiseXORCipher)
add_benchmark_target(BenchmarkRun_PepperJumpNoiseXORCipher)
add_benchmark_target(BenchmarkRun_PepperDualJumpNoiseXORCipher)
add_benchmark_target(BenchmarkRun_CascadeXORCipher)
add_benchmark_target(BenchmarkRun_CascadeJumpXORCipher)
add_benchmark_target(BenchmarkRun_SwapGridVV16)
add_benchmark_target(BenchmarkRun_SwapGridVH16)
add_benchmark_target(BenchmarkRun_SwapGridHV16)
add_benchmark_target(BenchmarkRun_SwapGridHH16)
add_benchmark_target(BenchmarkRun_SwapGridVV64)
add_benchmark_target(BenchmarkRun_SwapGridVH64)
add_benchmark_target(BenchmarkRun_SwapGridHV64)
add_benchmark_target(BenchmarkRun_SwapGridHH64)
add_benchmark_target(BenchmarkRun_SwapGridMaskVV16)
add_benchmark_target(BenchmarkRun_SwapGridMaskVH16)
add_benchmark_target(BenchmarkRun_SwapGridMaskHV16)
add_benchmark_target(BenchmarkRun_SwapGridMaskHH16)
add_benchmark_target(BenchmarkRun_SwapGridMaskVV64)
add_benchmark_target(BenchmarkRun_SwapGridMaskVH64)
add_benchmark_target(BenchmarkRun_SwapGridMaskHV64)
add_benchmark_target(BenchmarkRun_SwapGridMaskHH64)
add_benchmark_target(BenchmarkRun_SpiralGridH16)
add_benchmark_target(BenchmarkRun_SpiralGridV16)
add_benchmark_target(BenchmarkRun_SpiralGridH64)
add_benchmark_target(BenchmarkRun_SpiralGridV64)
add_benchmark_target(BenchmarkRun_SpiralGridMaskH16)
add_benchmark_target(BenchmarkRun_SpiralGridMaskV16)
add_benchmark_target(BenchmarkRun_SpiralGridMaskH64)
add_benchmark_target(BenchmarkRun_SpiralGridMaskV64)
add_benchmark_target(BenchmarkRun_All)
