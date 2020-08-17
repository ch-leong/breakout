#include "console.h"
#include "breakout.h"

int main()
{
    Console::Load();
    Breakout::Load();
    while (Breakout::isRunning)
    {
        Breakout::Input();
        Breakout::Update();
        ::Sleep(60);
    }
    system("pause");

    return 0;
}
