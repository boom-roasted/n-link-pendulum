#include "MainWindow.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int
main(int argc, char* args[])
{
    auto mainWindow = MainWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    mainWindow.runLoop();

    return 0;
}
