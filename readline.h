#pragma once

#include <string>
#include <optional>

class ReadLine {
public:
    std::optional<std::wstring> get(const std::wstring& prompt);
};
