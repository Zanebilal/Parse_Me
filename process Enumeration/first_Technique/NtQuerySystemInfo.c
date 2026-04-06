#include <Windows.h>
#include <stdio.h>

#include "Struct.h"

#define TARGET_PROCESS L"Greenshot.exe"

typedef NTSTATUS(NTAPI* fnNtQuerySystemInformation)(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID                    SystemInformation,
	ULONG                    SystemInformationLength,
	PULONG                   ReturnLength
	);

BOOL GetRemoteProcessHandle(LPCWSTR szProcName, DWORD* pdwPid, HANDLE* phProcess) {

	fnNtQuerySystemInformation pNtQuerySystemInformation = NULL;
	PSYSTEM_PROCESS_INFORMATION SystemProcInfo = NULL;
	PVOID pValueToFree = NULL;
	ULONG pReturnlength1 = NULL;
	ULONG pReturnLength2 = NULL;
	NTSTATUS STATUS = NULL;


	pNtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(GetModuleHandle(L"NTDLL.DLL"), "NtQuerySystemInformation");
	if (pNtQuerySystemInformation == NULL) {
		printf("[!] GetProcAddress Failed With Error : %d\n", GetLastError());
		return FALSE;
	}

	pNtQuerySystemInformation(SystemProcessInformation, NULL, NULL, &pReturnlength1);

	SystemProcInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T)pReturnlength1);
	if (SystemProcInfo == NULL) {
		printf("[!] HeapAlloc Failed With Error : %d\n", GetLastError());
		return FALSE;
	}

	pValueToFree = SystemProcInfo;

	STATUS = pNtQuerySystemInformation(SystemProcessInformation, SystemProcInfo, pReturnlength1, &pReturnLength2);
	if (STATUS != 0x0) {
		printf("[!] NtQuerySystemInformation Failed With Error : 0x%0.8X \n", STATUS);
		return FALSE;
	}

	while (TRUE) {

		if (SystemProcInfo->ImageName.Length && wcscmp(SystemProcInfo->ImageName.Buffer, szProcName) == 0) {
			*pdwPid = (DWORD)SystemProcInfo->UniqueProcessId;
			*phProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)SystemProcInfo->UniqueProcessId);
			break;
		}

		if (!SystemProcInfo->NextEntryOffset) {
			break;
		}

		SystemProcInfo = (PSYSTEM_PROCESS_INFORMATION)((ULONG_PTR)SystemProcInfo + SystemProcInfo->NextEntryOffset);
	}

	HeapFree(GetProcessHeap(), 0, pValueToFree);

	if (*pdwPid == NULL || *phProcess == NULL) {
		return FALSE;
	}
	else
		return TRUE;
}


int main() {

	DWORD		Pid = NULL;
	HANDLE		hProcess = NULL;

	if (!GetRemoteProcessHandle(TARGET_PROCESS, &Pid, &hProcess)) {
		wprintf(L"[!] Cound Not Get %s's Process Id \n", TARGET_PROCESS);
		return -1;
	}

	wprintf(L"[+] FOUND \"%s\" - Of Pid : %d \n", TARGET_PROCESS, Pid);

	CloseHandle(hProcess);

	printf("[#] Press <Enter> To Quit ... ");
	getchar();

	return 0;
}


