//
//  BTManager.swift
//  SDT
//
//  Created by Leif Wen on 2020/9/19.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

import Foundation
import IOBluetooth

class BluetoothDeviceInquiryDelegate : IOBluetoothDeviceInquiryDelegate {
	var btManager: BluetoothManager!
	
	init(BTManager: BluetoothManager) {
		btManager = BTManager
    }

	func deviceInquiryStarted(_ sender: IOBluetoothDeviceInquiry!) {
		btManager.bluetoothDeviceInquiryState = .started
	}

	func deviceInquiryDeviceFound(_ sender: IOBluetoothDeviceInquiry!, device: IOBluetoothDevice!) {
		btManager.bluetoothDeviceInquiryState = .found
		btManager.update = true;
	}

	func deviceInquiryComplete(_ sender: IOBluetoothDeviceInquiry!, error: IOReturn, aborted: Bool) {
		btManager.bluetoothDeviceInquiryState = .complete
	}
}

enum RFCOMMSTATE: Int {
	case unknown = 0
	case openComplete = 1
	case channelClosed = 2
	case controlSignalsChanged = 3
	case flowControlChanged = 4
	case writeComplete = 5
	case queueSpaceAvailable = 6
}

class BluetoothRFCOMMChannelDelegate : IOBluetoothRFCOMMChannelDelegate {
	var state: RFCOMMSTATE = .unknown
	
	func rfcommChannelData(_ rfcommChannel: IOBluetoothRFCOMMChannel!, data dataPointer: UnsafeMutableRawPointer!, length dataLength: Int){
		if let btm = BTManager {
			if let uid = btm.rxFifoUIDs[rfcommChannel] {
				let ptr = dataPointer.assumingMemoryBound(to: UInt8.self)
				BT_ReadCallBack(uid,ptr,Int32(dataLength))
			}
		}
	}

	func rfcommChannelOpenComplete(_ rfcommChannel: IOBluetoothRFCOMMChannel!, status error: IOReturn){
		state = .openComplete
	}

	func rfcommChannelClosed(_ rfcommChannel: IOBluetoothRFCOMMChannel!){
		state = .channelClosed
	}

	func rfcommChannelControlSignalsChanged(_ rfcommChannel: IOBluetoothRFCOMMChannel!){
		state = .controlSignalsChanged
	}

	func rfcommChannelFlowControlChanged(_ rfcommChannel: IOBluetoothRFCOMMChannel!){
		state = .flowControlChanged
	}

	func rfcommChannelWriteComplete(_ rfcommChannel: IOBluetoothRFCOMMChannel!, refcon: UnsafeMutableRawPointer!, status error: IOReturn){
		state = .writeComplete
	}

	func rfcommChannelQueueSpaceAvailable(_ rfcommChannel: IOBluetoothRFCOMMChannel!){
		state = .queueSpaceAvailable
	}
}

enum DISTATE: Int {
	case unknown = 0
	case started = 1
	case found = 2
	case complete = 3
}

enum BLMOP: Int {
	case none = 0
	case startScan = 1
	case stopScan = 2
	case connect = 3
	case disconnect = 4
	case openRFCOMMChannelSync = 5
	case closeRFCOMMChannelSync = 6
}

class BluetoothManager{
	private let oplock = NSLock()
	private var op: BLMOP = .none
	private var opStart: Bool = false
	private var opExit: Bool = false
	private var opResult: Bool = false
	private var opAddress: String = ""
	private var opChannelID: BluetoothRFCOMMChannelID = 0
	private var opRxFifoUID: UInt32 = 0
	
	private var bluetoothDeviceInquiryDelegate: BluetoothDeviceInquiryDelegate!
	private var bluetoothDeviceInquiry: IOBluetoothDeviceInquiry!
			var bluetoothDeviceInquiryState = DISTATE.unknown
	private var bluetoothRFCOMMChannelDelegate = BluetoothRFCOMMChannelDelegate()

	private var rfcomms = [IOBluetoothDevice: IOBluetoothRFCOMMChannel]()
			var rxFifoUIDs = [IOBluetoothRFCOMMChannel: UInt32]()
	
	private var simpleDescription: String = ""
	var update: Bool = false{
		didSet{
			if update == true{
				let des = devicesDescription
				if simpleDescription != des{
					simpleDescription = des
					update = true
				}
			}
		}
	}
	
	func IsUpdate() -> Bool {
		return update
	}
	
	init() {
		bluetoothDeviceInquiryDelegate = BluetoothDeviceInquiryDelegate(BTManager: self)
		bluetoothDeviceInquiry = IOBluetoothDeviceInquiry(delegate: bluetoothDeviceInquiryDelegate)
    }
	
	func StartScan() -> DISTATE {
		if (bluetoothDeviceInquiryState != .started) && (bluetoothDeviceInquiryState != .found) {
			bluetoothDeviceInquiryState = .unknown
			bluetoothDeviceInquiry.searchType = kIOBluetoothDeviceSearchLE.rawValue		//no any usefull
			bluetoothDeviceInquiry.updateNewDeviceNames = true
			bluetoothDeviceInquiry.start()
		}
		return bluetoothDeviceInquiryState
	}
	
	func StopScan() {
		bluetoothDeviceInquiry.stop()
		bluetoothDeviceInquiryState = .complete
	}
	
	func ScanState() -> DISTATE{
		return bluetoothDeviceInquiryState
	}

	var devicesDescription: String {
		var des: String = ""
		if let devices = bluetoothDeviceInquiry.foundDevices(){
			for (index,_device) in devices.enumerated() {
				if let device = _device as? IOBluetoothDevice {
					des += "< \(index + 1) > MAC addr:[\(device.addressString ?? "")] name: \(device.name ?? "")\r\n"
				}
			}
		}
		update = false
		return des
	}
	
	func GetDeviceByAddress(_ address: String?) -> IOBluetoothDevice? {
		if address != nil {
			if let devices = bluetoothDeviceInquiry.foundDevices(){
				for _device : Any in devices {
					if let device = _device as? IOBluetoothDevice {
						if device.addressString == address {
							return device
						}
					}
				}
			}
		}
		return nil
	}
	
	func GetDeviceByName(_ name: String?) -> IOBluetoothDevice? {
		if name != nil {
			if let devices = bluetoothDeviceInquiry.foundDevices(){
				for _device : Any in devices {
					if let device = _device as? IOBluetoothDevice {
						if device.name == name {
							return device
						}
					}
				}
			}
		}
		return nil
	}

	func Connect(by device: IOBluetoothDevice?) -> IOBluetoothDevice? {
		
		return device?.openConnection() == kIOReturnSuccess ? device : nil
	}
	
	func Disconnect(by device: IOBluetoothDevice?) {
		device?.closeConnection()
	}
	
	func OpenRFCOMMChannelSync(on device: IOBluetoothDevice?, withChannelID channelID: BluetoothRFCOMMChannelID,use rxFifoUID: UInt32) -> IOBluetoothRFCOMMChannel? {
		var rfcomm: IOBluetoothRFCOMMChannel?
		let ret = device?.openRFCOMMChannelSync(&rfcomm, withChannelID: channelID, delegate: bluetoothRFCOMMChannelDelegate)
		if ret == kIOReturnSuccess{
			rfcomms.updateValue(rfcomm!, forKey: device!)
			rxFifoUIDs.updateValue(rxFifoUID, forKey: rfcomm!)
		}
		return ret == kIOReturnSuccess ? rfcomm: nil
	}
	
	func CloseRFCOMMChannelSync(on rfcomm: IOBluetoothRFCOMMChannel?) {
		var _rfcomm:IOBluetoothRFCOMMChannel? = rfcomm
		if _rfcomm != nil {
			rfcomms.removeValue(forKey: _rfcomm!.getDevice())
			rxFifoUIDs.removeValue(forKey: _rfcomm!)
			rfcomm?.close()
			_rfcomm = nil
		}
	}
	
	func OpenRFCOMMChannelState() -> RFCOMMSTATE {
		return bluetoothRFCOMMChannelDelegate.state
	}

	func GetRFCOMMChannel(by device: IOBluetoothDevice?) -> IOBluetoothRFCOMMChannel? {
		var rfcomm: IOBluetoothRFCOMMChannel?
		if let _device = device{
			rfcomm = rfcomms[_device]
		}
		return rfcomm
	}

	func WriteRFCOMMChannelSync(on rfcomm: IOBluetoothRFCOMMChannel?,data : UnsafeMutableRawPointer!, length: UInt32) -> UInt32 {
		var count: UInt16 = 0
		var total: UInt32 = 0

		if let mtu = rfcomm?.getMTU() {
			count = length < UInt16.max ? UInt16(length) : UInt16.max
				
			if count > mtu{
				count = mtu
			}
				
			if rfcomm?.writeSync(data?.advanced(by: Int(total)), length: count) == kIOReturnSuccess{
				total += UInt32(count)
			}
		}
		return total
	}
	func GetMTU(on rfcomm: IOBluetoothRFCOMMChannel?) -> UInt16 {
		return rfcomm?.getMTU() ?? 0
	}
	func Exit() {
		opExit = true
	}
	func Run() {
		if (opStart == true){
			opResult = true
			switch op {
				case .startScan:
					_ = StartScan()
				case .stopScan:
					StopScan()
				case .connect:
					opResult = Connect(by: GetDeviceByAddress(opAddress)) == nil ? false : true
				case .disconnect:
					Disconnect(by: GetDeviceByAddress(opAddress))
				case .openRFCOMMChannelSync:
					opResult = OpenRFCOMMChannelSync(on: GetDeviceByAddress(opAddress),withChannelID: opChannelID,use: opRxFifoUID) == nil ? false : true
				case .closeRFCOMMChannelSync:
					CloseRFCOMMChannelSync(on: GetRFCOMMChannel(by: GetDeviceByAddress(opAddress)))
				case .none:
					fallthrough
				default:
					opResult = false
					opAddress = ""
					opChannelID = 0
					opRxFifoUID = 0
			}
			opStart = false
		}
	}
	
	func toDo(opCode: BLMOP,macAddr : String,withChannelID channelID: BluetoothRFCOMMChannelID,use rxFifoUID: UInt32) -> Bool {
		var ret: Bool = false
		if oplock.try(){
			op = opCode
			opAddress = macAddr
			opChannelID = channelID
			opRxFifoUID = rxFifoUID
			opStart = true
			while (opStart == true && opExit == false) {}
			ret = opResult
			oplock.unlock()
		}
		return ret
	}
}
