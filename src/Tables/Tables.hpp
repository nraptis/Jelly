#ifndef PEANUTBUTTER_TABLES_TABLES_HPP_
#define PEANUTBUTTER_TABLES_TABLES_HPP_

#include <array>
#include <cstddef>
#include <cstdint>

#include "../PeanutButter.hpp"

namespace peanutbutter::tables {

class Tables final {
 public:
  struct TableDescriptor {
    const char* mName;
    unsigned char* mData;
    std::size_t mSize;
  };

  static constexpr std::size_t kL1TableCount = 12u;
  static constexpr std::size_t kL2TableCount = 6u;
  static constexpr std::size_t kL3TableCount = 4u;
  static constexpr std::size_t kTableCount =
      kL1TableCount + kL2TableCount + kL3TableCount;

  static unsigned char gTableL1_A[BLOCK_SIZE_L1];
  static unsigned char gTableL1_B[BLOCK_SIZE_L1];
  static unsigned char gTableL1_C[BLOCK_SIZE_L1];
  static unsigned char gTableL1_D[BLOCK_SIZE_L1];
  static unsigned char gTableL1_E[BLOCK_SIZE_L1];
  static unsigned char gTableL1_F[BLOCK_SIZE_L1];
  static unsigned char gTableL1_G[BLOCK_SIZE_L1];
  static unsigned char gTableL1_H[BLOCK_SIZE_L1];
  static unsigned char gTableL1_I[BLOCK_SIZE_L1];
  static unsigned char gTableL1_J[BLOCK_SIZE_L1];
  static unsigned char gTableL1_K[BLOCK_SIZE_L1];
  static unsigned char gTableL1_L[BLOCK_SIZE_L1];

  static unsigned char gTableL2_A[BLOCK_SIZE_L2];
  static unsigned char gTableL2_B[BLOCK_SIZE_L2];
  static unsigned char gTableL2_C[BLOCK_SIZE_L2];
  static unsigned char gTableL2_D[BLOCK_SIZE_L2];
  static unsigned char gTableL2_E[BLOCK_SIZE_L2];
  static unsigned char gTableL2_F[BLOCK_SIZE_L2];

  static unsigned char gTableL3_A[BLOCK_SIZE_L3];
  static unsigned char gTableL3_B[BLOCK_SIZE_L3];
  static unsigned char gTableL3_C[BLOCK_SIZE_L3];
  static unsigned char gTableL3_D[BLOCK_SIZE_L3];

  static const std::array<TableDescriptor, kTableCount>& All();
  static void Seed(std::uint32_t pSeed);
};

}  // namespace peanutbutter::tables

#endif  // PEANUTBUTTER_TABLES_TABLES_HPP_
