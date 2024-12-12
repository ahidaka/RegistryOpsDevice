#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

#define DEVICE_NAME L"\\Device\\RegOpDev"
#define SYMBOLIC_LINK_NAME L"\\DosDevices\\RegOpDev"

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD RegistryOpsDeviceEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP RegistryOpsDeviceEvtDriverContextCleanup;

EXTERN_C_END
