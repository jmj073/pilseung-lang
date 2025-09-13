#include <iostream>

#include "readline.h"
#include "tokenize.h"

using namespace std;

int main() {
    std::locale::global(std::locale("ko_KR.UTF-8"));
    std::wcin.imbue(std::locale());

    const wstring prompt = L"^^> ";
    ReadLine read_line;

    while (auto in = read_line.get(prompt)) {
        auto tokens = tokenize(*in);

        for (auto& token: tokens) {
            wcout << L"<" << token << L">" << endl;
        }
    }
}
