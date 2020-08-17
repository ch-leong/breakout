#include "console.h"
#include <wchar.h>
#include <assert.h>

namespace Console
{
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
HWND hwnd = GetConsoleWindow();
int width = 50;
int height = 50;
int scale = 11;

CONSOLE_FONT_INFOEX Font{ (ULONG)sizeof(CONSOLE_FONT_INFOEX) };
int const area = width * height;
RECT deskSize; // Desktop resolution.
CONSOLE_SCREEN_BUFFER_INFO csbi; // Current Console Buffer Info.
SMALL_RECT windowRect; // New Console Size.
COORD coordScreen; // Get the Largest Size we can size the Console Window to.

void Load()
{
    swprintf_s(Font.FaceName, L"Terminal");
    assert(SetCurrentConsoleFontEx(handle, FALSE, &Font) != 0);
    Font.dwFontSize.Y = Font.dwFontSize.X = (SHORT)scale;
    assert(SetCurrentConsoleFontEx(handle, FALSE, &Font) != 0);

    GetWindowRect(GetDesktopWindow(), &deskSize);
    MoveWindow(hwnd, (deskSize.right >> 1) - ((height >> 1) * scale), (deskSize.bottom >> 1) - ((height >> 1) * scale), 0, 0, FALSE); // Center window.
    assert(GetConsoleScreenBufferInfo(handle, &csbi) != 0);

    // Define the New Console Window Size and Scroll Position.
    coordScreen = GetLargestConsoleWindowSize(handle);
    windowRect.Right = (SHORT)(min((SHORT)width, coordScreen.X) - 1);
    windowRect.Bottom = (SHORT)(min((SHORT)height, coordScreen.Y) - 1);
    windowRect.Left = windowRect.Top = (SHORT)0;

    // Define the New Console Buffer Size.
    coordScreen.X = (SHORT)width;
    coordScreen.Y = (SHORT)height;

    // Buffer size & window size adjustment.
    if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD)area)
    {
        assert(SetConsoleWindowInfo(handle, TRUE, &windowRect) != 0);
        assert(SetConsoleScreenBufferSize(handle, coordScreen) != 0);
    }
    if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y < (DWORD)area)
    {
        assert(SetConsoleScreenBufferSize(handle, coordScreen) != 0);
        assert(SetConsoleWindowInfo(handle, TRUE, &windowRect) != 0);
    }
}

void SetCursorPos(int x, int y)
{
    COORD coord{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(handle, coord);
}

}
