#include <iostream>
#include <thread>

#include "Utilities/ThreadWatcher.h"

namespace LCN::Utilities
{
    thread_local ThreadWatcher ThreadWatcher::watcher{};
    
    ThreadWatcher::ThreadWatcher()
    {
        std::cout << "Thread #" << std::this_thread::get_id() << " - Hello world !\n";
    }

    ThreadWatcher::~ThreadWatcher()
    {
        std::cout << "Thread #" << std::this_thread::get_id() << " - Goodbye world !\n";
    }
}