#pragma once
#pragma comment(lib, "ws2_32")

#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <io.h>

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

// Code written by Maksymilian Radwanski for academic programs @ 2024
// Modified @ 2025 for standalone usage

void ErrorHandler();
void ErrorHandler(void(*callBeforeExit)(void));
void ErrorHandler(std::string customMessage);
void ErrorHandler(std::string customMessage, void (*callBeforeExit)(void));
void WSAErrorHandler();
void WSAErrorHandler(std::string customMessage);

void SetWideOutput();
void SetNormalOutput();

WCHAR* wstowc(std::wstring s);
WCHAR* wstowc(std::wstring s, unsigned int additionalSpace);
WCHAR* stowc(std::string s);
WCHAR* stowc(std::string s, unsigned int additionalSpace);
std::wstring LPWCHtWSTR(LPWCH envVar);
WCHAR* WSTRtWCHAR(std::wstring envVar);