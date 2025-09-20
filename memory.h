#pragma once

#include <unordered_map>
#include <map>
#include <cstdint>
#include <optional>
#include <limits>

namespace ps {
    class Memory {
        private:
            using Umap = std::unordered_map<uintptr_t, uint8_t>;
            using Map = std::multimap<size_t, uintptr_t>;

        public:
            Memory();
            Memory(const Memory&) = delete;
            Memory(Memory&&);

            Memory& operator=(const Memory&) = delete;
            Memory& operator=(Memory&&);

            std::optional<uintptr_t> alloc(size_t sz, std::optional<uintptr_t> ptr = std::nullopt);

            bool is_alloced(uintptr_t ptr, size_t sz) const;

            bool is_free(uintptr_t ptr, size_t sz) const;

            template <typename T>
            bool set(uintptr_t ptr, const T& data) {
                if (!is_alloced(ptr, sizeof(data))) {
                    return false;
                }

                auto p = (const uint8_t*)&data;
                uintptr_t end = ptr + sizeof(data);

                while (ptr < end) {
                    m_mem[ptr++] = *p++;
                }

                return true;
            }

            template <typename T>
            std::optional<T> get(uintptr_t ptr) {
                if (!is_alloced(ptr, sizeof(T))) {
                    return std::nullopt;
                }
                T data;
                auto p = (uint8_t*)&data;
                uintptr_t end = ptr + sizeof(data);

                while (ptr < end) {
                    *p++ = m_mem[ptr++];
                }

                return data;
            }

        private:
            Umap m_mem;
            Map m_remain{ { std::numeric_limits<size_t>::max(), uintptr_t(0) } };
    };
} // namespace ps
