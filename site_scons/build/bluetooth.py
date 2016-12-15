

# os x:
# http://stackoverflow.com/questions/18121538/how-can-i-check-for-hardware-ble-support-on-os-x

# switch ([_manager state])
# {
#     case CBCentralManagerStateUnsupported:
#     state = @"This device does not support Bluetooth Low Energy.";
#         break;
#     case CBCentralManagerStateUnauthorized:
#     state = @"This app is not authorized to use Bluetooth Low Energy.";
#         break;
#     case CBCentralManagerStatePoweredOff:
#     state = @"Bluetooth on this device is currently powered off.";
#         break;
#     case CBCentralManagerStateResetting:
#     state = @"The BLE Manager is resetting; a state update is pending.";
#         break;
#     case CBCentralManagerStatePoweredOn:
#     state = @"Bluetooth LE is turned on and ready for communication.";
#         break;
#     case CBCentralManagerStateUnknown:
#     state = @"The state of the BLE Manager is unknown.";
#         break;
#     default:
#     state = @"The state of the BLE Manager is unknown.";

# }
