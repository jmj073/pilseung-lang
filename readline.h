#pragma once

#include <string>
#include <optional>

class ReadLine {
public:
    std::optional<std::string> get(const std::string& prompt);
};
