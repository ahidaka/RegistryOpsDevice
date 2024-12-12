// TestConApp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tchar.h>
#include "DeviceControlOps.h"

//
//
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

int main()
{
	WCHAR appendData[] = L"Test String\0";
	WCHAR readBuffer[512] = { 0 };
	char str[256] = { 0 };

	std::cout << "Test Con!\n";

	HANDLE hDevice = CreateFile(
		_T("\\\\.\\RegOpDev"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

    while (toupper(str[0]) != 'Q' && toupper(str[0]) != 'X') {
		printf("Input(D,E,R,C,A,L,Q,X): ");
		scanf_s("%s", str, (unsigned int) sizeof(str));

		switch (toupper(str[0]))
		{
		case 'D':
			std::cout << " Disable mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_DISABLE, NULL, 0, NULL, 0);
			break;

		case 'E':
			std::cout << " Enable mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_ENABLE, NULL, 0, NULL, 0);	
			break;

		case 'R':
			std::cout << " Register mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_REGISTER, NULL, 0, NULL, 0);
			break;

		case 'C':
			std::cout << " Clear mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_CLEAR, NULL, 0, NULL, 0);
			break;

		case 'A':
			std::cout << " Add mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_ADD, appendData, sizeof(appendData), NULL, 0);
			break;

		case 'L':
			std::cout << " List mode..." << std::endl;
			SendIoControl(hDevice, IOCTL_REG_LIST, NULL, 0, readBuffer, sizeof(readBuffer));
			break;

		case 'X':
		case 'Q':
			printf("Bye...\n");
			break;
			
		default:
			break;
		}
    }

    return 0;
}
