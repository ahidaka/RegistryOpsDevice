#include <ntddk.h>
#include <wdf.h>
#include <wdm.h>
#include "driver.h"
#include "queue.h"
#include "DeviceControlOps.h"
#include "trace.h"
#include "ComplicatedPot.tmh"

#define REG_PATH L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Services\\RegOpDev\\Parameters"
#define REG_VALUE_NAME L"ComplicatedPot"

NTSTATUS RegCreateComplicatedPot() {
    HANDLE regHandle;
    UNICODE_STRING regPath, valueName;
    OBJECT_ATTRIBUTES attributes;
    NTSTATUS status;
    ULONG disposition;

    RtlInitUnicodeString(&regPath, REG_PATH);
    InitializeObjectAttributes(&attributes, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateKey(&regHandle, KEY_ALL_ACCESS, &attributes, 0, NULL, REG_OPTION_NON_VOLATILE, &disposition);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "ZwCreateKey failed: %!STATUS!", status);
        return status;
    }

    RtlInitUnicodeString(&valueName, REG_VALUE_NAME);
    WCHAR initialValue[2] = { L'\0', L'\0' };
    status = ZwSetValueKey(regHandle, &valueName, 0, REG_MULTI_SZ, initialValue, sizeof(initialValue));
    ZwClose(regHandle);

    return status;
}

NTSTATUS RegAppendToComplicatedPot(WCHAR* data, size_t length) {
    HANDLE regHandle;
    UNICODE_STRING regPath, valueName;
    OBJECT_ATTRIBUTES attributes;
    NTSTATUS status;
    PKEY_VALUE_PARTIAL_INFORMATION buffer = NULL;
    ULONG bufferSize = 0;

    RtlInitUnicodeString(&regPath, REG_PATH);
    InitializeObjectAttributes(&attributes, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwOpenKey(&regHandle, KEY_ALL_ACCESS, &attributes);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_QUEUE, "ZwOpenKey failed: %!STATUS!", status);
        return status;
    }

    RtlInitUnicodeString(&valueName, REG_VALUE_NAME);
    status = ZwQueryValueKey(regHandle, &valueName, KeyValuePartialInformation, NULL, 0, &bufferSize);
    if (status != STATUS_BUFFER_TOO_SMALL) {
        ZwClose(regHandle);
        return status;
    }

    buffer = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool2(PagedPool, bufferSize, 'tag1');
    if (!buffer) {
        ZwClose(regHandle);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ZwQueryValueKey(regHandle, &valueName, KeyValuePartialInformation, buffer, bufferSize, &bufferSize);
    if (NT_SUCCESS(status)) {
        size_t existingLength = buffer->DataLength / sizeof(WCHAR);
        size_t newLength = existingLength + length + 1; // +1 for NULL terminator
        WCHAR* newValue = (WCHAR*)ExAllocatePool2(PagedPool, newLength * sizeof(WCHAR), 'tag2');

        if (newValue) {
            RtlCopyMemory(newValue, buffer->Data, buffer->DataLength);
            RtlCopyMemory(newValue + existingLength - 1, data, length * sizeof(WCHAR));
            newValue[newLength - 1] = L'\0';

            status = ZwSetValueKey(regHandle, &valueName, 0, REG_MULTI_SZ, newValue, (ULONG) newLength * sizeof(WCHAR));
            ExFreePoolWithTag(newValue, 'tag2');
        }
        else {
            status = STATUS_INSUFFICIENT_RESOURCES;
        }
    }

    ExFreePoolWithTag(buffer, 'tag1');
    ZwClose(regHandle);
    return status;
}

NTSTATUS RegReadComplicatedPot(WCHAR* outBuffer, size_t bufferSize) {
    HANDLE regHandle;
    UNICODE_STRING regPath, valueName;
    OBJECT_ATTRIBUTES attributes;
    NTSTATUS status;
    PKEY_VALUE_PARTIAL_INFORMATION buffer = NULL;
    ULONG requiredSize = 0;

    RtlInitUnicodeString(&regPath, REG_PATH);
    InitializeObjectAttributes(&attributes, &regPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwOpenKey(&regHandle, KEY_READ, &attributes);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    RtlInitUnicodeString(&valueName, REG_VALUE_NAME);
    status = ZwQueryValueKey(regHandle, &valueName, KeyValuePartialInformation, NULL, 0, &requiredSize);
    if (status != STATUS_BUFFER_TOO_SMALL) {
        ZwClose(regHandle);
        return status;
    }

    buffer = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool2(PagedPool, requiredSize, 'tag3');
    if (!buffer) {
        ZwClose(regHandle);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ZwQueryValueKey(regHandle, &valueName, KeyValuePartialInformation, buffer, requiredSize, &requiredSize);
    if (NT_SUCCESS(status) && buffer->Type == REG_MULTI_SZ) {
        RtlCopyMemory(outBuffer, buffer->Data, min(buffer->DataLength, bufferSize));
    }
    else {
        status = STATUS_INVALID_PARAMETER;
    }

    ExFreePoolWithTag(buffer, 'tag3');
    ZwClose(regHandle);
    return status;
}

NTSTATUS RegClearComplicatedPot() {
    return RegCreateComplicatedPot();
}
