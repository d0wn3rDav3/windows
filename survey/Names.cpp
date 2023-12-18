#include <Windows.h>
#include <stdio.h>

int main()
{
    // Setup variables for the system name
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1] = { 0 };
    DWORD buf = sizeof(computerName) / sizeof computerName[0];

    // Get Current system hostname
    if (GetComputerName(computerName, &buf))
    {
	fprintf(stdout, "[+] Computer Host Name: %ws\n", computerName);
    }
    else
    {
	fprintf(stderr, "[!] Could not enumerate the host name: %ld\n", GetLastError());
	return EXIT_FAILURE;
    }

    // Current User Token
    HANDLE hToken;
    DWORD dwBufferSize;
    PTOKEN_USER pTokenUser;
    wchar_t szUserName[MAX_PATH];
    DWORD dwUserNameSize = MAX_PATH;

    // Open the current running processes token
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
	fprintf(stderr, "[!] Error Opening Process Token: %ld\n", GetLastError());
	return EXIT_FAILURE;
    }

    // Grab the process' token information
    if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize) && GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
	fprintf(stderr, "[!] Error getting token information size: %ld\n", GetLastError());
	return EXIT_FAILURE;
    }

    // Should this be a HeapAlloc(GetProcessHeap()) call?
    // Malloc the buffer to save the Token User info
    pTokenUser = (PTOKEN_USER)malloc(dwBufferSize);
    if (pTokenUser == NULL)
    {
	fprintf(stderr, "[!] Malloc Error: %ld\n", GetLastError());
	CloseHandle(hToken);
	return EXIT_FAILURE;
    }

    // Get current user SID from process token.
    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwBufferSize, &dwBufferSize))
    {
	fprintf(stderr, "[!] Error getting token information: %ld\n", GetLastError());
	free(pTokenUser);
	CloseHandle(hToken);
	return EXIT_FAILURE;
    }

    // If we've made it here setup SID Variables
    SID_NAME_USE sidNameUse;
    DWORD dwDomainSize = MAX_PATH;
    wchar_t szDomain[MAX_PATH];

    // Convert SID to username
    if (!LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &dwUserNameSize, szDomain, &dwDomainSize, &sidNameUse))
    {
	fprintf(stderr, "[!] Error converting SID to username: %ld\n", GetLastError());
	return EXIT_FAILURE;
    }
    else
    {
	fprintf(stdout, "[+] Current  User: %ws\n", szUserName);
    }

    // Cleanup
    free(pTokenUser);
    CloseHandle(hToken);
	
    return EXIT_SUCCESS;
}
