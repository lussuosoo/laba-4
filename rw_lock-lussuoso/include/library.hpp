#ifndef RWLock_H
#define RWLock_H

#include <atomic>

namespace template_library {
    class RWSpinLock {
    private:
        std::atomic<uint32_t> state_{0}; // младший бит - флаг записи, остальные - счетчик читателей
        static constexpr uint32_t kWriteFlag = 1;
        static constexpr uint32_t kReadIncrement = 2;


    public:
        void LockRead() {
            uint32_t expected = state_.load(std::memory_order_relaxed);
            while (true) {
                // Ждем, пока флаг записи не снят (младший бит = 0)
                while ((expected & kWriteFlag) != 0) {
                    expected = state_.load(std::memory_order_relaxed);
                    std::this_thread::yield();
                }
                
                // Пытаемся увеличить счетчик читателей
                uint32_t desired = expected + kReadIncrement;
                if (state_.compare_exchange_weak(expected, desired, 
                                               std::memory_order_acquire,
                                               std::memory_order_relaxed)) {
                    break;
                }
            }
        }

        void UnlockRead() {
            // Уменьшаем счетчик читателей
            state_.fetch_sub(kReadIncrement, std::memory_order_release);
        }

        void LockWrite() {
            uint32_t expected = 0;
            while (true) {
                // Ждем, пока нет читателей и флаг записи не установлен
                while ((expected & ~kWriteFlag) != 0 || (expected & kWriteFlag) != 0) {
                    expected = state_.load(std::memory_order_relaxed);
                    std::this_thread::yield();
                }
                
                // Пытаемся установить флаг записи
                uint32_t desired = expected | kWriteFlag;
                if (state_.compare_exchange_weak(expected, desired,
                                                 std::memory_order_acquire,
                                                 std::memory_order_relaxed)) {
                    break;
                }
            }
            
            // Ждем, пока все читатели завершат работу
            while ((state_.load(std::memory_order_acquire) & ~kWriteFlag) != 0) {
                std::this_thread::yield();
            }   
        }

        void UnlockWrite() {
            // Снимаем флаг записи
            state_.fetch_and(~kWriteFlag, std::memory_order_release);
        }
    };
}

#endif // RWLock_H