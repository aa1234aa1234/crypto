#include "Application.h"

int main(int argc, char **argv)
{
    Application app;
    Application::isRunning = 1;
    app.run();
}