#include "winErrorHandlers.h"

void ErrorHandler() {
    LPVOID lpMsgBuffer;
    DWORD errorCode = GetLastError();

    if (errorCode == 0) {
        std::cout << "ErrorHandler() called but no error code was reported! Continuing program execution..." << std::endl;
        return;
    }
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpMsgBuffer, 0, NULL) == 0) {
        printf("Failed to format error message!\n");
        LocalFree(lpMsgBuffer);
        ExitProcess(errorCode);
    }
    else {
        // ani cout ani wcout nie działa poprawnie z lpMsgBuffer
        printf("Error message: %ls", (std::wstring*)lpMsgBuffer);
        LocalFree(lpMsgBuffer);
        ExitProcess(errorCode);
    }
}
void ErrorHandler(void(*callBeforeExit)(void)) {
    LPVOID lpMsgBuffer;
    DWORD errorCode = GetLastError();

    if (errorCode == 0) {
        std::cout << "ErrorHandler() called but no error code was reported! Continuing program execution..." << std::endl;
        return;
    }
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpMsgBuffer, 0, NULL) == 0) {
        printf("Failed to format error message!\n");
        LocalFree(lpMsgBuffer);
        callBeforeExit();
        ExitProcess(errorCode);
    }
    else {
        // ani cout ani wcout nie działa poprawnie z lpMsgBuffer
        printf("Error message: %ls", (std::wstring*)lpMsgBuffer);
        LocalFree(lpMsgBuffer);
        callBeforeExit();
        ExitProcess(errorCode);
    }
}
void ErrorHandler(std::string customMessage) {
    std::cout << customMessage << std::endl;
    ErrorHandler();
}
void ErrorHandler(std::string customMessage, void(*callBeforeExit)(void)) {
    std::cout << customMessage << std::endl;
    ErrorHandler(callBeforeExit);
}
void WSAErrorHandler() {
    LPVOID lpMsgBuffer;
    DWORD errorCode = WSAGetLastError();

    if (errorCode == 0) {
        std::cout << "ErrorHandler() called but no error code was reported! Continuing program execution..." << std::endl;
        return;
    }
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&lpMsgBuffer, 0, NULL) == 0) {
        printf("Failed to format error message!\n");
        LocalFree(lpMsgBuffer);
        WSACleanup();
        ExitProcess(errorCode);
    }
    else {
        // ani cout ani wcout nie działa poprawnie z lpMsgBuffer
        printf("Error message: %ls", (std::wstring*)lpMsgBuffer);
        LocalFree(lpMsgBuffer);
        WSACleanup();
        ExitProcess(errorCode);
    }
}
void WSAErrorHandler(std::string customMessage) {
    std::cout << customMessage << std::endl;
    WSAErrorHandler();
}

void SetWideOutput() {
    std::cout.flush();
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1 || std::wcout.fail()) {
        ErrorHandler("Failed to change console mode to wide output!");
    }
}
void SetNormalOutput() {
    std::wcout.flush();
    if (_setmode(_fileno(stdout), _O_TEXT) == -1 || std::cout.fail()) {
        ErrorHandler("Failed to change console mode to normal output!");
    }
}

WCHAR* wstowc(std::wstring s) {
    WCHAR* retCharArr = new WCHAR[s.length() + 1];
    for (int i = 0; i < s.length(); i++) {
        retCharArr[i] = s[i];
    }
    retCharArr[s.length()] = '\0';
    return retCharArr;
}
WCHAR* wstowc(std::wstring s, unsigned int additionalSpace) {
    WCHAR* retCharArr = new WCHAR[s.length() + 1 + additionalSpace];
    for (int i = 0; i < s.length(); i++) {
        retCharArr[i] = s[i];
    }
    retCharArr[s.length() + additionalSpace] = '\0';
    return retCharArr;
}
WCHAR* stowc(std::string s) {
    WCHAR* retCharArr = new WCHAR[s.length() + 1];
    for (int i = 0; i < s.length(); i++) {
        retCharArr[i] = s[i];
    }
    retCharArr[s.length()] = '\0';
    return retCharArr;
}
WCHAR* stowc(std::string s, unsigned int additionalSpace) {
    WCHAR* retCharArr = new WCHAR[s.length() + 1 + additionalSpace];
    for (int i = 0; i < s.length(); i++) {
        retCharArr[i] = s[i];
    }
    retCharArr[s.length() + additionalSpace] = '\0';
    return retCharArr;
}
std::wstring LPWCHtWSTR(LPWCH envVar) {
    if (envVar[0] == '\0' || envVar[1] == '\0') return L"";
    unsigned long envVarLength = 0;
    std::wstring retVal = L"";

    // Zliczanie długości zmiennych środowiskowych
    for (envVarLength = 1; envVar[envVarLength - 1] != '\0' && envVar[envVarLength] != '\0'; envVarLength++);

    // Kopiowanie zmiennych do wstringa
    retVal.resize(envVarLength, '\0');
    for (int i = 0; i < envVarLength; i++) {
        retVal[i] = envVar[i];
    }
    return retVal;
}
WCHAR* WSTRtWCHAR(std::wstring envVar) {
    if (envVar[0] == '\0' || envVar[1] == '\0') return NULL;
    unsigned long envVarLength = 1;
    for (envVarLength = 1; envVar[(unsigned long long)envVarLength - 1] != '\0' && envVar[envVarLength] != '\0'; envVarLength++);

    WCHAR* retVal = new WCHAR[envVarLength];
    for (int i = 0; i < envVarLength; i++) {
        retVal[i] = envVar[i];
    }

    return retVal;
}