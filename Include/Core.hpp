#pragma once

#include <thread>

class Core
{
private:
    std::thread mainThread;

public:
    void acceptLoop();

    void run();
    void stop();
};