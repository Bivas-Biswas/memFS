#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>
#include <exception>

typedef std::function<void()> OperationTask;

class ThreadPool {
    std::vector<std::thread> workers;
    std::thread assigner;
    std::vector<std::atomic<bool>> is_worker_busy;
    std::vector<std::atomic<OperationTask*>> worker_tasks;  // Changed to atomic pointer
    std::atomic<bool> stop;
    LockFreeQueue<OperationTask>* ready_queue;

public:
    ThreadPool(int num_threads, LockFreeQueue<OperationTask>* ready_queue_ptr)
        : stop(false), 
          is_worker_busy(num_threads), 
          worker_tasks(num_threads),
          ready_queue(ready_queue_ptr)
    {
        for (int i = 0; i < num_threads; ++i) {
            is_worker_busy[i] = false;
            worker_tasks[i].store(nullptr);
            workers.emplace_back(&ThreadPool::workerThread, this, i);
        }
        assigner = std::thread(&ThreadPool::assignerThread, this);
    }

    ~ThreadPool() {
        stop.store(true);
        
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        
        if (assigner.joinable()) {
            assigner.join();
        }

        // Cleanup any remaining tasks
        for (auto& task_ptr : worker_tasks) {
            OperationTask* task = task_ptr.load();
            if (task != nullptr) {
                delete task;
            }
        }
    }

    void workerThread(int id) {
        while (!stop.load(std::memory_order_acquire)) {
            OperationTask* task_ptr = worker_tasks[id].load(std::memory_order_acquire);
            
            if (task_ptr == nullptr) {
                std::this_thread::yield();  // Better than busy waiting
                continue;
            }

            // Get the task and clear the slot
            if (worker_tasks[id].compare_exchange_strong(task_ptr, nullptr)) {
                try {
                    if (task_ptr && *task_ptr) {  // Double check validity
                        (*task_ptr)();  // Execute the task
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Task execution failed: " << e.what() << std::endl;
                }
                
                delete task_ptr;  // Clean up the task
                is_worker_busy[id].store(false, std::memory_order_release);
            }
        }
    }

    void assignerThread() {
        while (!stop.load(std::memory_order_acquire)) {
            if (ready_queue->empty()) {
                std::this_thread::yield();
                continue;
            }

            int free_worker_id = getFreeWorker();
            if (free_worker_id != -1) {
                OperationTask task;
                if (ready_queue->dequeue(task)) {
                    OperationTask* task_ptr = new OperationTask(std::move(task));
                    
                    // Try to assign the task
                    OperationTask* expected = nullptr;
                    if (worker_tasks[free_worker_id].compare_exchange_strong(
                            expected, task_ptr)) {
                        is_worker_busy[free_worker_id].store(true, std::memory_order_release);
                    } else {
                        delete task_ptr;  // Failed to assign, clean up
                    }
                }
            }
            std::this_thread::yield();
        }
    }

    int getFreeWorker() {
        for (size_t i = 0; i < is_worker_busy.size(); ++i) {
            bool expected = false;
            if (is_worker_busy[i].compare_exchange_strong(expected, true)) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
};

#endif