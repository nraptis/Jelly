//
//  SplintMaskByteBlockCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/18/23.
//

import Foundation

struct SplintMaskByteBlockCipher: Cipher {
    let blockSize: Int
    let mask: UInt8
    init(blockSize: Int = 12, mask: UInt8 = 243) {
        self.blockSize = blockSize
        self.mask = mask
    }
    
    func encrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        if blocks.count <= 0 { return data }
        for blockIndex in blocks.indices {
            var list1 = [UInt8]()
            var list2 = [UInt8]()
            let half = (blocks[blockIndex].count + 1) / 2
            var index = 0
            while index < half {
                list1.append(blocks[blockIndex][index])
                index += 1
            }
            while index < blocks[blockIndex].count {
                list2.append(blocks[blockIndex][index])
                index += 1
            }
            index = 0
            var index1 = 0
            var index2 = 0
            while index1 < list1.count && index2 < list2.count {
                blocks[blockIndex][index] = list1[index1]
                index += 1
                index1 += 1
                blocks[blockIndex][index] = list2[index2]
                index += 1
                index2 += 1
            }
            while index1 < list1.count {
                blocks[blockIndex][index] = list1[index1]
                index += 1
                index1 += 1
            }
            while index2 < list2.count {
                blocks[blockIndex][index] = list2[index2]
                index += 1
                index2 += 1
            }
        }
        let antimask = ~mask
        var blockIndex = 0
        var index = 0
        while blockIndex < blocks.count {
            var byteIndex = 0
            while byteIndex < blocks[blockIndex].count {
                dataBytes[index] &= antimask
                dataBytes[index] |= (blocks[blockIndex][byteIndex] & mask)
                byteIndex += 1
                index += 1
            }
            blockIndex += 1
        }
        blocks.removeAll()
        return Data(dataBytes)
    }
    func decrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        if blocks.count <= 0 { return data }
        for blockIndex in blocks.indices {
            var list1 = [UInt8]()
            var list2 = [UInt8]()
            var index = 0
            while index < blocks[blockIndex].count {
                if (index & 1) == 0 {
                    list1.append(blocks[blockIndex][index])
                } else {
                    list2.append(blocks[blockIndex][index])
                }
                index += 1
            }
            index = 0
            var index1 = 0
            while index1 < list1.count {
                blocks[blockIndex][index] = list1[index1]
                index += 1
                index1 += 1
            }
            var index2 = 0
            while index2 < list2.count {
                blocks[blockIndex][index] = list2[index2]
                index += 1
                index2 += 1
            }
        }
        let antimask = ~mask
        var blockIndex = 0
        var index = 0
        while blockIndex < blocks.count {
            var byteIndex = 0
            while byteIndex < blocks[blockIndex].count {
                dataBytes[index] &= antimask
                dataBytes[index] |= (blocks[blockIndex][byteIndex] & mask)
                byteIndex += 1
                index += 1
            }
            blockIndex += 1
        }
        blocks.removeAll()
        return Data(dataBytes)
    }
}
