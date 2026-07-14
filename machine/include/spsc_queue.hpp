#pragma once
#include <atomic>
#include <array>
#include <cstdint>
#include <immintrin.h> // _mm_pause

template<typename T, size_t Capacity>
class SPSCQueue {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of 2");
    static constexpr size_t MASK = Capacity - 1;

public:
    bool push(const T& item) noexcept {
        const uint64_t head = head_.load(std::memory_order_relaxed);
        if (head - tail_.load(std::memory_order_acquire) >= Capacity)
            return false; // full
        buffer_[head & MASK] = item;
        head_.store(head + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& out) noexcept {
        const uint64_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire))
            return false; // empty
        out = buffer_[tail & MASK];
        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }

private:
    // Each atomic on its own cache line — prevents false sharing between
    // the producer (writes head_) and consumer (writes tail_).
    alignas(64) std::atomic<uint64_t> head_{0};
    alignas(64) std::atomic<uint64_t> tail_{0};
    alignas(64) std::array<T, Capacity> buffer_;
};