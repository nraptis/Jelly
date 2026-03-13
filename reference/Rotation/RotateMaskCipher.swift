//
//  RotateMaskCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct RotateMaskCipher: Cipher {
    let mask: UInt8
    let shift: Int
    init(mask: UInt8 = 60, shift: Int = 1) {
        self.mask = mask
        self.shift = shift
    }
    
    func encrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        if dataBytes.count <= 0 { return data }
        var dataBytesRotated = [UInt8]()
        dataBytesRotated.reserveCapacity(dataBytes.count)
        var rotation = shift % dataBytes.count
        if rotation < 0 { rotation += dataBytes.count }
        var index = rotation
        while index < dataBytes.count {
            dataBytesRotated.append(dataBytes[index])
            index += 1
        }
        index = 0
        while index < rotation {
            dataBytesRotated.append(dataBytes[index])
            index += 1
        }
        let antimask = ~mask
        for byteIndex in dataBytes.indices {
            dataBytes[byteIndex] &= antimask
            dataBytes[byteIndex] |= (dataBytesRotated[byteIndex] & mask)
        }
        return Data(dataBytes)
    }
    
    func decrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        if dataBytes.count <= 0 { return data }
        var dataBytesRotated = [UInt8]()
        dataBytesRotated.reserveCapacity(dataBytes.count)
        var rotation = (-shift) % dataBytes.count
        if rotation < 0 { rotation += dataBytes.count }
        var index = rotation
        while index < dataBytes.count {
            dataBytesRotated.append(dataBytes[index])
            index += 1
        }
        index = 0
        while index < rotation {
            dataBytesRotated.append(dataBytes[index])
            index += 1
        }
        let antimask = ~mask
        for byteIndex in dataBytes.indices {
            dataBytes[byteIndex] &= antimask
            dataBytes[byteIndex] |= (dataBytesRotated[byteIndex] & mask)
        }
        return Data(dataBytes)
    }
}
