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

add_library(swift_test_data STATIC
  tests_swift/SwiftTestData.cpp
)
target_include_directories(swift_test_data PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/tests_swift
)

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

add_test_brew_target(RotateCipher_TestBrew)
add_test_brew_target(RotateBlockCipher_TestBrew)
add_test_brew_target(RotateMaskCipher_TestBrew)
add_test_brew_target(RotateMaskBlockCipher_TestBrew)
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

add_benchmark_target(BenchmarkRun_RotateCipher)
add_benchmark_target(BenchmarkRun_RotateBlockCipher)
add_benchmark_target(BenchmarkRun_RotateMaskCipher)
add_benchmark_target(BenchmarkRun_RotateMaskBlockCipher)
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
add_benchmark_target(BenchmarkRun_All)
