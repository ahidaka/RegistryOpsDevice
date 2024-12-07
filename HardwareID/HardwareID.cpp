// HardwareID.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <windows.h>
#include <iostream>
#include <tchar.h>

#define IOCTL_REG_CREATE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_APPEND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_READ   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_CLEAR  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

BOOL CheckHardwareID(WCHAR* HwID)
{
	WCHAR hwid[512] = { 0 };
	DWORD bytesReturned = 0;
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
		return FALSE;
	}
	// Read data from registry key
	std::cout << "Reading data from registry key..." << std::endl;
	if (DeviceIoControl(hDevice, IOCTL_REG_READ, NULL, 0, hwid, sizeof(hwid), &bytesReturned, NULL)) {
		std::wcout << L"Data read: " << hwid << std::endl;
		if (wcscmp(hwid, HwID) == 0) {
			std::cout << "Hardware ID matched." << std::endl;
			CloseHandle(hDevice);
			return TRUE;
		}
		else {
			std::cout << "Hardware ID mismatched." << std::endl;
			CloseHandle(hDevice);
			return FALSE;
		}
	}
	else {
		std::cerr << "Operation failed. Error code: " << GetLastError() << std::endl;
		CloseHandle(hDevice);
		return FALSE;
	}
}

//
//int main()
//
int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: HardwareID.exe <Hardware ID>" << std::endl;
		return 1;
	}
	WCHAR HwID[512] = { 0 };
	//WCHAR HwID[] = L"Test String\0";

	mbstowcs_s(NULL, HwID, argv[1], strlen(argv[1]));
	if (CheckHardwareID(HwID)) {
		std::cout << "Hardware ID matched." << std::endl;
	}
	else {
		std::cout << "Hardware ID mismatched." << std::endl;
	}

	return 0;
}
