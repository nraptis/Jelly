//
//  WeaveMaskByteBlockCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct WeaveMaskByteBlockCipher: Cipher {
    let blockSize: Int
    let mask: UInt8
    let count: Int
    let frontStride: Int
    let backStride: Int
    init(blockSize: Int = 12, mask: UInt8 = 51, count: Int = 1, frontStride: Int = 1, backStride: Int = 0) {
        self.blockSize = blockSize
        self.mask = mask
        self.count = count
        self.frontStride = frontStride
        self.backStride = backStride
    }
    
    func encrypt(data: Data) throws -> Data {
        try process(data: data)
    }
    
    func decrypt(data: Data) throws -> Data {
        try process(data: data)
    }
    
    private func process(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        var blocks = BlockHelper.dataToBlocks(data: dataBytes, blockSize: blockSize)
        dataBytes.removeAll()
        if blocks.count <= 0 { return data }
        var count = count
        if count < 1 { count = 1 }
        let antimask = ~mask
        for blockIndex in blocks.indices {
            var front = 0
            var back = blocks[blockIndex].count - 1
            while true {
                var swaps = count
                while swaps > 0 && front < back {
                    var byteA = blocks[blockIndex][front]
                    var byteB = blocks[blockIndex][back]
                    let maskedA = byteA & mask
                    let maskedB = byteB & mask
                    byteA &= antimask
                    byteB &= antimask
                    byteA |= maskedB
                    byteB |= maskedA
                    blocks[blockIndex][front] = byteA
                    blocks[blockIndex][back] = byteB
                    swaps -= 1
                    front += 1
                    back -= 1
                }
                if front >= back { break }
                var skips = frontStride
                while skips > 0 && front < back {
                    skips -= 1
                    front += 1
                }
                if front >= back { break }
                skips = backStride
                while skips > 0 && front < back {
                    skips -= 1
                    back -= 1
                }
                if front >= back { break }
            }
        }
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
}
