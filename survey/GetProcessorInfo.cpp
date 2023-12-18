// GetProcessorInfo.cpp

#include <Windows.h>
#include <stdio.h>

int main()
{
    // Bring in the SYSTEM_INFO struct
	SYSTEM_INFO sysInfo = { 0 };

	// Populate the sysInfo struct
	GetSystemInfo(&sysInfo);

	// Assign the SYSTEM_INFO struct members to our variables
	DWORD dwNumOfProcessors = sysInfo.dwNumberOfProcessors;
	DWORD dwProcessorType = sysInfo.dwProcessorType;

	// Get the Processor Arch
	if (sysInfo.dwProcessorType != NULL)
	{
		fprintf(stdout, "[+] System Arch: %d\n", sysInfo.dwProcessorType);
	} 
	else
	{ 
		fprintf(stderr, "[!] Could not get system arch... Error: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}

	// Get the Number of Processors
	if (dwNumOfProcessors != NULL)
	{
		fprintf(stdout, "[+] Number of Processors: %d\n", dwNumOfProcessors);
	}
	else
	{
		fprintf(stderr, "[!] Could not enumerate number of system processors: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}