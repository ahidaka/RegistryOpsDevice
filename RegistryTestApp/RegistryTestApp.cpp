// RegistryTestApp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>

#include <windows.h>
#include <iostream>
#include <tchar.h>

#define IOCTL_REG_CREATE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_APPEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_READ   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_CLEAR  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

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
        _T("\\\\.\\RegistryOpsDevice"),
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
    SendIoControl(hDevice, IOCTL_REG_CREATE, NULL, 0, NULL, 0);

    // Append data to registry key
    std::cout << "Appending data to registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_APPEND, appendData, sizeof(appendData), NULL, 0);

    // Read data from registry key
    std::cout << "Reading data from registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_READ, NULL, 0, readBuffer, sizeof(readBuffer));
    std::wcout << L"Data read: " << readBuffer << std::endl;

    // Clear registry key
    std::cout << "Clearing registry key..." << std::endl;
    SendIoControl(hDevice, IOCTL_REG_CLEAR, NULL, 0, NULL, 0);

    CloseHandle(hDevice);
    return 0;
}
