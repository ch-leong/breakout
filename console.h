#pragma once
#include <windows.h>

#define ToXY(x, y) Console::SetCursorPos(x, y)

namespace Console
{
void Load();
void SetCursorPos(int x, int y);

extern HANDLE handle;
extern HWND hwnd;
extern int width;
extern int height;
extern int scale;
}
