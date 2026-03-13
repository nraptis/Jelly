//
//  WeaveMaskBlockCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct WeaveMaskBlockCipher: Cipher {
    let blockSize: Int
    let mask: UInt8
    let count: Int
    let frontStride: Int
    let backStride: Int
    init(blockSize: Int = 11, mask: UInt8 = 204, count: Int = 1, frontStride: Int = 1, backStride: Int = 0) {
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
        var front = 0
        var back = blocks.count - 1
        if blocks[blocks.count - 1].count != blockSize {
            back -= 1
        }
        let antimask = ~mask
        var maskedA = [UInt8](repeating: 0, count: blockSize)
        var maskedB = [UInt8](repeating: 0, count: blockSize)
        while true {
            var swaps = count
            while swaps > 0 && front < back {
                for index in 0..<blockSize {
                    maskedA[index] = blocks[front][index] & mask
                    blocks[front][index] &= antimask
                }
                for index in 0..<blockSize {
                    maskedB[index] = blocks[back][index] & mask
                    blocks[back][index] &= antimask
                }
                for index in 0..<blockSize {
                    blocks[front][index] |= maskedB[index]
                    blocks[back][index] |= maskedA[index]
                }
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
        return Data(BlockHelper.dataFromBlocks(blocks: blocks))
    }
}
