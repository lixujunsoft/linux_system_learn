#include <iostream>
#include <atomic>
#include <mutex>

using namespace std;

class Singleton {
private:
    Singleton() {};
    Singleton(const Singleton &other) {};
public:
    static Singleton *getInstance();
    static atomic<Singleton*> m_instance;
    static mutex m_mutex;
};

// C++ 11版本之后的跨平台实现（volatile）
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;

Singleton *Singleton::getInstance() {
    Singleton *tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);   // 获取内存fence
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Singleton();
            std::atomic_thread_fence(std::memory_order_release); // 释放内存fence
            m_instance.store(tmp, std::memory_order_release);
        }
    }
    return tmp;
}

int main() {

}