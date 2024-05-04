#include <iostream>
#include <vector>
#include <windows.h>

int main() {
    char currentPath[MAX_PATH];
    DWORD length = GetCurrentDirectoryA(sizeof(currentPath), currentPath);
    if (length == 0) {
        std::cerr << "Failed to get current working directory. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    if (!OpenClipboard(NULL)) {
        std::cerr << "Failed to open clipboard. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length + 1);
    if (hMem == NULL) {
        CloseClipboard();
        std::cerr << "Failed to allocate memory for clipboard data. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    char* ptr = static_cast<char*>(GlobalLock(hMem));
    if (ptr == NULL) {
        GlobalFree(hMem);
        CloseClipboard();
        std::cerr << "Failed to lock memory for clipboard data. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    strcpy(ptr, currentPath);

    GlobalUnlock(hMem);

    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();

    std::cout << "Current working directory has been copied to clipboard: " << currentPath << std::endl;

    HWND consoleWindow = GetConsoleWindow();
    
    ShowWindow(consoleWindow, SW_HIDE);

    return 0;
}
