#pragma once

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    ULONG PrivateDeviceData;  // just a placeholder

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

//WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
RegistryOpsDeviceQueueInitialize(
    _In_ WDFDEVICE Device
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL RegistryOpsDeviceEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP RegistryOpsDeviceEvtIoStop;


NTSTATUS RegCreateComplicatedPot();

NTSTATUS RegAppendToComplicatedPot(WCHAR* data, size_t length);

NTSTATUS RegReadComplicatedPot(WCHAR* outBuffer, size_t bufferSize);

NTSTATUS RegClearComplicatedPot();

EXTERN_C_END
