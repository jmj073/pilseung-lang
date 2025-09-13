#pragma once

#include <regex>
#include <ranges>

#include "ast.h"

using namespace std;
using namespace ranges;

struct regex_view : view_interface<regex_view> {
    using iterator = sregex_iterator;

    regex_view(const string& str, const regex& re)
        : m_str(&str), m_re(&re) {}

    iterator begin() const {
        return sregex_iterator(m_str->begin(), m_str->end(), *m_re);
    }

    iterator end() const {
        return sregex_iterator();
    }

private:
    const string* m_str;
    const regex* m_re;
};

auto tokenize(const std::string& code) {
    auto re = std::regex(R"([ \t]*(!+|님|께|신!!?고!합니다!|경례!|대하여!|바로!|(?:필!*)+승!+|[\n\r\f]+|[^\s님께]+))");
    return regex_view(code, re);
}

ps::Program tokenize_and_parse(const std::string& code);
