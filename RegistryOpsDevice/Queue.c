#include "driver.h"
#include "queue.h"
#include "DeviceControlOps.h"
#include "trace.h"
#include "queue.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, RegistryOpsDeviceQueueInitialize)
#endif

NTSTATUS
RegistryOpsDeviceQueueInitialize(
    _In_ WDFDEVICE Device
    )
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    PAGED_CODE();

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );

    queueConfig.EvtIoDeviceControl = RegistryOpsDeviceEvtIoDeviceControl;
    queueConfig.EvtIoStop = RegistryOpsDeviceEvtIoStop;

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if(!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    return status;
}

VOID
RegistryOpsDeviceEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    WDFDEVICE device;
    PDEVICE_CONTEXT context;
    NTSTATUS status = STATUS_SUCCESS;
    size_t bufferLength = 0;
    WCHAR* inputBuffer = NULL;
    WCHAR* outputBuffer = NULL;

    UNREFERENCED_PARAMETER(Queue);
    TraceEvents(TRACE_LEVEL_INFORMATION,
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d", 
                Queue, Request, (int) OutputBufferLength, (int) InputBufferLength, IoControlCode);

    device = WdfIoQueueGetDevice(Queue);
    context = DeviceGetContext(device);

    switch (IoControlCode) {
    case IOCTL_REG_DISABLE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_DISABLE received");

        context->ModeNum = 0; // Disable;
        //
        status = RegCreateComplicatedPot();
        if (!NT_SUCCESS(status)) {
            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "RegCreateComplicatedPot failed %!STATUS!", status);
        }
        //
        break;

    case IOCTL_REG_ENABLE:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_ENABLE received");
        context->ModeNum = 1; // Enable;
        break;

    case IOCTL_REG_REGISTER:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_REGISTER received");
        context->ModeNum = 2; // Register;
        break;

    case IOCTL_REG_CLEAR:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_CLEARE received");
        status = RegClearComplicatedPot();
        break;

    case IOCTL_REG_ADD:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_APPEND received");

        // 入力バッファを取得
        status = WdfRequestRetrieveInputBuffer(Request, InputBufferLength, (PVOID*)&inputBuffer, &bufferLength);
        if (NT_SUCCESS(status) && bufferLength >= sizeof(WCHAR)) {
            size_t stringLength = bufferLength / sizeof(WCHAR);
            status = RegAppendToComplicatedPot(inputBuffer, stringLength);
        }
        else {
            status = STATUS_INVALID_PARAMETER;
        }
        break;

    case IOCTL_REG_LIST:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "IOCTL_REG_READ received");

        // 出力バッファを取得
        status = WdfRequestRetrieveOutputBuffer(Request, OutputBufferLength, (PVOID*)&outputBuffer, &bufferLength);
        if (NT_SUCCESS(status) && bufferLength >= sizeof(WCHAR)) {
            status = RegReadComplicatedPot(outputBuffer, bufferLength);
            if (NT_SUCCESS(status)) {
                WdfRequestSetInformation(Request, bufferLength);
            }
        }
        else {
            status = STATUS_INVALID_PARAMETER;
        }
        break;

    default:
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "Invalid IoControlCode: 0x%X", IoControlCode);
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    WdfRequestComplete(Request, status);

    return;
}

VOID
RegistryOpsDeviceEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
{
    TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d", 
                Queue, Request, ActionFlags);
    return;
}
