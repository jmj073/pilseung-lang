#include <iostream>

#include "readline.h"
#include "tokenize.h"
#include "parse.h"

using namespace std;

int main() {
    std::locale::global(std::locale("ko_KR.UTF-8"));
    std::wcin.imbue(std::locale());

    const wstring prompt = L"^^> ";
    const wstring prompt_notyet = L"... ";

    ReadLine read_line;

    while (auto in = read_line.get(prompt)) {
        auto tokens = ps::tokenize(*in + L'\n');

        for (auto& token: tokens) {
            wcout << L"<" << token << L">" << endl;
        }

        try {
            auto ast = ps::parse(tokens);
            print_ast(ast);
            wcout << endl;
        } catch (ps::SyntaxError& e) {
            wcerr << e.wwhat() << endl;
        } catch (ps::NotYet& e) {
            while (auto in = read_line.get(prompt_notyet)) {
                auto new_tokens = ps::tokenize(*in + L'\n');
                tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
                for (auto& token: tokens) {
                    wcout << L"<" << token << L">" << endl;
                }

                try {
                    auto ast = ps::parse(tokens);
                    print_ast(ast);
                    wcout << endl;
                    break;
                } catch (ps::SyntaxError& e) {
                    wcerr << e.wwhat() << endl;
                    break;
                } catch (ps::NotYet& e) {
                }
            }
        }

    }
}
