#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>

constexpr auto PID_LIST_SIZE = 1024;		// Global for the size of the list

INT main()
{
	// Setup Process Variables
	DWORD dwProcList[PID_LIST_SIZE] = { 0 };
	DWORD dwRealSize = 0;
	BOOL bResult = FALSE; // Return type of BOOL
	DWORD32 dwCount = 0;

	// Enumerate the running processes
	bResult = EnumProcesses(dwProcList, sizeof(dwProcList), &dwRealSize);

	// Make sure bResult is not FALSE
	if (!bResult)
	{
		fprintf(stderr, "[!] Error: EnumProcesses: %ld", GetLastError());
		return FALSE;
	}

	// Get the number of running processes
	dwCount = dwRealSize / sizeof(DWORD);

	// Iterate over our process list
	for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
	{
		// Open a HANDLE to the current process
		HANDLE hProcess = OpenProcess(
			PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ,
			FALSE,
			dwProcList[dwIndex]
		);

		// Make sure the handle is there.
		if (hProcess)
		{
			wchar_t szProcessPath[MAX_PATH];
			DWORD dwProcNameLen = GetModuleFileNameExW(
				hProcess,
				NULL,
				szProcessPath,
				MAX_PATH
			);
			if (dwProcNameLen > 0)
			{
				wprintf(L"[+] PID: %d - %ws\n", dwProcList[dwIndex], szProcessPath);
			}
			else
			{
				fprintf(stderr, "[!] Error: GetModuleBaseNameW: %ld\n", GetLastError());
				return EXIT_FAILURE;
			}
			CloseHandle(hProcess);
		}
		else
		{
			fprintf(stderr, "[!] Error: OpenProcess() PID: %d -- Error Code: %ld\n",dwProcList[dwIndex], GetLastError());
		}
	}

	fprintf(stdout, "[+] Done!\n");
	return ERROR_SUCCESS;
}