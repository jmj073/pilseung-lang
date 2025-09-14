#pragma once

#include <unistd.h>
#include <cstdio>
#include <regex>
#include <ranges>

struct wregex_view: std::ranges::view_interface<wregex_view> {
    using iterator = std::wsregex_iterator;

    wregex_view(const std::wstring& str, const std::wregex& re)
        : m_str(&str), m_re(&re) {}

    iterator begin() const {
        return std::wsregex_iterator(m_str->begin(), m_str->end(), *m_re);
    }

    iterator end() const {
        return std::wsregex_iterator();
    }

private:
    const std::wstring* m_str;
    const std::wregex* m_re;
};

static inline bool stdin_is_tty() {
    return isatty(fileno(stdin));
}

static inline bool stdout_is_tty() {
    return isatty(fileno(stdout));
}
