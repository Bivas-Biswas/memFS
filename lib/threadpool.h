#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>

// #include "global.h"
#include "types.h"
#include "lockfreequeue.h"

class ThreadPool
{
    // Vector of worker threads
    std::vector<std::thread> workers;

    // Assigner thread
    std::thread assigner;

    // Flags to indicate whether each worker thread is busy or free
    std::vector<std::atomic<bool>> is_worker_busy;

    // Task assigned to each worker
    std::vector<OperationTask> worker_tasks;

    // Flag to stop the thread pool
    std::atomic<bool> stop;

    LockFreeQueue<OperationTask> *ready_queue;

public:
    // Constructor
    ThreadPool(int num_threads, LockFreeQueue<OperationTask> *ready_queue_ptr)
        : stop(false), is_worker_busy(num_threads), worker_tasks(num_threads), ready_queue(ready_queue_ptr)
    {
        // Initialize all workers as free
        for (int i = 0; i < num_threads; ++i)
        {
            is_worker_busy[i] = false;
            worker_tasks[i] = nullptr; // No task initially
            workers.emplace_back(&ThreadPool::workerThread, this, i);
        }
        // Start the assigner thread
        assigner = std::thread(&ThreadPool::assignerThread, this);
    }

    // Destructor
    ~ThreadPool()
    {
        // Set stop flag
        stop.store(true);

        // Join all worker threads
        for (std::thread &worker : workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
        // Join assigner thread
        if (assigner.joinable())
        {
            assigner.join();
        }
    }

    // Worker thread function
    void workerThread(int id)
    {
        while (!stop.load())
        {
            OperationTask operationTask;

            // Check if the worker has been assigned a task
            if (!worker_tasks[id])
            {
                // Sleep to avoid busy-waiting
                continue;
            }

            // Get the task assigned by the assigner thread
            operationTask = worker_tasks[id];
            worker_tasks[id] = nullptr; // Clear the task after acquiring it

            if (operationTask)
            {
                // Execute the task
                operationTask();

                // // move it to blocked queue
                // blocked_tasks.enqueue(task);

                // Mark worker as free after finishing the task
                is_worker_busy[id].store(false);
            }
        }
    }

    // Assigner thread function
    void assignerThread()
    {
        while (!stop.load())
        {
            OperationTask operationTask;

            // Lock the queue and check if there are tasks to assign
            {
                if (ready_queue->empty())
                {
                    // Sleep if no tasks are available
                    continue;
                }
            }

            // Find a free worker
            int free_worker_id = getFreeWorker();
            if (free_worker_id != -1)
            {
                {
                    if (!ready_queue->empty())
                    {
                        // Get a task from the queue
                        ready_queue->dequeue(operationTask);
                    }
                }

                if (operationTask)
                {
                    // Assign the task to the free worker
                    worker_tasks[free_worker_id] = std::move(operationTask);
                    is_worker_busy[free_worker_id].store(true); // Mark worker as busy
                }
            }
        }
    }

    // Utility function to find the first free worker thread
    int getFreeWorker()
    {
        for (int i = 0; i < is_worker_busy.size(); ++i)
        {
            if (!is_worker_busy[i].load())
            {
                return i; // Return the ID of the free worker
            }
        }
        return -1; // No free workers available
    }
};

#endif