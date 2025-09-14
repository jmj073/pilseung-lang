#pragma once

#include <string>
#include <fstream>
#include <optional>

class ReadLine {
public:
    ReadLine();
    std::optional<std::wstring> get(const std::wstring& prompt);

private:
    std::wofstream m_term;
};
