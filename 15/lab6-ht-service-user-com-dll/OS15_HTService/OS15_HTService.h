#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <string>
#include "sddl.h"

#include "d:\\1POIT\\3\\OSI\\labs\\ОССП_Лабораторная_работа_15 (6 чаcoв)\\lab6-ht-service-user-com-dll\\OS15_HTCOM_LIB\\pch.h"
#include "d:\\1POIT\\3\\OSI\\labs\\ОССП_Лабораторная_работа_15 (6 чаcoв)\\lab6-ht-service-user-com-dll\\OS15_HTCOM_LIB\\OS15_HTCOM_LIB.h"

#define SERVICENAME L"kuskus"
#define HTPATH L"d:\\1POIT\\3\\OSI\\labs\\HTspace.ht"

#define USERNAME L"HTUser01"
#define PASSWORD L"1234"

#define TRACEPATH L"d:\\1POIT\\3\\OSI\\labs\\service.trace"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

SECURITY_ATTRIBUTES getSecurityAttributes();
HANDLE createStopEvent(const wchar_t* stopEventName);
void startService();
void trace(const char* msg, int r = std::ofstream::app);
