#include "MainWindow.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int
main(int argc, char* args[])
{
    // Set up the main window, handles all the UI etc
    auto mainWindow = MainWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

    // So it begins
    mainWindow.runLoop();

    return 0;
}
