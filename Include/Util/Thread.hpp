#pragma once

#include <thread>

namespace Aurora::Util
{
    class Thread
    {
    private:
        void loop()
        {
            while(running)
            {
                process();
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
            }
        };

        bool running;
        std::thread thread;
        int sleepMs;

    protected:
        Thread(int sleepMs) : running(false), sleepMs(sleepMs) {}
        virtual void process() {};

    public:
        virtual ~Thread() = default;

        void run()
        {
            if(!running)
            {
                running = true;
                thread = std::thread(&Thread::loop, this);
            }
        }

        void stop()
        {
            if(running)
            {
                running = false;
                if(thread.joinable()) thread.join();
            }
        }
    };
}