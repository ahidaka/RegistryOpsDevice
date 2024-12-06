/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_RegistryOpsDevice,
    0x9f3a8d0e,0x3399,0x4b04,0xab,0x1f,0x82,0xb9,0x4a,0xdc,0x3b,0xee);
// {9f3a8d0e-3399-4b04-ab1f-82b94adc3bee}
