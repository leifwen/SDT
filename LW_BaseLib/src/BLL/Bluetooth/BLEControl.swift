//
//  BLEControl.swift
//  SDT
//
//  Created by Leif Wen on 2020/9/12.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

import Foundation
import CoreBluetooth

var bleInit: Bool = false
var BLEDevs: BLEDevicesManager?

func BLE_Init(){
	if bleInit == false{
		BLEDevs = BLEDevicesManager()
		bleInit = true;
	}
}

private func RegMatch(for regex: String, in text: String) -> [String] {
    do {
        let regex = try NSRegularExpression(pattern: regex)
        let results = regex.matches(in: text,
                                    range: NSRange(text.startIndex..., in: text))
        return results.map {
            String(text[Range($0.range, in: text)!])
        }
	} catch _ {
        return []
    }
}

enum InquireType: Int {
	case name = 0
	case UUID128 = 1
	case UUID16 = 2
	case index = 3
}

func CheckInquireType(by input: String) -> InquireType {
	let regExp128 = "^[0-9a-fA-f]{8}-[0-9a-fA-f]{4}-[0-9a-fA-f]{4}-[0-9a-fA-f]{4}-[0-9a-fA-f]{12}$"
	let regExp16 = "^[0-9a-fA-f]{4}$"
	let regExpInt = "^[0-9]{1,2}$"
	let res128 = RegMatch(for: regExp128, in: input)
	let res16 = RegMatch(for: regExp16, in: input)
	let resInt = RegMatch(for: regExpInt, in: input)

	if res128.count > 0{
		return InquireType.UUID128
	}
	if res16.count > 0{
		return InquireType.UUID16
	}
	if resInt.count > 0 {
		return InquireType.index
	}
	return InquireType.name
}

@_cdecl("Swift_BLE_Scan")
func BLE_Scan(uuid: UnsafePointer<CChar>!) -> CInt {
	BLE_Init()
	
	let sUUID = String.init(cString: uuid)
	switch CheckInquireType(by: sUUID){
		case .UUID128,.UUID16:
			let cbuuid = CBUUID(string: sUUID)
			return CInt(BLEDevs?.StartScan(serviceUUIDs: [cbuuid]).rawValue ?? 0)
		default:
			break
	}
	return CInt(BLEDevs?.StartScan(serviceUUIDs: nil).rawValue ?? 0)
}

@_cdecl("Swift_BLE_StopScan")
func BLE_StopScan() {
	BLE_Init()
	BLEDevs?.StopScan()
}

@_cdecl("Swift_BLE_GetDevicesDescription")
func BLE_GetDevicesDescription() -> UnsafePointer<CChar> {
	BLE_Init()
	return (BLEDevs!.devicesDescription as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetDevicesDetailDescription")
func BLE_GetDevicesDetailDescription() -> UnsafePointer<CChar> {
	BLE_Init()
	return (BLEDevs!.devicesDetailDescription as NSString).utf8String!
}

@_cdecl("Swift_BLE_IsUpdate")
func BLE_IsUpdate() -> CBool {
	BLE_Init()
	return BLEDevs!.IsUpdate()
}

@_cdecl("Swift_BLE_ConnectByIndex")
func BLE_ConnectByIndex(_ index: CInt) -> CInt {
	BLE_Init()
	
	if let device = BLEDevs?.Connect(deviceIndex: Int(index) - 1) {
		BLEDevs?.DiscoverServices(by: device, serviceUUIDs: nil)
		return CInt(device.connectState.rawValue)
	}
	return CInt(CONSTATE.fail.rawValue)
}

@_cdecl("Swift_BLE_ConnectByName")
func BLE_ConnectByName(_ name: UnsafePointer<CChar>!) -> CInt {
	BLE_Init()
	
	let input = String.init(cString: name)
	if let device = BLEDevs?.Connect(name: input) {
		BLEDevs?.DiscoverServices(by: device, serviceUUIDs: nil)
		return CInt(device.connectState.rawValue)
	}
	return CInt(CONSTATE.fail.rawValue)
}

@_cdecl("Swift_BLE_ConnectByIdentifier")
func BLE_ConnectByIdentifier(_ identifier: UnsafePointer<CChar>!) -> CInt {
	BLE_Init()
	
	let input = String.init(cString: identifier)
	if CheckInquireType(by: input) == .UUID128 {
		let uuid = NSUUID(uuidString: input)
		if let device = BLEDevs?.Connect(identifier: uuid) {
			BLEDevs?.DiscoverServices(by: device, serviceUUIDs: nil)
			return CInt(device.connectState.rawValue)
		}
	}
	return CInt(CONSTATE.fail.rawValue)
}

@_cdecl("Swift_BLE_ConnectByAny")
func BLE_ConnectByAny(_ any: UnsafePointer<CChar>!) -> CInt {
	BLE_Init()
	
	let input = String.init(cString: any)
	switch CheckInquireType(by: input){
		case .UUID128:
			let uuid = NSUUID(uuidString: input)
			if let device = BLEDevs?.Connect(identifier: uuid) {
				BLEDevs?.DiscoverServices(by: device, serviceUUIDs: nil)
				return CInt(device.connectState.rawValue)
			}
		case .name,.UUID16:
			if let device = BLEDevs?.Connect(name: input) {
				BLEDevs?.DiscoverServices(by: device, serviceUUIDs: nil)
				return CInt(device.connectState.rawValue)
			}
		default:
			let ind: CInt = CInt(input,radix:10) ?? -1
			return BLE_ConnectByIndex(ind)
	}
	return CInt(CONSTATE.fail.rawValue)
}

@_cdecl("Swift_BLE_GetDeviceDetailDescriptionByIndex")
func BLE_GetDeviceDetailDescriptionByIndex(_ index: CInt) -> UnsafePointer<CChar> {
	BLE_Init()
	
	return (BLEDevs!.DeviceDetailDescription(deviceIndex: Int(index) - 1) as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetDeviceDetailDescriptionByName")
func BLE_GetDeviceDetailDescriptionByName(_ name: UnsafePointer<CChar>!) -> UnsafePointer<CChar> {
	BLE_Init()
	
	let input = String.init(cString: name)
	return (BLEDevs!.DeviceDetailDescription(name: input) as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetDeviceDetailDescriptionByIdentifier")
func BLE_GetDeviceDetailDescriptionByIdentifier(_ identifier: UnsafePointer<CChar>!) -> UnsafePointer<CChar> {
	BLE_Init()
	
	let input = String.init(cString: identifier)
	if CheckInquireType(by: input) == .UUID128 {
		let uuid = NSUUID(uuidString: input)
		return (BLEDevs!.DeviceDetailDescription(identifier: uuid) as NSString).utf8String!
	}
	return (BLEDevs!.DeviceDetailDescription(name: input) as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetDeviceDetailDescriptionByAny")
func BLE_GetDeviceDetailDescriptionByAny(_ any: UnsafePointer<CChar>!) -> UnsafePointer<CChar> {
	BLE_Init()
	
	let input = String.init(cString: any)
	switch CheckInquireType(by: input){
		case .UUID128:
			let uuid = NSUUID(uuidString: input)
			return (BLEDevs!.DeviceDetailDescription(identifier: uuid) as NSString).utf8String!
		case .name,.UUID16:
			return (BLEDevs!.DeviceDetailDescription(name: input) as NSString).utf8String!
		default:
			let ind: CInt = CInt(input,radix:10) ?? -1
			return BLE_GetDeviceDetailDescriptionByIndex(ind)
	}
}

@_cdecl("Swift_BLE_DisconnectByIndex")
func BLE_DisconnectByIndex(_ index: CInt){
	BLE_Init()
	
	BLEDevs?.Disconnect(deviceIndex: Int(index) - 1)
}

@_cdecl("Swift_BLE_DisconnectByName")
func BLE_DisconnectByName(_ name: UnsafePointer<CChar>!) {
	BLE_Init()
	
	let input = String.init(cString: name)
	BLEDevs?.Disconnect(name: input)
}

@_cdecl("Swift_BLE_DisconnectByIdentifier")
func BLE_DisconnectByIdentifier(_ identifier: UnsafePointer<CChar>!) {
	BLE_Init()
	
	let input = String.init(cString: identifier)
	if CheckInquireType(by: input) == .UUID128 {
		let uuid = NSUUID(uuidString: input)
		BLEDevs?.Disconnect(identifier: uuid)
	}
}

@_cdecl("Swift_BLE_DisconnectByAny")
func BLE_DisconnectByAny(_ any: UnsafePointer<CChar>!) {
	BLE_Init()
	
	let input = String.init(cString: any)
	switch CheckInquireType(by: input){
		case .UUID128:
			let uuid = NSUUID(uuidString: input)
			BLEDevs?.Disconnect(identifier: uuid)
		case .name,.UUID16:
			BLEDevs?.Disconnect(name: input)
		default:
			let ind: CInt = CInt(input,radix:10) ?? -1
			BLE_DisconnectByIndex(ind)
	}
}

@_cdecl("Swift_BLE_GetPeripheralByIndex")
func BLE_GetPeripheralByIndex(_ index: CInt) -> UnsafePointer<CChar> {
	BLE_Init()
	
	return (BLEDevs!.GetDevice(by: Int(index) - 1)!.peripheral.identifier.uuidString as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetPeripheralByName")
func BLE_GetPeripheralByName(_ name: UnsafePointer<CChar>!) -> UnsafePointer<CChar> {
	BLE_Init()
	
	let input = String.init(cString: name)
	return (BLEDevs!.GetDevice(by: input)!.peripheral.identifier.uuidString as NSString).utf8String!
}

@_cdecl("Swift_BLE_GetPeripheralByAny")
func BLE_GetPeripheralByAny(_ any: UnsafePointer<CChar>!) -> UnsafePointer<CChar> {
	BLE_Init()
	
	let input = String.init(cString: any)
	switch CheckInquireType(by: input){
		case .UUID128:
			let uuid = NSUUID(uuidString: input)
			return (BLEDevs!.GetDevice(by: uuid)!.peripheral.identifier.uuidString as NSString).utf8String!
		case .name,.UUID16:
			return (BLEDevs!.GetDevice(by: input)!.peripheral.identifier.uuidString as NSString).utf8String!
		default:
			let ind: CInt = CInt(input,radix:10) ?? 0
			return (BLEDevs!.GetDevice(by: Int(ind - 1))!.peripheral.identifier.uuidString as NSString).utf8String!
	}
}

@_cdecl("Swift_BLE_GetServiceAndCharacteristic")
func BLE_GetServiceAndCharacteristic(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceIndex: CInt
	,_ characteristicIndex: CInt) -> UnsafePointer<CChar> {
	
	BLE_Init()
	
	let input = String.init(cString: identifier)
	let si = Int(serviceIndex) - 1
	let ci = Int(characteristicIndex) - 1
	var retStr: String = ""
	var ifok: Bool = false

	if CheckInquireType(by: input) == .UUID128 {
		let uuid = NSUUID(uuidString: input)
		if let device = BLEDevs!.GetDevice(by: uuid){
			if let services = device.peripheral.services{
				if (si >= 0 && si < services.count) {
					retStr = services[si].uuid.uuidString
					if let characteristics = services[si].characteristics{
						if (ci >= 0 && ci < characteristics.count) {
							retStr += "|"
							retStr += characteristics[ci].uuid.uuidString
							ifok = true
						}
					}
				}
			}
		}
	}
	if ifok == false{
		retStr = ""
	}
	return (retStr as NSString).utf8String!
}

@_cdecl("Swift_BLE_Refresh")
func BLE_Refresh() {
	BLE_Init()
	
	BLEDevs?.CleanDevices()
}

func BLE_CovertID(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!) ->(id: NSUUID?,sid: CBUUID?,cid: CBUUID?) {

	var id: NSUUID?
	var sid: CBUUID?
	var cid: CBUUID?
	
	var input = String.init(cString: identifier)
	if CheckInquireType(by: input) == .UUID128 {
		id = NSUUID(uuidString: input)
	}
	
	input = String.init(cString: serviceUUID)
	var ret = CheckInquireType(by: input)
	if ret == .UUID128 || ret == .UUID16 {
		sid = CBUUID(string: input)
	}
	
	input = String.init(cString: characteristicUUID)
	ret = CheckInquireType(by: input)
	if ret == .UUID128 || ret == .UUID16 {
		cid = CBUUID(string: input)
	}
	return (id,sid,cid)
}

@_cdecl("Swift_BLE_CheckID")
func BLE_CheckID(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!) ->CBool {
	
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid)
	return(characteristic != nil)
}

@_cdecl("Swift_BLE_GetMTU")
func BLE_GetMTU(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!) -> CInt{
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		return CInt(characteristic.service.peripheral.maximumWriteValueLength(for: .withResponse))
	}
	return 0
}

@_cdecl("Swift_BLE_GetMTU_NR")
func BLE_GetMTU_NR(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!) -> CInt{
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		return CInt(characteristic.service.peripheral.maximumWriteValueLength(for: .withoutResponse))
	}
	return 0
}

@_cdecl("Swift_BLE_Write")
func BLE_Write(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!
	, buffer: UnsafeMutablePointer<CChar>!,len: CInt) -> CInt{
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		let data = Data(bytesNoCopy: buffer,count: Int(len), deallocator: .none)
		if let ret = BLEDevs?.Write(to: characteristic, with: data){
			return (ret > 0 ? 1 : 0)
		}
	}
	return 0
}

@_cdecl("Swift_BLE_WriteNR")
func BLE_WriteNR(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!
	, buffer: UnsafeMutablePointer<CChar>!,len: CInt) -> CInt{
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		let data = Data(bytesNoCopy: buffer,count: Int(len), deallocator: .none)
		BLEDevs?.WriteNR(to: characteristic, with: data)
	}
	return 1
}

@_cdecl("Swift_BLE_SetNotify")
func BLE_SetNotify(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!
	,use rxFifoUID: UInt32) {
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		BLEDevs?.SetNotify(for: characteristic,use: rxFifoUID)
	}
}

@_cdecl("Swift_BLE_Read")
func BLE_Read(
	 _ identifier: UnsafePointer<CChar>!
	,_ serviceUUID: UnsafePointer<CChar>!
	,_ characteristicUUID: UnsafePointer<CChar>!
	, buffer: UnsafePointer<CChar>!,len: CInt) -> CInt{
	BLE_Init()
	
	let ids = BLE_CovertID(identifier,serviceUUID,characteristicUUID)
	if let characteristic = BLEDevs?.GetCharacteristic(identifier: ids.id, serviceUUID: ids.sid, characteristicUUID: ids.cid){
		//let data = Data(bytesNoCopy: buffer,count: Int(len), deallocator: .none)
		BLEDevs?.Read(for: characteristic)
		//BLEDevs?.Write(to: characteristic!, with: data);
	}
	return 0
}
