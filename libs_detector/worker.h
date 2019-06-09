#ifndef _WORKER_H_
#define _WORKER_H_

#include <atomic>
#include <thread>
#include <mutex>

class Worker
{
    public:
        Worker();
        virtual ~Worker();

        void start();
        void wait_for_done();
    protected:
        virtual void main();

    private:
        void main_thread_func();

    protected:
        std::thread *main_thread;

        std::atomic<bool> is_running;
        std::atomic<bool> run;
        std::atomic<bool> is_done;
};

#endif
