#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <timezoneapi.h>

BOOL printFileText(LPWSTR fileName) {
    try {
        std::cout << "\n\tprintFileText\n";
        HANDLE hf = CreateFile( // Функция CreateFile может создать новый файл или открыть существующий файл.
                               fileName, // имя файла
                               GENERIC_READ, // режим доступа (чтение)
                               NULL, // совместный доступ
                               NULL, // SD (дескр. защиты)
                               OPEN_ALWAYS, // как действовать (Открывает файл, если таковой существует. Если файл не существует, функция создает файл)
                               FILE_ATTRIBUTE_NORMAL, // атрибуты файла
                               NULL // дескр.шаблона файла
        );
        if (hf == INVALID_HANDLE_VALUE) {
            CloseHandle(hf);
            throw "create or open file failed";
        }
                              
        DWORD  n = NULL;
        char buf[1024];

        ZeroMemory(buf, sizeof(buf)); //заполняет блок памяти нулями
        BOOL b = ReadFile(hf, &buf, 1024, &n, NULL); //Функция ReadFile читает данные из файла, начиная с позиции, обозначенной указателем файла.
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

BOOL printFileInfo(LPWSTR fileName) {
    try {
        std::cout << "\tprintFileInfo\n";
        HANDLE hf = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hf == INVALID_HANDLE_VALUE) throw "open file failed";

        char filepath[20];
        wcstombs(filepath, fileName, 20); //Преобразует последовательность расширенных символов в соответствующую последовательность многобайтовых символов. 
        std::cout << "name: " << filepath << "\n";
        std::cout << "type: " << GetFileType(hf) << "\n";   // 1 = FILE_TYPE_DISK (файл на диске)
        std::cout << "size: " << GetFileSize(hf, NULL) << " bytes\n";

        BY_HANDLE_FILE_INFORMATION info; // содержит информацию, извлеченную функцией GetFileInformationByHandle
        ZeroMemory(&info, sizeof(BY_HANDLE_FILE_INFORMATION));
        GetFileInformationByHandle(hf, &info);
        SYSTEMTIME time;

        // получает дату для пояса UCT
        if (!FileTimeToSystemTime(&info.ftCreationTime, &time))
            throw "FileTimeToSystemTime error";

        // UCT -> пояс текущего устройства
        SystemTimeToTzSpecificLocalTime(NULL, &time, &time);
        std::cout << "creation time: " << time.wDay << "." << time.wMonth << "." << time.wYear << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "\n";

        if (!FileTimeToSystemTime(&info.ftLastWriteTime, &time))
            throw "FileTimeToSystemTime error";

        SystemTimeToTzSpecificLocalTime(NULL, &time, &time);
        std::cout << "modified time: " << time.wDay << "." << time.wMonth << "." << time.wYear << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "\n";

        CloseHandle(hf);
        return true;
    }
    catch (const char* em) {
        std::cout << "error:" << em << "\n";
        return false;
    }
}

int main()
{
    setlocale(0, "ru");
    LPWSTR file = (LPWSTR)(L"..\\os09_01.txt");

    printFileInfo(file);
    printFileText(file);
}