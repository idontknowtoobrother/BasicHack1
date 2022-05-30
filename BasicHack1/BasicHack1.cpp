
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

// Global Vars
HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName) {
	PROCESSENTRY32 procEntry32;

	// Defineing the size so we can populate it :D
	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	// Taking a snapshot of all process running ..
	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE) 
	{
		std::cout << "Failed to take snapshot of processes!" << std::endl;
		return false;
	}

	while (Process32Next(hProcSnap, &procEntry32)) 
	{
		std::cout << procEntry32.szExeFile << std::endl;

		if (!strcmp(procName, procEntry32.szExeFile)) 
		{
			std::cout << "Founded process" << procEntry32.szExeFile << " with process ID" << procEntry32.th32ProcessID << std::endl;
			pID = procEntry32.th32ProcessID;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

			if (hProc == NULL) {
				std::cout << "Failed getting handle to process." << std::endl;
			}

			CloseHandle(hProcSnap);
			return true;
		}

	}

	std::cout << "Couldn't find " << procName << " in the process snapshot" << std::endl;
	CloseHandle(hProcSnap);
	return false;


}


template <class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite) 
{
	WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addressToRead) 
{
	dataType rpmBuffer;
	ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);
	return rpmBuffer;
} 

int main()
{
	DWORD playerHP1 = 0x0073F0F8;
	DWORD playerHP2 = 0x0073F10C;
	attachProc((char*)"simpleGame.exe");

	while (true) {
		Sleep(0);
	}

}
