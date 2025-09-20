#include "memory.h"

namespace ps {

    Memory::Memory()
        : m_mem()
    { }

    Memory::Memory(Memory&& other)
        : m_mem(std::move(other.m_mem))
    { }

    Memory& Memory::operator=(Memory&& other) {
        m_mem = std::move(other.m_mem);
        return *this;
    }

    std::optional<uintptr_t> Memory::alloc(size_t sz, std::optional<uintptr_t> ptr) {
        auto rit = m_remain.end();
        uintptr_t i{};
        if (ptr) {
            if (!is_free(*ptr, sz)) {
                return std::nullopt;
            }
            i = *ptr;

            for (auto it = m_remain.begin(); it != m_remain.end(); ++it) {
                uintptr_t begin = it->second;
                uintptr_t end = it->second + it->first;
                if (begin <= i && i < end) {
                    rit = it;
                    break;
                }
            }
        } else {
            auto it = m_remain.lower_bound(sz);
            if (it == m_remain.end()) {
                return std::nullopt;
            }
            i = it->second;
            rit = it;
        }

        if (rit == m_remain.end()) {
            return std::nullopt;
        }

        uintptr_t left_ptr = rit->second;
        size_t left_sz = i - rit->second;
        uintptr_t right_ptr = i + sz;
        size_t right_sz = (rit->second + rit->first) - (i + sz);
        
        m_remain.erase(rit);

        if (left_sz) {
            m_remain.insert({ left_sz, left_ptr });
        }
        if (right_sz) {
            m_remain.insert({ right_sz, right_ptr });
        }

        auto ret = i;
        
        for (uintptr_t end = i + sz; i < end; ++i) {
            m_mem.insert({ i, uint8_t(0) });
        }

        return ret;
    }

    bool Memory::is_alloced(uintptr_t ptr, size_t sz) const {
        for (uintptr_t end = ptr + sz; ptr < end; ++ptr) {
            if (!m_mem.contains(ptr)) {
                return false;
            }
        }
        return true;
    }

    bool Memory::is_free(uintptr_t ptr, size_t sz) const {
        for (uintptr_t end = ptr + sz; ptr < end; ++ptr) {
            if (m_mem.contains(ptr)) {
                return false;
            }
        }
        return true;
    }
} // namespace ps
