//
//  RotateCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct RotateCipher: Cipher {
    let shift: Int
    init(shift: Int = 1) {
        self.shift = shift
    }
    
    func encrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        if dataBytes.count <= 0 { return data }
        var rotate = shift % dataBytes.count
        if rotate < 0 {
            rotate += dataBytes.count
        }
        var start = 0
        var end = dataBytes.count - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        start = 0
        end = rotate - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        start = rotate
        end = dataBytes.count - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        return Data(dataBytes)
    }
    
    func decrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        if dataBytes.count <= 0 { return data }
        var rotate = (-shift) % dataBytes.count
        if rotate < 0 {
            rotate += dataBytes.count
        }
        var start = 0
        var end = dataBytes.count - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        start = 0
        end = rotate - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        start = rotate
        end = dataBytes.count - 1
        while start < end {
            dataBytes.swapAt(start, end)
            start += 1
            end -= 1
        }
        return Data(dataBytes)
    }
}
