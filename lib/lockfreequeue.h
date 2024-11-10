#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include <memory>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        T value;
        std::atomic<Node*> next;
        
        Node() : next(nullptr) {}
        explicit Node(const T& val) : value(val), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
    std::atomic<size_t> size_;

public:
    LockFreeQueue() : size_(0) {
        Node* dummy = new Node();
        head.store(dummy, std::memory_order_release);
        tail.store(dummy, std::memory_order_release);
    }

    ~LockFreeQueue() {
        while (Node* node = head.load(std::memory_order_acquire)) {
            head.store(node->next.load(std::memory_order_acquire), 
                      std::memory_order_release);
            delete node;
        }
    }

    void enqueue(const T& value) {
        Node* new_node = new Node(value);
        
        while (true) {
            Node* last = tail.load(std::memory_order_acquire);
            Node* next = last->next.load(std::memory_order_acquire);
            
            if (last == tail.load(std::memory_order_acquire)) {
                if (next == nullptr) {
                    if (last->next.compare_exchange_weak(next, new_node,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        tail.compare_exchange_strong(last, new_node,
                            std::memory_order_release,
                            std::memory_order_relaxed);
                        size_.fetch_add(1, std::memory_order_release);
                        return;
                    }
                } else {
                    tail.compare_exchange_weak(last, next,
                        std::memory_order_release,
                        std::memory_order_relaxed);
                }
            }
        }
    }

    bool dequeue(T& result) {
        while (true) {
            Node* first = head.load(std::memory_order_acquire);
            Node* last = tail.load(std::memory_order_acquire);
            Node* next = first->next.load(std::memory_order_acquire);
            
            if (first == head.load(std::memory_order_acquire)) {
                if (first == last) {
                    if (next == nullptr) {
                        return false;
                    }
                    tail.compare_exchange_weak(last, next,
                        std::memory_order_release,
                        std::memory_order_relaxed);
                } else {
                    result = next->value;
                    if (head.compare_exchange_weak(first, next,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        delete first;
                        size_.fetch_sub(1, std::memory_order_release);
                        return true;
                    }
                }
            }
        }
    }

    bool empty() const {
        Node* first = head.load(std::memory_order_acquire);
        if (first == nullptr) return true;
        
        Node* next = first->next.load(std::memory_order_acquire);
        return next == nullptr;
    }

    size_t size() const {
        return size_.load(std::memory_order_acquire);
    }
};

#endif