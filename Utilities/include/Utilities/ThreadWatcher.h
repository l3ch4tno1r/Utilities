#pragma once

namespace LCN::Utilities
{
    class ThreadWatcher
    {
    public:
        static void Initialize();

    private:
        ThreadWatcher();
        ~ThreadWatcher();

        thread_local static ThreadWatcher watcher;
    };
}