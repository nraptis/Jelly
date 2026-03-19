# PeanutButter Cipher Final Report

This document is the final overview for the cipher mini-library in this repo.
It is meant to be readable by engineers, reviewers, and curious attackers.

## Disclaimer

I am allowing world governments to utilize and/or analyze these ciphers for internal encryption, or to study them for security purposes.

These ciphers are released **for entertainment only**. I personally believe this is a strong stack of ciphers, especially in layered use, but there is no formal cryptographic research, no peer review, and no external security certification backing that claim. Treat this library as an experimental engineering project, not as a standards-track cryptosystem.

All grades below are informal engineering judgments, not proofs.

## Global Rules

- `pLength` must be a multiple of `BLOCK_GRANULARITY = 192` for every cipher in this library.
- `pSource` and `pDestination` must not alias unless a specific caller-side wrapper copies into a safe temporary buffer first.
- Block-size ciphers only use these block sizes: `8`, `12`, `16`, `24`, `32`, `48`.
- Password, mask, noise, and jump tables are expected to be positive multiples of `16` where those parameters exist.
- Most table-driven ciphers repeat shorter tables across larger buffers.
- Masked variants use a one-byte bitmask unless otherwise noted.
- Matrix families are fixed-shape:
  - `16` means a `4 x 4` byte tile.
  - `64` means an `8 x 8` byte tile.
- Grades use this meaning:
  - `Cryptographic Worthiness, Member of Three-Tier Encryption`: how useful the cipher is as one component inside a layered `L1/L2/L3` stack.
  - `Cryptographic Worthiness, Individual Cipher`: how respectable it is if used mostly on its own.
  - `Overall Speed`: practical implementation speed in this repo.
  - `Crack Difficulty`: how annoying it is to reverse or model in isolation, assuming the attacker knows the algorithm.

## Family Overview

- **Rotation**: circular byte movement; cheap, predictable, useful as a spreader.
- **Splint**: front/back interleave; very good positional mixing for low cost.
- **Weave**: stride-based front/back exchange; more irregular than simple reverse.
- **Reverse**: symmetry and order inversion; weak alone, useful in stacks.
- **Ripple**: adjacent swap waves; cheap local disturbance.
- **Invert**: bit flip layer; low security alone, useful for stack asymmetry.
- **Password**: repeated or jumped password-driven math over the data stream.
- **Pepper**: external mask/noise table driven byte math, including jump-table variants.
- **Cascade**: XOR with rolling chunk feedback; state accumulates across the pass.
- **SwapGrid**: fixed tile row/column pair swaps with controlled unroll order.
- **SpiralGrid**: fixed tile border-spiral motion by amount.

## Masked Rule

For masked variants that preserve unmasked bits:

```text
out = (base & ~mask) | (moved & mask)
```

Bit example with `mask = 11110000`:

```text
base : 10101111
moved: 01010001
out  : 01011111
```

## Rotation Family

Rotation means circular shifting with wraparound.

```text
RotateCipher(+2)
[A0][B1][C2][D3][E4][F5]
=> [E4][F5][A0][B1][C2][D3]
```

Block contrast:

```text
RotateBlockCipher(blockSize=4, +1 block)
[a b c d][e f g h]
=> [e f g h][a b c d]

RotateBlockByteCipher(blockSize=4, +1 byte)
[a b c d][e f g h]
=> [d a b c][h e f g]
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `RotateCipher` | `amount: int`; `pLength % 192 == 0` | `A` | `C` | `A+` | `C` |
| `RotateBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `amount: int`; `pLength % blockSize == 0` | `A` | `B-` | `A+` | `C+` |
| `RotateBlockByteCipher` | `blockSize in {8,12,16,24,32,48}`; `amount: int`; `pLength % blockSize == 0` | `A` | `B` | `A+` | `B-` |
| `RotateMaskCipher` | `mask: byte`; `amount: int` | `A-` | `B-` | `A` | `B-` |
| `RotateMaskBlockCipher08/12/16/24/32/48` | fixed block size; `mask: byte`; `amount: int` | `A` | `B` | `A` | `B` |
| `RotateMaskByteBlockCipher08/12/16/24/32/48` | fixed block size; `mask: byte`; `amount: int` | `A` | `B+` | `A` | `B` |

## Splint Family

Splint splits the stream into halves and interleaves them. It is one of the cleaner “cheap but real” mixers in the set.

```text
SplintCipher
in : [a][b][c][d][e][f][g][h]
out: [a][e][b][f][c][g][d][h]
```

Decrypt recovers by pulling even positions first, then odd positions.

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `SplintCipher` | `pLength % 192 == 0` | `A+` | `B+` | `A+` | `B` |
| `SplintBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `pLength % blockSize == 0` | `A` | `B-` | `A+` | `B-` |
| `SplintByteBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `pLength % blockSize == 0` | `A` | `B` | `A+` | `B-` |
| `SplintMaskCipher` | `mask: byte` | `A+` | `B` | `A` | `B+` |
| `SplintMaskBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `mask: byte` | `A` | `B` | `A` | `B` |
| `SplintMaskByteBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `mask: byte` | `A` | `B+` | `A` | `B` |

## Weave Family

Weave repeatedly swaps from the front and back while skipping by stride. It behaves like a cached irregular permutation.

```text
Simple idea (count=1, frontStride=0, backStride=0)
in : [0][1][2][3][4][5][6][7]
out: [7][6][5][4][3][2][1][0]
```

Real use is more irregular because `count`, `frontStride`, and `backStride` skew which positions meet.

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `WeaveCipher` | `count: int`; `frontStride: int`; `backStride: int`; `pLength % 192 == 0` | `A` | `B` | `B+` | `B` |
| `WeaveBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `count/frontStride/backStride` | `A` | `B` | `A-` | `B` |
| `WeaveByteBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `count/frontStride/backStride` | `A-` | `B-` | `B+` | `C+` |
| `WeaveMaskCipher` | `mask: byte`; `count/frontStride/backStride` | `A` | `B` | `B` | `B+` |
| `WeaveMaskBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `mask: byte`; `count/frontStride/backStride` | `A` | `B+` | `B` | `B+` |
| `WeaveMaskByteBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `mask: byte`; `count/frontStride/backStride` | `A` | `B` | `B` | `B` |

## Reverse Family

Reverse is intentionally simple. Alone, it is weak. In a stack, it changes directionality, helps symmetry, and gives cheap reordering.

```text
ReverseCipher
[a][b][c][d][e][f] => [f][e][d][c][b][a]

ReverseBlockCipher(2)
[a b][c d][e f] => [e f][c d][a b]
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `ReverseCipher` | `pLength % 192 == 0` | `B` | `D` | `A+` | `D` |
| `ReverseBlockCipher08/12/16/24/32/48` | fixed block size | `B` | `D+` | `A+` | `D` |
| `ReverseBlockByteCipher08/12/16/24/32/48` | fixed block size | `B` | `D+` | `A+` | `D+` |
| `ReverseMaskCipher` | `mask: byte` | `B+` | `C-` | `A+` | `C-` |
| `ReverseMaskBlockCipher08/12/16/24/32/48` | fixed block size; `mask: byte` | `B+` | `C` | `A+` | `C` |
| `ReverseMaskByteBlockCipher08/12/16/24/32/48` | fixed block size; `mask: byte` | `B+` | `C` | `A+` | `C` |

## Ripple Family

Ripple performs repeated adjacent-pair swaps in alternating phases. It is local, cheap, and useful as a disturbance layer.

```text
RippleCipher(rounds=2)
start : [0][1][2][3][4][5]
phase0: [1][0][3][2][5][4]
phase1: [1][3][0][5][2][4]
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `RippleCipher` | `rounds: int`; `pLength % 192 == 0` | `B+` | `C` | `A+` | `C` |
| `RippleBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `rounds: int` | `B+` | `C+` | `A` | `C+` |
| `RippleMaskCipher` | `mask: byte`; `rounds: int` | `A-` | `B-` | `A` | `B-` |
| `RippleMaskBlockCipher` | `blockSize in {8,12,16,24,32,48}`; `mask: byte`; `rounds: int` | `A-` | `B` | `A-` | `B` |

## Invert Family

Invert is the simplest family.

```text
InvertCipher
00110101 => 11001010

InvertMaskCipher(mask=11110000)
00110101 => 11000101
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `InvertCipher` | `pLength % 192 == 0` | `C` | `F` | `A+` | `F` |
| `InvertMaskCipher` | `mask: byte` | `B` | `D` | `A+` | `D-` |

## Password Family

Password ciphers apply math using a caller-supplied password table. Jump variants step through password tiles by a jump stream instead of a flat walk.

Simple XOR idea:

```text
data    : [10][20][30][40]
password: [AA][BB][CC][DD]
out     : [BA][9B][FC][9D]
```

Jump idea:

```text
password tiles: [T0][T1][T2][T3]...
jumps         : [1][3][0][2]...
use order     : T0 -> T1 -> T0 -> T0 -> T2 ...
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `PasswordAddCipher` | `passwordLength % 16 == 0` | `A` | `B` | `A` | `B` |
| `PasswordSubtractCipher` | `passwordLength % 16 == 0` | `A` | `B` | `A` | `B` |
| `PasswordXORCipher` | `passwordLength % 16 == 0` | `A` | `B+` | `A+` | `B` |
| `PasswordJumpAddCipher` | `passwordLength % 16 == 0`; `jumpLength % 16 == 0` | `A` | `B+` | `A-` | `B+` |
| `PasswordJumpSubtractCipher` | `passwordLength % 16 == 0`; `jumpLength % 16 == 0` | `A` | `B+` | `A-` | `B+` |
| `PasswordJumpXORCipher` | `passwordLength % 16 == 0`; `jumpLength % 16 == 0` | `A+` | `A-` | `A` | `A-` |

## Pepper Family

Pepper is the table-driven family. Unlike older masked-bit experiments, the current Pepper ciphers are **full-byte transforms** using external mask/noise tables and, in some variants, jump tables.

Plain XOR Pepper:

```text
data : [d0][d1][d2][d3]
mask : [m0][m1][m2][m3]
out  : [d0^m0][d1^m1][d2^m2][d3^m3]
```

Jumped Pepper:

```text
mask chunks : [M0][M1][M2][M3]
noise chunks: [N0][N1][N2][N3]
mask jumps  : [1][3][0]...
noise jumps : [2][1][0]...

chunk 0 uses M0 + N0
chunk 1 uses M1 + N2
chunk 2 uses M0 + N3
...
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `PepperAddCipher` | `maskLength % 16 == 0` | `A` | `B` | `A+` | `B` |
| `PepperSubtractCipher` | `maskLength % 16 == 0` | `A` | `B` | `A+` | `B` |
| `PepperXORCipher` | `maskLength % 16 == 0` | `A` | `B+` | `A+` | `B+` |
| `PepperNoiseXORCipher` | `maskLength % 16 == 0`; `noiseLength % 16 == 0` | `A+` | `A-` | `A+` | `A-` |
| `PepperJumpXORCipher` | `maskLength % 16 == 0`; `jumpLength % 16 == 0` | `A+` | `A-` | `A` | `A-` |
| `PepperJumpNoiseXORCipher` | `maskLength % 16 == 0`; `noiseLength % 16 == 0`; `jumpLength % 16 == 0` | `A+` | `A` | `A` | `A` |
| `PepperDualJumpNoiseXORCipher` | `maskLength % 16 == 0`; `noiseLength % 16 == 0`; `maskJumpLength % 16 == 0`; `noiseJumpLength % 16 == 0` | `A+` | `A` | `A-` | `A` |

## Cascade Family

Cascade adds rolling state. It is one of the few families here where the pass accumulates change across chunk boundaries.

```text
chunk0 -> xor with mask and seed state -> produce feedback0
chunk1 -> xor with mask and feedback0 -> produce feedback1
chunk2 -> xor with mask and feedback1 -> ...
```

That is not formal diffusion analysis, but it does make local reasoning harder than a pure permutation family.

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `CascadeXORCipher` | `maskLength % 16 == 0` | `A+` | `B+` | `A-` | `B+` |
| `CascadeJumpXORCipher` | `maskLength % 16 == 0`; `jumpLength % 16 == 0` | `A+` | `A-` | `A-` | `A-` |

## SwapGrid Family

SwapGrid is a fixed matrix family. It rolls bytes into a tile, swaps row or column pairs, then unrolls either horizontally or vertically.

`VV16` style example:

```text
in:
[ a b c d ]
[ e f g h ]
[ i j k l ]
[ m n o p ]

after vertical row-pair swap:
[ e f g h ]
[ a b c d ]
[ m n o p ]
[ i j k l ]
```

`VH16` changes the unroll order, not the swap rule. The `64` variants do the same idea on `8 x 8` tiles.

Bit-grid reminder for masked SwapGrid:

```text
base byte at slot X : 10101100
moved byte into X   : 01010011
mask                : 11110000
out                 : 01011100
```

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `SwapGridVV16` | fixed `16`-byte tile | `A` | `B-` | `A+` | `B-` |
| `SwapGridVH16` | fixed `16`-byte tile | `A` | `B-` | `A+` | `B-` |
| `SwapGridHV16` | fixed `16`-byte tile | `A` | `B-` | `A+` | `B-` |
| `SwapGridHH16` | fixed `16`-byte tile | `A` | `B-` | `A+` | `B-` |
| `SwapGridVV64` | fixed `64`-byte tile | `A` | `B` | `A+` | `B` |
| `SwapGridVH64` | fixed `64`-byte tile | `A` | `B` | `A+` | `B` |
| `SwapGridHV64` | fixed `64`-byte tile | `A` | `B` | `A+` | `B` |
| `SwapGridHH64` | fixed `64`-byte tile | `A` | `B` | `A+` | `B` |
| `SwapGridMaskVV16` | fixed `16`-byte tile; `mask: byte` | `A` | `B` | `A+` | `B` |
| `SwapGridMaskVH16` | fixed `16`-byte tile; `mask: byte` | `A` | `B` | `A+` | `B` |
| `SwapGridMaskHV16` | fixed `16`-byte tile; `mask: byte` | `A` | `B` | `A+` | `B` |
| `SwapGridMaskHH16` | fixed `16`-byte tile; `mask: byte` | `A` | `B` | `A+` | `B` |
| `SwapGridMaskVV64` | fixed `64`-byte tile; `mask: byte` | `A+` | `B+` | `A+` | `B+` |
| `SwapGridMaskVH64` | fixed `64`-byte tile; `mask: byte` | `A+` | `B+` | `A+` | `B+` |
| `SwapGridMaskHV64` | fixed `64`-byte tile; `mask: byte` | `A+` | `B+` | `A+` | `B+` |
| `SwapGridMaskHH64` | fixed `64`-byte tile; `mask: byte` | `A+` | `B+` | `A+` | `B+` |

## SpiralGrid Family

SpiralGrid takes the outer border of a fixed tile and rotates it around the border path by `pAmount`.

`4 x 4` example:

```text
[ a b c d ]
[ e f g h ]
[ i j k l ]
[ m n o p ]

Spiral by 2 =>

[ i e a b ]
[ m f g c ]
[ n j k d ]
[ o p l h ]
```

The `V` and `H` variants differ by unroll orientation. Masked variants keep only masked bits from the moved byte.

| Cipher | Valid Parameters | Three-Tier | Individual | Speed | Crack |
|---|---|---:|---:|---:|---:|
| `SpiralGridH16` | fixed `16`-byte tile; `amount: int` | `A` | `B` | `A` | `B` |
| `SpiralGridV16` | fixed `16`-byte tile; `amount: int` | `A` | `B` | `A` | `B` |
| `SpiralGridH64` | fixed `64`-byte tile; `amount: int` | `A+` | `B+` | `A-` | `B+` |
| `SpiralGridV64` | fixed `64`-byte tile; `amount: int` | `A+` | `B+` | `A-` | `B+` |
| `SpiralGridMaskH16` | fixed `16`-byte tile; `mask: byte`; `amount: int` | `A` | `B+` | `A` | `B+` |
| `SpiralGridMaskV16` | fixed `16`-byte tile; `mask: byte`; `amount: int` | `A` | `B+` | `A` | `B+` |
| `SpiralGridMaskH64` | fixed `64`-byte tile; `mask: byte`; `amount: int` | `A+` | `A-` | `A-` | `A-` |
| `SpiralGridMaskV64` | fixed `64`-byte tile; `mask: byte`; `amount: int` | `A+` | `A-` | `A-` | `A-` |

## Final Notes

- Best individual families here, in my opinion: `PepperJumpNoiseXOR`, `PepperDualJumpNoiseXOR`, `CascadeJumpXOR`, masked `SwapGrid64`, masked `SpiralGrid64`, and the stronger Splint variants.
- Best cheap utility families for stacking: `Rotate*`, `Reverse*`, `Ripple*`, `Invert*`.
- Best “teeth” in a three-tier stack: Splint + Pepper/Cascade + one matrix family.
- Weakest alone: `InvertCipher`, `ReverseCipher`, plain `RippleCipher`.
- Strongest idea in the library is not any one cipher. It is the ability to build a long asymmetric stack of fast, mutually different transforms across `L1`, `L2`, and `L3`.

If this library ever graduates from experiment to serious security work, the next step is not “add more ciphers.” The next step is formal review, attack modeling, interoperability hardening, and repeatable test vectors.
