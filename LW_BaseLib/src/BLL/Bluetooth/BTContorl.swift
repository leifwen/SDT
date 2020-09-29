//
//  BTContorl.swift
//  SDT
//
//  Created by Leif Wen on 2020/9/21.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

import Foundation
import IOBluetooth

var btInit: Bool = false
var BTManager: BluetoothManager?

func BT_Init(){
	if btInit == false{
		BTManager = BluetoothManager()
		btInit = true;
	}
}

@_cdecl("Swift_BT_Scan")
func BT_Scan() -> CInt {
	BT_Init()

	return BTManager?.toDo(opCode: .startScan, macAddr: "", withChannelID: 0, use: 0) == false ? 0 : 1
}

@_cdecl("Swift_BT_StopScan")
func BT_StopScan() {
	BT_Init()

	_ = BTManager?.toDo(opCode: .stopScan, macAddr: "", withChannelID: 0, use: 0)
}

@_cdecl("Swift_BT_ScanState")
func BT_ScanState() -> CInt {
	BT_Init()
	
	return CInt(BTManager!.ScanState().rawValue)
}

@_cdecl("Swift_BT_Connect")
func BT_Connect(macAddr: UnsafePointer<CChar>!) -> CInt {
	BT_Init()
	
	let input = String.init(cString: macAddr)
	
	return BTManager?.toDo(opCode: .connect, macAddr: input, withChannelID: 0, use: 0) == false ? 0 : 1
}

@_cdecl("Swift_BT_Disconnect")
func BT_Disconnect(macAddr: UnsafePointer<CChar>!) {
	BT_Init()
	
	let input = String.init(cString: macAddr)

	_ = BTManager?.toDo(opCode: .disconnect, macAddr: input, withChannelID: 0, use: 0)
}

@_cdecl("Swift_BT_OpenRFCOMMChannelSync")
func BT_OpenRFCOMMChannelSync(macAddr: UnsafePointer<CChar>!,withChannelID channelID: UInt8, use rxFifoUID: UInt32) -> CInt {
	BT_Init()
	
	let input = String.init(cString: macAddr)
	
	return BTManager?.toDo(opCode: .openRFCOMMChannelSync, macAddr: input, withChannelID: channelID, use: rxFifoUID) == false ? 0 : 1
}

@_cdecl("Swift_BT_CloseRFCOMMChannelSync")
func BT_CloseRFCOMMChannelSync(macAddr: UnsafePointer<CChar>!) {
	BT_Init()
	
	let input = String.init(cString: macAddr)
	
	_ = BTManager?.toDo(opCode: .closeRFCOMMChannelSync, macAddr: input, withChannelID: 0, use: 0)
}

@_cdecl("Swift_BT_RFCOMMChannelState")
func BT_RFCOMMChannelState() -> CInt {
	BT_Init()
	
	return CInt(BTManager?.OpenRFCOMMChannelState().rawValue == nil ? 0 : 1)
}

@_cdecl("Swift_BT_WriteRFCOMMChannelSync")
func BT_WriteRFCOMMChannelSync(macAddr: UnsafePointer<CChar>!,data: UnsafePointer<CChar>!,len: UInt32) -> UInt32 {
	BT_Init()
	
	let input = String.init(cString: macAddr)
	let device = BTManager?.GetDeviceByAddress(input)
	let rfcomm = BTManager?.GetRFCOMMChannel(by: device)
	let buf = UnsafeMutableRawPointer(mutating: data)
	
	return BTManager?.WriteRFCOMMChannelSync(on: rfcomm,data: buf,length: len) ?? 0
}

@_cdecl("Swift_BT_GetDevicesDescription")
func BT_GetDevicesDescription() -> UnsafePointer<CChar> {
	BT_Init()
	
	return (BTManager!.devicesDescription as NSString).utf8String!
}

@_cdecl("Swift_BT_IsUpdate")
func BT_IsUpdate() -> CBool {
	BT_Init()
	
	return BTManager!.IsUpdate()
}
