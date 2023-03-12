#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <fileapi.h>

BOOL printFileText(LPWSTR fileName) {
	try {
		std::cout << "\n\tprintFileText\n";
		HANDLE hf = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//совместное чтение и запись
		if (hf == INVALID_HANDLE_VALUE) throw "create or open file failed";

		DWORD  n = NULL;
		char buf[1024];

		ZeroMemory(buf, sizeof(buf));
		BOOL b = ReadFile(hf, &buf, 1024, &n, NULL);
		if (!b) throw "read file failed";
		std::cout << buf << std::endl;
		CloseHandle(hf);
		return true;
	}
	catch (const char* em) {
		std::cout << "error:" << em << std::endl;
		return false;
	}
}

BOOL insRowFileTxt(LPWSTR fileName, LPWSTR str, DWORD row) {
	char filepath[20];
	wcstombs(filepath, fileName, 20);
	char stringToInsert[50];
	wcstombs(stringToInsert, str, 50);

	std::cout << "Path to file: " << filepath << "\n";
	std::cout << "String to insert: " << stringToInsert << "\n";
	std::cout << "Row to insert: " << row << "\n\n";

	try
	{
		HANDLE hf = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hf == INVALID_HANDLE_VALUE) {
			CloseHandle(hf);
			throw "create or open file failed";
		}

		DWORD n = NULL;
		char buf[1024];
		BOOL b;

		ZeroMemory(buf, sizeof(buf));
		b = ReadFile(hf, &buf, sizeof(buf), &n, NULL);
		if (!b) {
			CloseHandle(hf);
			throw ("Read file unsuccessful\n");
		}

		if (!b) {
			std::cout << "Read file unsuccessfully\n";
			CloseHandle(hf);
			return false;
		}
		std::cout << "\tBEFORE:\n";
		std::cout << buf << std::endl;
		CloseHandle(hf);

		HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		char editedBuf[1024];
		ZeroMemory(editedBuf, sizeof(editedBuf));

		int line = 1;
		int j = 0;
		int k = 0;
		for (int i = 0; i < n; i++) {
			if (line == row) {
				for (int k = 0; k < sizeof(stringToInsert); k++) {
					editedBuf[j] = str[k];
					j++;
					if (str[k + 1] == '\0') { //конец цепочки символов
						editedBuf[j] = '\r'; //	возврат каретки
						j++;
						editedBuf[j] = '\n'; //перевод строки
						j++;
						row = 0;
						break;
					}
				}
				i--;
			}
			else {
				editedBuf[j] = buf[i];
				j++;
			}

			if (buf[i] == '\n') {
				line++;
			}

			if (buf[i + 1] == '\0' && row == -1) {
				for (int k = 0; k < sizeof(stringToInsert); k++) {
					editedBuf[j] = str[k];
					j++;
					if (str[k + 1] == '\0') {
						editedBuf[j] = '\r';
						j++;
						editedBuf[j] = '\n';
						j++;
						row = 0;
						break;
					}
				}
			}
		}

		b = WriteFile(hAppend, editedBuf, j, &n, NULL);
		if (!b) {
			CloseHandle(hAppend);
			throw ("Write file unsuccessful\n");
		}

		std::cout << "\tAFTER:\n";
		std::cout << editedBuf << std::endl;
		CloseHandle(hAppend);
		std::cout << "---------------------------------------------------------\n";
		return true;
	}
	catch (const char* em) {
		std::cout << "!! Error: " << em << " \n";
		std::cout << "---------------------------------------------------------\n";
		return false;
	}
}


int main()
{
	setlocale(0, "ru");
	LPWSTR  file = (LPWSTR)(L"../os09_01.txt");

	char str[] = "7777777777";
	wchar_t wStr[50];
	mbstowcs(wStr, str, strlen(str) + 1);
	LPWSTR strToIns = wStr;

	insRowFileTxt(file, strToIns, 1);
	insRowFileTxt(file, strToIns, -1);
	insRowFileTxt(file, strToIns, 5);
	insRowFileTxt(file, strToIns, 7);

	printFileText(file);

	system("pause");
}