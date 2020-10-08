//
//  BLEManager.swift
//  BTT
//
//  Created by Leif Wen on 2020/9/10.
//  Copyright © 2020 Leif Wen. All rights reserved.
//

import Foundation
import CoreBluetooth

class AdvertisementData {
	var localName: String?
	var txPowerLevel: NSNumber = 0
	var serviceUUIDs = [CBUUID]()
	var serviceData: String?
	var manufacturerData: NSData?
	var overflowServiceUUIDs = [CBUUID]()
	var isConnectable: Bool = false
	var solicitedServiceUUIDs = [CBUUID]()
	var dataChannel: NSNumber = 0
	var appleMfgData: NSDictionary?
	
	func update(_ advData: [String : Any]){
		localName				= advData[CBAdvertisementDataLocalNameKey]				as? String ?? localName
		txPowerLevel			= advData[CBAdvertisementDataTxPowerLevelKey]			as? NSNumber ?? txPowerLevel
		serviceUUIDs			= advData[CBAdvertisementDataServiceUUIDsKey] 			as? [CBUUID] ?? serviceUUIDs
		if case serviceData 	= advData[CBAdvertisementDataServiceDataKey]			as? String{}
		manufacturerData		= advData[CBAdvertisementDataManufacturerDataKey]		as? NSData ?? manufacturerData
		overflowServiceUUIDs	= advData[CBAdvertisementDataOverflowServiceUUIDsKey]	as? [CBUUID] ?? overflowServiceUUIDs
		isConnectable			= advData[CBAdvertisementDataIsConnectable]				as? Bool ?? isConnectable
		solicitedServiceUUIDs	= advData[CBAdvertisementDataSolicitedServiceUUIDsKey]	as? [CBUUID] ?? solicitedServiceUUIDs
		dataChannel				= advData["kCBAdvDataChannel"]							as? NSNumber ?? dataChannel
		appleMfgData			= advData["kCBAdvDataAppleMfgData"]						as? NSDictionary ?? appleMfgData
	}
}

enum CONSTATE: Int {
	case disconnect = 0
	case discovering = 1
	case discovered = 2
	case connecting = 3
	case connected = 4
	case fail = 5
}

class BLEDevice {
	var peripheral: CBPeripheral! = nil
	var connectState: CONSTATE = .disconnect
	var identifier: UUID { return peripheral.identifier }
	var servicesCount: Int = 0
	var characteristicsCount: Int = 0
	var rssi: NSNumber = 0
	var adv = AdvertisementData()
	var description: String { return "Name:\(adv.localName ?? "N/A"), ID:[\(peripheral.identifier.uuidString)], \(rssi)dBm, \(connectState)" }
	var servicesUUIDs: String { return "UUID:\(adv.serviceUUIDs)" }
	var rxFifoUIDs = [CBCharacteristic: UInt32]()
	
	func PropertiesString(_ properties: CBCharacteristicProperties)->(String){
		var propertiesReturn : String = ""

		if (properties.rawValue & CBCharacteristicProperties.broadcast.rawValue) != 0 {
			propertiesReturn += ",broadcast"
		}
		if (properties.rawValue & CBCharacteristicProperties.read.rawValue) != 0 {
			propertiesReturn += ",read"
		}
		if (properties.rawValue & CBCharacteristicProperties.writeWithoutResponse.rawValue) != 0 {
			propertiesReturn += ",write without response"
		}
		if (properties.rawValue & CBCharacteristicProperties.write.rawValue) != 0 {
			propertiesReturn += ",write"
		}
		if (properties.rawValue & CBCharacteristicProperties.notify.rawValue) != 0 {
			propertiesReturn += ",notify"
		}
		if (properties.rawValue & CBCharacteristicProperties.indicate.rawValue) != 0 {
			propertiesReturn += ",indicate"
		}
		if (properties.rawValue & CBCharacteristicProperties.authenticatedSignedWrites.rawValue) != 0 {
			propertiesReturn += ",authenticated signed writes"
		}
		if (properties.rawValue & CBCharacteristicProperties.extendedProperties.rawValue) != 0 {
			propertiesReturn += ",indicate"
		}
		if (properties.rawValue & CBCharacteristicProperties.notifyEncryptionRequired.rawValue) != 0 {
			propertiesReturn += ",notify encryption required"
		}
		if (properties.rawValue & CBCharacteristicProperties.indicateEncryptionRequired.rawValue) != 0 {
			propertiesReturn += ",indicate encryption required"
		}
		return propertiesReturn
	}
	
	func ServicesDescription(by peripheral: CBPeripheral, head: String) -> String{
		var des: String = ""
		var sHead: String = head + "|"
		if let services = peripheral.services {
			for (index,service) in services.enumerated() {
				des += sHead
				des += "--\(index + 1).Service:              [\(service.uuid.uuidString)]\r\n"
				if service == services.last{
					sHead = head + " "
				}
				des += CharacteristicsDescription(by: service,head: sHead)
            }
        }
		return des
	}
	
	func CharacteristicsDescription(by service: CBService, head: String) -> String{
		var des: String = ""
		var sHead: String = head + "    |"
		if let characteristics = service.characteristics {
			for (index,characteristic) in characteristics.enumerated() {
				des += sHead
				des += "--\(index + 1).Characteristic:  [\(characteristic.uuid.uuidString)]"
				des += " \(String(describing: PropertiesString(characteristic.properties)))\r\n"
				if characteristic == characteristics.last{
					sHead = head + "     "
				}
				
				des += DescriptorsDescription(by: characteristic,head: sHead)
			}
        }
		return des
	}
	
	func DescriptorsDescription(by characteristic: CBCharacteristic, head: String) -> String{
		var des: String = ""
		let sHead: String = head + "    |"
		if let descriptors = characteristic.descriptors {
			for (index,descriptor) in descriptors.enumerated() {
				des += sHead + "--\(index + 1).Descriptor: [\(descriptor.uuid.uuidString)]\r\n"
			}
        }
		return des
	}

	func Update(_ peripheral: CBPeripheral!, adv advData: [String : Any], rssi RSSI: NSNumber){
		self.peripheral = peripheral
		adv.update(advData)
		rssi = RSSI
	}
}


class BLEDevicesManager{
	private var centralManager: CBCentralManager!
	private var coreDelegate: BLECoreDelegate!
	var state: CBManagerState{ return centralManager.state }
	
	private var devices = [BLEDevice]()
	private var simpleDescription: String = ""
	private var update: Bool = false{
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
	
	init() {
		coreDelegate = BLECoreDelegate(bleDevManager: self)
		centralManager = CBCentralManager(delegate: coreDelegate, queue: nil)
    }
	
	func IsUpdate() -> Bool {
		return update
	}

	func Reset(){
		devices.removeAll()
	}
	
	func Insert(_ peripheral: CBPeripheral, advData: [String : Any], rssi: NSNumber) {
		var device = GetDevice(by: peripheral)
		if device == nil {
			peripheral.delegate = coreDelegate
			devices.append(BLEDevice())
			device = devices.last
		}
		device!.Update(peripheral,adv: advData,rssi: rssi)
		update = true;
	}
			
	func GetDevice(by index: Int) -> BLEDevice? {
		return (index >= 0 && index < devices.count) ? devices[index] : nil
	}
	
	func GetDevice(by name: String?) -> BLEDevice? {
		if name != nil {
			for dev in devices {
				if dev.adv.localName == name {
					return dev
				}
			}
		}
		return nil
	}
	
	func GetDevice(by identifier: NSUUID?) -> BLEDevice? {
		if identifier != nil {
			for dev in devices {
				if identifier == NSUUID(uuidString: dev.identifier.uuidString) {
					return dev
				}
			}
		}
		return nil
	}

	func GetDevice(by peripheral: CBPeripheral?) -> BLEDevice? {
		if peripheral != nil {
			for dev in devices {
				if dev.peripheral == peripheral {
					return dev
				}
			}
		}
		return nil
	}

	func GetService(in device: BLEDevice?,by cbuuid: CBUUID?) -> CBService? {
		if cbuuid != nil {
			if let services = device?.peripheral.services {
				for service in services {
					if cbuuid == service.uuid {
						return service
					}
				}
			}
		}
		return nil
	}
	
	func GetMTU(device: BLEDevice?) -> Int {
		return device?.peripheral.maximumWriteValueLength(for: .withoutResponse) ?? 0
	}
	
	func GetCharacteristic(in service: CBService?, by cbuuid: CBUUID?) -> CBCharacteristic? {
		if cbuuid != nil {
			if let characteristics = service?.characteristics {
				for characteristic in characteristics {
					if cbuuid == characteristic.uuid {
						return characteristic
					}
				}
			}
		}
		return nil
	}
	
	func GetCharacteristic(identifier: NSUUID?,serviceUUID: CBUUID?, characteristicUUID: CBUUID?) -> CBCharacteristic? {
		let	device = BLEDevs?.GetDevice(by: identifier)
		let	service = BLEDevs?.GetService(in: device, by: serviceUUID)
		let characteristic = BLEDevs?.GetCharacteristic(in: service, by: characteristicUUID)
		return characteristic
	}
	
	func CleanDevices() {
		var removed: Bool = true
		while(removed) {
			removed = false
			for (index,dev) in devices.enumerated() {
				if dev.connectState == .disconnect || dev.connectState == .fail{
					devices.remove(at: index)
					removed = true
					break
				}
				else if dev.peripheral.state == .disconnected || dev.peripheral.state == .disconnecting{
					dev.connectState = .disconnect
					devices.remove(at: index)
					removed = true
					break
				}
			}
		}
	}

	func StartScan(serviceUUIDs: [CBUUID]?) -> CBManagerState {
		if state == CBManagerState.poweredOn && centralManager.isScanning == false {
			centralManager.scanForPeripherals(withServices: serviceUUIDs, options: nil)
		}
		return state
	}

	func Connect(device: BLEDevice) -> BLEDevice {
		if state == CBManagerState.poweredOn {
			if device.connectState == .fail || device.connectState == .disconnect {
				device.connectState = .discovering
				device.servicesCount = 0
				device.characteristicsCount = 0
				centralManager.connect(device.peripheral, options: nil)
			}
		}
		return device
	}
	
	func Connect(deviceIndex: Int) -> BLEDevice? {
		if let dev = GetDevice(by: deviceIndex){
			return(Connect(device: dev))
		}
		return nil
	}
	
	func Connect(name: String?) -> BLEDevice? {
		if let dev = GetDevice(by: name){
			return(Connect(device: dev))
		}
		return nil
	}

	func Connect(identifier: NSUUID?) -> BLEDevice? {
		if let dev = GetDevice(by: identifier){
			return(Connect(device: dev))
		}
		return nil
	}

	func DiscoverServices(by device: BLEDevice?, serviceUUIDs: [CBUUID]?) {
		if state == CBManagerState.poweredOn {
			if device?.connectState == .discovered{
				device?.connectState = .connecting
				device?.peripheral.discoverServices(serviceUUIDs)
			}
		}
	}

	func Disconnect(device: BLEDevice) {
		if state == CBManagerState.poweredOn {
			centralManager.cancelPeripheralConnection(device.peripheral)
			device.connectState = .disconnect
		}
	}

	func Disconnect(deviceIndex: Int) {
		if let dev = GetDevice(by: deviceIndex){
			return(Disconnect(device: dev))
		}
	}
	
	func Disconnect(name: String?) {
		if let dev = GetDevice(by: name){
			return(Disconnect(device: dev))
		}
	}

	func Disconnect(identifier: NSUUID?) {
		if let dev = GetDevice(by: identifier){
			return(Disconnect(device: dev))
		}
	}
	
	func Write(to characteristic: CBCharacteristic,with data: Data) {
		if state == CBManagerState.poweredOn {
			characteristic.service.peripheral.writeValue(data, for: characteristic, type: .withoutResponse)
		}
	}
	
	func Read(for characteristic: CBCharacteristic) {
		if state == CBManagerState.poweredOn {
			characteristic.service.peripheral.readValue(for: characteristic)
		}
	}
	
	func SetNotify(for characteristic: CBCharacteristic,use rxFifoUID: UInt32) {
		if state == CBManagerState.poweredOn {
			let device = GetDevice(by:characteristic.service.peripheral)
			device?.rxFifoUIDs.updateValue(rxFifoUID, forKey: characteristic)
			characteristic.service.peripheral.setNotifyValue(true, for: characteristic)
		}
	}

	func StopScan() {
		if state == CBManagerState.poweredOn {
			centralManager.stopScan()
		}
	}

	var devicesDescription: String {
		var des: String = ""
		for (index,device) in devices.enumerated() {
			des += "< \(index + 1) > \(device.description)\r\n"
			des += "      \(device.servicesUUIDs)\r\n"
		}
		update = false
		return des
	}
	
	var devicesDetailDescription: String {
		var des: String = ""
		for (index,device) in devices.enumerated() {
			des += "< \(index + 1) > \(device.description)\r\n"
			des += "      \(device.servicesUUIDs)\r\n"
			des += "\(device.ServicesDescription(by: device.peripheral,head: "        "))"
		}
		return des
	}
	
	func DeviceDetailDescription(device: BLEDevice) -> String {
		var des: String = ""
		des = "MTU:\(device.peripheral.maximumWriteValueLength(for: .withoutResponse))\r\n"
		des += "\(device.description)\r\n"
		des += "\(device.servicesUUIDs)\r\n"
		des += "\(device.ServicesDescription(by: device.peripheral,head: "  "))"
		return des
	}

	func DeviceDetailDescription(deviceIndex: Int) -> String {
		if let dev = GetDevice(by: deviceIndex){
			return(DeviceDetailDescription(device: dev))
		}
		return ""
	}
	
	func DeviceDetailDescription(name: String?) -> String {
		if let dev = GetDevice(by: name){
			return(DeviceDetailDescription(device: dev))
		}
		return ""
	}

	func DeviceDetailDescription(identifier: NSUUID?) -> String {
		if let dev = GetDevice(by: identifier){
			return(DeviceDetailDescription(device: dev))
		}
		return ""
	}
}


class BLECoreDelegate: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
	var bleDevManager: BLEDevicesManager!
	
	init(bleDevManager: BLEDevicesManager) {
        super.init()
		self.bleDevManager = bleDevManager
    }

	func centralManagerDidUpdateState(_ central: CBCentralManager) {
		//callback func by corebluetooth frame after centralManager init
	}

	func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
		//callback func by corebluetooth frame after centralManager scan
		bleDevManager.Insert(peripheral, advData: advertisementData, rssi: RSSI)
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
		//callback func by corebluetooth frame after centralManager connect peripheral
		bleDevManager.GetDevice(by: peripheral)?.connectState = .discovered
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
		bleDevManager.GetDevice(by: peripheral)?.connectState = .fail
    }
	
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral) {
		//callback func by corebluetooth frame after centralManager disconnect peripheral
		bleDevManager.GetDevice(by: peripheral)?.connectState = .disconnect
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
		//callback func by corebluetooth frame after call peripheral.discoverServices
        if let services = peripheral.services {
			bleDevManager.GetDevice(by: peripheral)?.servicesCount = services.count
			for service in services {
                peripheral.discoverCharacteristics(nil, for: service)
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
		//callback func by corebluetooth frame after call peripheral.discoverCharacteristics
		bleDevManager.GetDevice(by: peripheral)?.servicesCount -= 1
		if let characteristics = service.characteristics {
			bleDevManager.GetDevice(by: peripheral)?.characteristicsCount += characteristics.count
            for characteristic in characteristics {
				peripheral.discoverDescriptors(for: characteristic)
			}
        }
    }

	func peripheral(_ peripheral: CBPeripheral, didDiscoverDescriptorsFor characteristic: CBCharacteristic, error: Error?){
		//callback func by corebluetooth frame after call peripheral.discoverDescriptors
		bleDevManager.GetDevice(by: peripheral)?.characteristicsCount -= 1
		if bleDevManager.GetDevice(by: peripheral)?.characteristicsCount == 0
			&& bleDevManager.GetDevice(by: peripheral)?.servicesCount == 0{
			bleDevManager.GetDevice(by: peripheral)?.connectState = .connected
		}
	}
	
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
		if let cdata = characteristic.value{
			let device = bleDevManager.GetDevice(by: peripheral)
			if let uid = device?.rxFifoUIDs[characteristic] {
				_ = cdata.withUnsafeBytes({ (ptr : UnsafeRawBufferPointer) in BLE_ReadCallBack(uid,ptr.baseAddress,Int32(cdata.count))})
			}
		}
    }
	
	func peripheral(_ peripheral: CBPeripheral, didUpdateNotificationStateFor characteristic: CBCharacteristic, error: Error?) {
		//characteristic.service.peripheral.readValue(for: characteristic)
    }
}
