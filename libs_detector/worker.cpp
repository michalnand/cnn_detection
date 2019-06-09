#include <worker.h>
#include <iostream>

Worker::Worker()
{
    is_running  = true;
    is_done     = false;
    run      = true;

    main_thread = new std::thread(&Worker::main_thread_func, this);
}


Worker::~Worker()
{
    is_running  = false;
    is_done     = false;
    run      = true;

    start();

    main_thread->join();
    delete main_thread;
}

void Worker::start()
{
    is_done = false;
    run  = true;
}

void Worker::wait_for_done()
{
    while (is_done == false)
    {

    }
}

void Worker::main()
{
    std::cout << "running worker with ID " << (unsigned long int)this << "\n";

    volatile unsigned int cnt = 100000000;

    while (cnt--)
    {

    }
}

void Worker::main_thread_func()
{
    while (is_running)
    {
        while (run == false)
        {

        }

        run  = false;

        if (is_running)
            main();

        is_done = true;
    }
}
