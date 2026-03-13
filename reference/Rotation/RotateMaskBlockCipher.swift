//
//  RotateMaskBlockCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct RotateMaskBlockCipher: Cipher {
    let blockSize: Int
    let mask: UInt8
    let shift: Int
    init(blockSize: Int = 5, mask: UInt8 = 240, shift: Int = -1) {
        self.blockSize = blockSize
        self.mask = mask
        self.shift = shift
    }
    
    func encrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        dataBytes.removeAll()
        if blocks.count <= 0 { return data }
        for blockIndex in blocks.indices {
            var dataBytesRotated = [UInt8]()
            dataBytesRotated.reserveCapacity(blocks[blockIndex].count)
            var rotation = shift % blocks[blockIndex].count
            if rotation < 0 { rotation += blocks[blockIndex].count }
            var index = rotation
            while index < blocks[blockIndex].count {
                dataBytesRotated.append(blocks[blockIndex][index])
                index += 1
            }
            index = 0
            while index < rotation {
                dataBytesRotated.append(blocks[blockIndex][index])
                index += 1
            }
            let antimask = ~mask
            for byteIndex in blocks[blockIndex].indices {
                blocks[blockIndex][byteIndex] &= antimask
                blocks[blockIndex][byteIndex] |= (dataBytesRotated[byteIndex] & mask)
            }
        }
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
    
    func decrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        dataBytes.removeAll()
        if blocks.count <= 0 { return data }
        for blockIndex in blocks.indices {
            var dataBytesRotated = [UInt8]()
            dataBytesRotated.reserveCapacity(blocks[blockIndex].count)
            var rotation = (-shift) % blocks[blockIndex].count
            if rotation < 0 { rotation += blocks[blockIndex].count }
            var index = rotation
            while index < blocks[blockIndex].count {
                dataBytesRotated.append(blocks[blockIndex][index])
                index += 1
            }
            index = 0
            while index < rotation {
                dataBytesRotated.append(blocks[blockIndex][index])
                index += 1
            }
            let antimask = ~mask
            for byteIndex in blocks[blockIndex].indices {
                blocks[blockIndex][byteIndex] &= antimask
                blocks[blockIndex][byteIndex] |= (dataBytesRotated[byteIndex] & mask)
            }
        }
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
}
