#pragma once

#include <string>
#include <fstream>
#include <optional>

class ReadLine {
public:
    ReadLine();
    std::optional<std::wstring> get(const std::wstring& prompt);

    operator bool() const;

private:
    std::wofstream m_term;
};
