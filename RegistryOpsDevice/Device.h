#pragma once
#include "public.h"

EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
#define POT_SIZE (4096)

typedef enum _OP_MODE
{
	Disable = 0,
	Enable = 1,
	Register = 2,
	Clear = 3,
} MODE;

typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder

	//ULONG Mode; // 0: Disable, 1: Enable, 2: Register, 3: Clear
	MODE ModeNum;
	WCHAR Data[512];
	size_t DataLength;
	WCHAR Pot[POT_SIZE];
	size_t PotLength;
	const size_t MaxPotSize; // POT_SIZE

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
RegistryOpsDeviceCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

EXTERN_C_END
