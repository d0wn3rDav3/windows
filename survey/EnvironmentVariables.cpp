#include <Windows.h>
#include <stdio.h>

int main()
{
	// Grab the systems environment variables
    LPWSTR envStrings = GetEnvironmentStrings();

    // Make sure we're not dereferecing a NULL pointer
	if (envStrings == NULL)
	{
		fprintf(stderr, "Error: envStrings are NULL: %ld\n", GetLastError());
		return EXIT_FAILURE;
	}

	// Setup our Iterator
	LPWSTR curEnvStr = envStrings;
	fprintf(stdout, "[+] Grabbing System Environment Variables [+]\n");
	
    // Iterate over the environment variables and print to stdout
	while (*curEnvStr != L'\0')
	{
		fprintf(stdout, "\t[+] %ws\n", curEnvStr);
		curEnvStr += lstrlenW(curEnvStr) + 1;
	}

	// Cleanup
	FreeEnvironmentStrings(envStrings);
}
