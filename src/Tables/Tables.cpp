#include "Tables.hpp"

#include <array>
#include <cstdint>

namespace peanutbutter::tables {

unsigned char Tables::gTableL1_A[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_B[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_C[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_D[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_E[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_F[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_G[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_H[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_I[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_J[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_K[BLOCK_SIZE_L1];
unsigned char Tables::gTableL1_L[BLOCK_SIZE_L1];

unsigned char Tables::gTableL2_A[BLOCK_SIZE_L2];
unsigned char Tables::gTableL2_B[BLOCK_SIZE_L2];
unsigned char Tables::gTableL2_C[BLOCK_SIZE_L2];
unsigned char Tables::gTableL2_D[BLOCK_SIZE_L2];
unsigned char Tables::gTableL2_E[BLOCK_SIZE_L2];
unsigned char Tables::gTableL2_F[BLOCK_SIZE_L2];

unsigned char Tables::gTableL3_A[BLOCK_SIZE_L3];
unsigned char Tables::gTableL3_B[BLOCK_SIZE_L3];
unsigned char Tables::gTableL3_C[BLOCK_SIZE_L3];
unsigned char Tables::gTableL3_D[BLOCK_SIZE_L3];

namespace {

std::uint32_t NextState(std::uint32_t pState) {
  pState ^= (pState << 13);
  pState ^= (pState >> 17);
  pState ^= (pState << 5);
  return pState;
}

void Fill(unsigned char* pDestination,
          std::size_t pLength,
          std::uint32_t pSeed,
          std::uint32_t pSalt) {
  std::uint32_t aState = pSeed ^ pSalt;
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    aState = NextState(aState + static_cast<std::uint32_t>(aIndex) + pSalt);
    pDestination[aIndex] = static_cast<unsigned char>((aState >> 11) & 0xFFu);
  }
}

}  // namespace

const std::array<Tables::TableDescriptor, Tables::kTableCount>& Tables::All() {
  static const std::array<TableDescriptor, kTableCount> kTables = {{
      {"gTableL1_A", gTableL1_A, BLOCK_SIZE_L1},
      {"gTableL1_B", gTableL1_B, BLOCK_SIZE_L1},
      {"gTableL1_C", gTableL1_C, BLOCK_SIZE_L1},
      {"gTableL1_D", gTableL1_D, BLOCK_SIZE_L1},
      {"gTableL1_E", gTableL1_E, BLOCK_SIZE_L1},
      {"gTableL1_F", gTableL1_F, BLOCK_SIZE_L1},
      {"gTableL1_G", gTableL1_G, BLOCK_SIZE_L1},
      {"gTableL1_H", gTableL1_H, BLOCK_SIZE_L1},
      {"gTableL1_I", gTableL1_I, BLOCK_SIZE_L1},
      {"gTableL1_J", gTableL1_J, BLOCK_SIZE_L1},
      {"gTableL1_K", gTableL1_K, BLOCK_SIZE_L1},
      {"gTableL1_L", gTableL1_L, BLOCK_SIZE_L1},
      {"gTableL2_A", gTableL2_A, BLOCK_SIZE_L2},
      {"gTableL2_B", gTableL2_B, BLOCK_SIZE_L2},
      {"gTableL2_C", gTableL2_C, BLOCK_SIZE_L2},
      {"gTableL2_D", gTableL2_D, BLOCK_SIZE_L2},
      {"gTableL2_E", gTableL2_E, BLOCK_SIZE_L2},
      {"gTableL2_F", gTableL2_F, BLOCK_SIZE_L2},
      {"gTableL3_A", gTableL3_A, BLOCK_SIZE_L3},
      {"gTableL3_B", gTableL3_B, BLOCK_SIZE_L3},
      {"gTableL3_C", gTableL3_C, BLOCK_SIZE_L3},
      {"gTableL3_D", gTableL3_D, BLOCK_SIZE_L3},
  }};
  return kTables;
}

void Tables::Seed(std::uint32_t pSeed) {
  const auto& aTables = All();
  for (std::size_t aIndex = 0; aIndex < aTables.size(); ++aIndex) {
    Fill(aTables[aIndex].mData, aTables[aIndex].mSize, pSeed,
         0x9E3779B9u * static_cast<std::uint32_t>(aIndex + 1u));
  }
}

}  // namespace peanutbutter::tables
