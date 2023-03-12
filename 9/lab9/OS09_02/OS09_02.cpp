#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>

BOOL printFileText(LPWSTR fileName) {
	try {
		std::cout << "\n\tprintFileText\n";
		HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

BOOL delRowFileTxt(LPWSTR fileName, DWORD row) {
	std::cout << "\n\tdelRowFileTxt\n";
	char filepath[20];
	wcstombs(filepath, fileName, 20);
	std::cout << "Path to file: " << filepath << "\n";
	std::cout << "Row to delete: " << row << "\n\n";

	try
	{
		HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
		std::cout << "\tBEFORE:\n";
		std::cout << buf << std::endl;
		CloseHandle(hf);

		HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		char editedBuf[1024];
		ZeroMemory(editedBuf, sizeof(editedBuf));

		int line = 1;
		int j = 0;
		bool rowFound = false;
		for (int i = 0; i < n; i++) {
			if (line == row) {
				rowFound = true;
			}
			else {
				editedBuf[j] = buf[i];
				j++;
			}

			if (buf[i] == '\n') {
				line++;
			}
		}
		if (!rowFound) {
			CloseHandle(hAppend);
			throw ("Row was not found\n");
		}

		b = WriteFile(hAppend, editedBuf, n, &n, NULL);
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

int main() {
	setlocale(0, "ru");
	char text[] = "../OS09_01.txt";
	LPWSTR file = (LPWSTR)(L"../os09_01.txt");

	delRowFileTxt(file, 1);
	delRowFileTxt(file, 3);
	delRowFileTxt(file, 8);
	delRowFileTxt(file, 10);

	printFileText(file);

	system("pause");
	return 0;
}