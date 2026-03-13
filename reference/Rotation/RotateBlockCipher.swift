//
//  RotateBlockCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct RotateBlockCipher: Cipher {
    let blockSize: Int
    let shift: Int
    init(blockSize: Int = 4, shift: Int = 1) {
        self.blockSize = blockSize
        self.shift = shift
    }
    
    func encrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        dataBytes.removeAll()
        if blocks.count <= 0 { return data }
        var dataBytesRotated = [UInt8](repeating: 0, count: blockSize)
        for blockIndex in blocks.indices {
            var rotation = shift % blocks[blockIndex].count
            if rotation < 0 { rotation += blocks[blockIndex].count }
            var sourceByteIndex = rotation
            var destByteIndex = 0
            while sourceByteIndex < blocks[blockIndex].count {
                dataBytesRotated[destByteIndex] = blocks[blockIndex][sourceByteIndex]
                sourceByteIndex += 1
                destByteIndex += 1
            }
            sourceByteIndex = 0
            while sourceByteIndex < rotation {
                dataBytesRotated[destByteIndex] = blocks[blockIndex][sourceByteIndex]
                sourceByteIndex += 1
                destByteIndex += 1
            }
            for byteIndex in blocks[blockIndex].indices {
                blocks[blockIndex][byteIndex] = dataBytesRotated[byteIndex]
            }
        }
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
    
    func decrypt(data: Data) throws -> Data {
        if data.count <= 0 { return data }
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        if blocks.count <= 0 { return data }
        dataBytes.removeAll()
        var dataBytesRotated = [UInt8](repeating: 0, count: blockSize)
        for blockIndex in blocks.indices {
            var rotation = (-shift) % blocks[blockIndex].count
            if rotation < 0 { rotation += blocks[blockIndex].count }
            var sourceByteIndex = rotation
            var destByteIndex = 0
            while sourceByteIndex < blocks[blockIndex].count {
                dataBytesRotated[destByteIndex] = blocks[blockIndex][sourceByteIndex]
                sourceByteIndex += 1
                destByteIndex += 1
            }
            sourceByteIndex = 0
            while sourceByteIndex < rotation {
                dataBytesRotated[destByteIndex] = blocks[blockIndex][sourceByteIndex]
                sourceByteIndex += 1
                destByteIndex += 1
            }
            for byteIndex in blocks[blockIndex].indices {
                blocks[blockIndex][byteIndex] = dataBytesRotated[byteIndex]
            }
        }
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
}
