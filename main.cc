#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    wchar_t currentPath[MAX_PATH];
    DWORD length = GetCurrentDirectoryW(sizeof(currentPath) / sizeof(wchar_t), currentPath);
    if (length == 0) {
        std::wcerr << L"Failed to get current working directory. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    if (!OpenClipboard(NULL)) {
        std::wcerr << L"Failed to open clipboard. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (length + 1) * sizeof(wchar_t));
    if (hMem == NULL) {
        CloseClipboard();
        std::wcerr << L"Failed to allocate memory for clipboard data. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    wchar_t* ptr = static_cast<wchar_t*>(GlobalLock(hMem));
    if (ptr == NULL) {
        GlobalFree(hMem);
        CloseClipboard();
        std::wcerr << L"Failed to lock memory for clipboard data. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    wcscpy(ptr, currentPath);

    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);

    CloseClipboard();

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrowPath = converter.to_bytes(currentPath);
    std::wcout << L"Current working directory has been copied to clipboard: " << currentPath << std::endl;
    std::cout << "Current working directory has been copied to clipboard: " << narrowPath << std::endl;

    HWND consoleWindow = GetConsoleWindow();
    
    ShowWindow(consoleWindow, SW_HIDE);

    return 0;
}
