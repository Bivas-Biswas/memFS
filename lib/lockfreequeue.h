#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <memory>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeQueue
{
public:
    LockFreeQueue()
    {
        Node *dummy = new Node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue()
    {
        while (Node *node = head.load())
        {
            head.store(node->next.load());
            delete node;
        }
    }

    void enqueue(T value)
    {
        Node *newNode = new Node(value);
        Node *oldTail = nullptr;

        while (true)
        {
            oldTail = tail.load(std::memory_order_acquire);
            Node *next = oldTail->next.load(std::memory_order_acquire);

            if (oldTail == tail.load())
            {
                if (next == nullptr)
                {
                    if (oldTail->next.compare_exchange_weak(next, newNode))
                    {
                        break;
                    }
                }
                else
                {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
        tail.compare_exchange_weak(oldTail, newNode);
    }

    bool dequeue(T &result)
    {
        Node *oldHead = nullptr;

        while (true)
        {
            oldHead = head.load(std::memory_order_acquire);
            
            Node *oldTail = tail.load(std::memory_order_acquire);
            Node *next = oldHead->next.load(std::memory_order_acquire);

            if (oldHead == head.load())
            {
                if (oldHead == oldTail)
                {
                    if (next == nullptr)
                    {
                        return false;
                    }
                    tail.compare_exchange_weak(oldTail, next);
                }
                else
                {
                    result = next->value;
                    if (head.compare_exchange_weak(oldHead, next))
                    {
                        break;
                    }
                }
            }
        }
        delete oldHead;
        return true;
    }

    bool empty() const
    {
        Node *oldHead = head.load(std::memory_order_acquire);
        Node *oldTail = tail.load(std::memory_order_acquire);

        if (oldHead == nullptr || oldTail == nullptr)
        {
            return true; // Consider queue empty on inconsistency
        }

        Node *next = oldHead->next.load(std::memory_order_acquire);

        return (oldHead == oldTail && next == nullptr);
    }

private:
    struct Node
    {
        T value;
        std::atomic<Node *> next;

        Node(T val) : value(val), next(nullptr) {}
    };

    std::atomic<Node *> head;
    std::atomic<Node *> tail;
};

#endif