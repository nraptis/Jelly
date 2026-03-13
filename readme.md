# Jelly Cipher Quick Guide

This file is a practical, human-readable map of the cipher set in this repo.
Masked variants are documented inline with their base cipher (not as separate families).

`SB_CIPHER_LENGTH_GRANULARITY = 192` is a hard rule for all ciphers here.

================================================================
## Masked Rule (Used Everywhere)

Masked variants keep unmasked bits in place and move only masked bits.

```
out = (base & ~mask) | (moved & mask)
```

Bit-level example (`mask = 11110000`):

```
base : 10101111
moved: 01010001
mask : 11110000
~mask: 00001111

out  : 01011111
       ^^^^---- high nibble from moved
           ^^^^ low nibble from base
```

================================================================
## Rotation Family

Pairs:
- `RotateCipher` <-> `RotateMaskCipher`
- `RotateBlockCipher` <-> `RotateMaskBlockCipher`
- `RotateBlockByteCipher` <-> `RotateMaskByteBlockCipher`

Meaning:
- Rotate bytes by an amount with wraparound.
- `Block`: rotate whole blocks.
- `BlockByte`: rotate bytes inside each block.

Example (`RotateCipher`, shift `+2`):

```
[A0][B1][C2][D3][E4][F5]  =>  [E4][F5][A0][B1][C2][D3]
```

Block contrast (`blockSize = 4`, shift `+1 block`):

```
Block      : [a b c d][e f g h] => [e f g h][a b c d]
BlockByte  : [a b c d][e f g h] => [d a b c][h e f g]
```

================================================================
## Splint Family

Pairs:
- `SplintCipher` <-> `SplintMaskCipher`
- `SplintBlockCipher` <-> `SplintMaskBlockCipher`
- `SplintByteBlockCipher` <-> `SplintMaskByteBlockCipher`

Meaning:
- Splits source into front/back halves and interleaves them.
- Decrypt collects even positions, then odd positions.

Example (`SplintCipher`):

```
in : [a][b][c][d][e][f][g][h]
out: [a][e][b][f][c][g][d][h]
```

Insight:
- This is a strong byte-position mixer with cheap inverse logic.

================================================================
## Weave Family

Pairs:
- `WeaveCipher` <-> `WeaveMaskCipher`
- `WeaveBlockCipher` <-> `WeaveMaskBlockCipher`
- `WeaveByteBlockCipher` <-> `WeaveMaskByteBlockCipher`

Meaning:
- Repeated front/back swapping with configurable swap count and strides.
- Equivalent to applying a cached permutation map.

Simple example (`count=1`, `frontStride=0`, `backStride=0`):

```
in : [0][1][2][3][4][5][6][7]
out: [7][6][5][4][3][2][1][0]
```

Insight:
- `count/stride` parameters tune "how hard" front/back entangle.

================================================================
## Reverse Family

Pairs:
- `ReverseCipher` <-> `ReverseMaskCipher`
- `ReverseBlockCipher` <-> `ReverseMaskBlockCipher`
- `ReverseBlockByteCipher` <-> `ReverseMaskByteBlockCipher`

Meaning:
- `Reverse`: reverse all bytes.
- `Block`: reverse block order.
- `BlockByte`: reverse bytes inside each block.

Example:

```
Reverse         : [a][b][c][d][e][f] => [f][e][d][c][b][a]
ReverseBlock(2) : [a b][c d][e f]    => [e f][c d][a b]
```

================================================================
## Ripple Family

Pairs:
- `RippleCipher` <-> `RippleMaskCipher`
- `RippleBlockCipher` <-> `RippleMaskBlockCipher`

Meaning:
- Alternating adjacent swaps by phase (`0` then `1`) for `rounds`.
- Block variants do the same idea at block granularity.

Example (`rounds=2` on bytes):

```
start : [0][1][2][3][4][5]
phase0: [1][0][3][2][5][4]
phase1: [1][3][0][5][2][4]
```

Insight:
- Ripple is cheap per pass but highly sensitive to rounds and block size.

================================================================
## Invert Family

Pair:
- `InvertCipher` <-> `InvertMaskCipher`

Meaning:
- `InvertCipher`: `byte = ~byte`
- Masked version inverts only masked bits.

Example:

```
in :  00110101
out:  11001010
```

Masked example (`mask = 11110000`):

```
in :  00110101
out:  11000101
```

================================================================
## Password Family

Cipher:
- `PasswordCipher` (no masked variant)

Meaning:
- Expands password bytes into a worker buffer and XORs source against it.
- Same function for seal/unseal (`XOR` symmetry).

Example:

```
data    : [10][20][30][40]
password: [AA][BB][CC][DD]
out     : [BA][9B][FC][9D]
```

================================================================
## SwapGrid Family

Pairs (16-tile):
- `SwapGridVV16` <-> `SwapGridMaskVV16`
- `SwapGridVH16` <-> `SwapGridMaskVH16`
- `SwapGridHV16` <-> `SwapGridMaskHV16`
- `SwapGridHH16` <-> `SwapGridMaskHH16`

Pairs (64-tile):
- `SwapGridVV64` <-> `SwapGridMaskVV64`
- `SwapGridVH64` <-> `SwapGridMaskVH64`
- `SwapGridHV64` <-> `SwapGridMaskHV64`
- `SwapGridHH64` <-> `SwapGridMaskHH64`

Meaning:
- First letter: swap order (`V` rows, `H` columns).
- Second letter: unroll/read order (`V` or `H`).

4x4 example (`VV16` style, row-major view):

```
in:
[ a b c d ]
[ e f g h ]
[ i j k l ]
[ m n o p ]

after vertical row-pair swap + vertical unroll:
[ e f g h ]
[ a b c d ]
[ m n o p ]
[ i j k l ]
```

Bit-grid reminder for masked SwapGrid:

```
base byte at slot X : 10101100
moved byte into X   : 01010011
mask                : 11110000
out                 : 01011100
```

Use masked SwapGrid when you want position shuffling without fully replacing each byte.

================================================================
## SpiralGrid Family

Pairs:
- `SpiralGridH16` <-> `SpiralGridMaskH16`
- `SpiralGridV16` <-> `SpiralGridMaskV16`
- `SpiralGridH64` <-> `SpiralGridMaskH64`
- `SpiralGridV64` <-> `SpiralGridMaskV64`

Meaning:
- Rotates perimeter rings of fixed tiles by `pAmount`.
- `H` and `V` variants use different ring index orderings.

4x4 example (`SpiralGrid*16`, `pAmount = +2`, ring rotate):

```
in:
[ a b c d ]
[ e f g h ]
[ i j k l ]
[ m n o p ]

out:
[ i e a b ]
[ m k j c ]
[ n g f d ]
[ o p l h ]
```

Bit-stack visual (sparse, conceptual):

```
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
11110000|00001101|10101001|
```

After masked spiral: ring positions move, but only masked bit planes move.

================================================================
## Practical Notes

- If you need easy invertibility plus parameterized movement, start with Rotation/Weave.
- If you need clear grid-local diffusion, use SwapGrid/SpiralGrid variants.
- If you need bit-plane-preserving transforms, use masked variants.
- For performance comparisons, always benchmark at L1/L2/L3 lengths and by mode (`Soft`, `Simd`, `Neon`).
