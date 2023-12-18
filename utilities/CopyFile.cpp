#include <Windows.h>
#include <stdio.h>

#define REQUIRED_ARGC 3UL

int main(INT argc, PCHAR argv[])
{
	// Check for required arguments
	if (REQUIRED_ARGC != argc)
	{
		fprintf(stderr, "[!] Usage: CopyFile.exe C:\\PathToSource\\File C:\\PathToDestination\\File\n");
		return EXIT_FAILURE;
	}
	else
	{
		// Setup variables for our files
		LPCSTR SourceFilePath = argv[1];
		LPCSTR DupeFilePath = argv[2];
		HANDLE hFile = CreateFileA(
			SourceFilePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		// Make sure the source file is there.
		if (hFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Reading Source File - CreateFileA: %ld\n", GetLastError());
			return EXIT_FAILURE;
		}

		// Create our destination file.
		// Note: if the destination file already exists this will overwrite it.
		HANDLE hDestFile = CreateFileA(DupeFilePath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		// Make sure the handle to the destination file is there.
		if (hDestFile == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Error Destination CreateFileA: %ld\n", GetLastError());
			CloseHandle(hFile);
			return EXIT_FAILURE;
		}

		// Setup variables for copying the contents from our
		// source file to our destination file.
		DWORD bytesIn;
		DWORD bytesOut;
		LPCSTR buf[4096];

		// Read source file and write to our desitnation file.
		while (ReadFile(hFile, buf, sizeof(buf), &bytesIn, NULL) && bytesIn > 0)
		{
			if (!WriteFile(hDestFile, buf, bytesIn, &bytesOut, NULL) && bytesIn > 0)
			{
				fprintf(stderr, "Error: WriteFile: %lu\n", GetLastError());
				return EXIT_FAILURE;
			}
			break;
		}

		// Cleanup
		CloseHandle(hFile);
		CloseHandle(hDestFile);
	}
}