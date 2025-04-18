#ifndef RWLock_H
#define RWLock_H

#include <atomic>

namespace template_library {
    class RWSpinLock {
    private:
        std::atomic<uint32_t> state_{0};
        static constexpr uint32_t kWriteFlag = 1;
        static constexpr uint32_t kReadIncrement = 2;


    public:
        void LockRead() {
            uint32_t expected = state_.load(std::memory_order_relaxed);
            while (true) {
                
                while ((expected & kWriteFlag) != 0) {
                    expected = state_.load(std::memory_order_relaxed);
                    std::this_thread::yield();
                }
                
               
                uint32_t desired = expected + kReadIncrement;
                if (state_.compare_exchange_weak(expected, desired, 
                                               std::memory_order_acquire,
                                               std::memory_order_relaxed)) {
                    break;
                }
            }
        }

        void UnlockRead() {
            
            state_.fetch_sub(kReadIncrement, std::memory_order_release);
        }

        void LockWrite() {
            uint32_t expected = 0;
            while (true) {
                
                while ((expected & ~kWriteFlag) != 0 || (expected & kWriteFlag) != 0) {
                    expected = state_.load(std::memory_order_relaxed);
                    std::this_thread::yield();
                }
                
                
                uint32_t desired = expected | kWriteFlag;
                if (state_.compare_exchange_weak(expected, desired,
                                                 std::memory_order_acquire,
                                                 std::memory_order_relaxed)) {
                    break;
                }
            }
            
            
            while ((state_.load(std::memory_order_acquire) & ~kWriteFlag) != 0) {
                std::this_thread::yield();
            }   
        }

        void UnlockWrite() {
            
            state_.fetch_and(~kWriteFlag, std::memory_order_release);
        }
    };
}

#endif // RWLock_H
