#pragma once

// DeviceControlOps.h

#define TARGET_HARDWARE_ID L"USB\\VID_XXXX&PID_YYYY" // for Test or Dummy

#define IOCTL_REG_DISABLE  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) // D: Disable all HID devices
#define IOCTL_REG_ENABLE   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) // E: Enable all HID devices
#define IOCTL_REG_REGISTER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) // R: Enable registered HID devices
#define IOCTL_REG_CLEAR    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS) // C: Clear registered HID devices
#define IOCTL_REG_ADD      CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS) // A: Add registered HID device
#define IOCTL_REG_LIST     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS) // L: List registered HID devices