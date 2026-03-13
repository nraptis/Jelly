//
//  SplintCipher.swift
//  EncryptionTools
//
//  Created by Nicky Taylor on 11/1/23.
//

import Foundation

struct SplintCipher: Cipher {
    func encrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        var list1 = [UInt8]()
        var list2 = [UInt8]()
        let half = (dataBytes.count + 1) / 2
        var index = 0
        while index < half {
            list1.append(dataBytes[index])
            index += 1
        }
        while index < dataBytes.count {
            list2.append(dataBytes[index])
            index += 1
        }
        index = 0
        var index1 = 0
        var index2 = 0
        while index1 < list1.count && index2 < list2.count {
            dataBytes[index] = list1[index1]
            index += 1
            index1 += 1
            dataBytes[index] = list2[index2]
            index += 1
            index2 += 1
        }
        while index1 < list1.count {
            dataBytes[index] = list1[index1]
            index += 1
            index1 += 1
        }
        while index2 < list2.count {
            dataBytes[index] = list2[index2]
            index += 1
            index2 += 1
        }
        return Data(dataBytes)
    }
    
    func decrypt(data: Data) throws -> Data {
        var dataBytes = [UInt8](data)
        var list1 = [UInt8]()
        var list2 = [UInt8]()
        var index = 0
        while index < dataBytes.count {
            if (index & 1) == 0 {
                list1.append(dataBytes[index])
            } else {
                list2.append(dataBytes[index])
            }
            index += 1
        }
        index = 0
        var index1 = 0
        while index1 < list1.count {
            dataBytes[index] = list1[index1]
            index += 1
            index1 += 1
        }
        var index2 = 0
        while index2 < list2.count {
            dataBytes[index] = list2[index2]
            index += 1
            index2 += 1
        }
        return Data(dataBytes)
    }
}
