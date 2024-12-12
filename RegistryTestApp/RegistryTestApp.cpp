// RegistryTestApp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#include <windows.h>
#include <iostream>
#include <tchar.h>

#include "DeviceControlOps.h"

//
void SendIoControl(HANDLE hDevice, DWORD controlCode, LPVOID inBuffer, DWORD inBufferSize, LPVOID outBuffer, DWORD outBufferSize) {
    DWORD bytesReturned = 0;
    if (DeviceIoControl(hDevice, controlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, &bytesReturned, NULL)) {
        std::cout << "Operation successful. Bytes returned: " << bytesReturned << std::endl;
    }
    else {
        std::cerr << "Operation failed. Error code: " << GetLastError() << std::endl;
    }
}

int main() {
    HANDLE hDevice = CreateFile(
        _T("\\\\.\\RegOpDev"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open device. Error: " << GetLastError() << std::endl;
        return 1;
    }

    WCHAR appendData[] = L"Test String\0";
    WCHAR readBuffer[512] = { 0 };

    // Create registry key
    std::cout << "Creating registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_DISABLE, NULL, 0, NULL, 0);

    // Append data to registry key
    std::cout << "Appending data to registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_ADD, appendData, sizeof(appendData), NULL, 0);

    // Read data from registry key
    std::cout << "Reading data from registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_LIST, NULL, 0, readBuffer, sizeof(readBuffer));
    std::wcout << L"Data read: " << readBuffer << std::endl;

    // Clear registry key
    std::cout << "Clearing registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_CLEAR, NULL, 0, NULL, 0);

    CloseHandle(hDevice);
    return 0;
}
