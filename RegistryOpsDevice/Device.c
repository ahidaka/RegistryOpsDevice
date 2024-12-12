#include "driver.h"
#include "trace.h"
#include "device.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, RegistryOpsDeviceCreateDevice)
#endif

NTSTATUS
RegistryOpsDeviceCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    // Register NTFileName, This must be done before WdfDeviceCreate.
    DECLARE_CONST_UNICODE_STRING(NTFileName, DEVICE_NAME);
    status = WdfDeviceInitAssignName(DeviceInit, &NTFileName);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! WdfDeviceInitAssignName failed %!STATUS!", status);
        return status;
    }

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! WdfDeviceCreate %!STATUS!", status);

    if (NT_SUCCESS(status)) {
        deviceContext = DeviceGetContext(device);

        deviceContext->PrivateDeviceData = 0;



        // Register dosDeviceName
        DECLARE_CONST_UNICODE_STRING(dosDeviceName, SYMBOLIC_LINK_NAME);

        status = WdfDeviceCreateSymbolicLink(device, &dosDeviceName);
        if (!NT_SUCCESS(status)) {
            TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! WdfDeviceCreateSymbolicLink failed %!STATUS!", status);
            return status;
        }

        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_RegistryOpsDevice,
            NULL // ReferenceString
            );

        if (NT_SUCCESS(status)) {
            status = RegistryOpsDeviceQueueInitialize(device);
            if (!NT_SUCCESS(status)) {
                TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! RegistryOpsDeviceQueueInitialize failed %!STATUS!", status);
                return status;
            }
        }
        else {
            TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "%!FUNC! WdfDeviceCreateDeviceInterface failed %!STATUS!", status);
        }
    }

    return status;
}
