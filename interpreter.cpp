#include <iostream>
#include <cstdlib>

#include "readline.h"
#include "tokenize.h"
#include "parse.h"
#include "eval.h"

using namespace std;

static const wstring prompt = L"^^> ";
static const wstring prompt_notyet = L"... ";

static optional<ps::Program> READ_LINE(ReadLine& read_line) {
    auto in = read_line.get(prompt);
    if (!in) return nullopt;
    
    auto tokens = ps::tokenize(*in + L'\n');

    try {
        return ps::parse(tokens);
    } catch (ps::SyntaxError& e) {
        wcerr << e.wwhat() << endl;
        return nullopt;
    } catch (ps::NotYet& e) {
        while (auto in = read_line.get(prompt_notyet)) {
            auto new_tokens = ps::tokenize(*in + L'\n');
            tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
            try {
                return ps::parse(tokens);
            } catch (ps::SyntaxError& e) {
                wcerr << e.wwhat() << endl;
                return nullopt;
            } catch (ps::NotYet& e) {
            }
        }
    }

    return nullopt;
}

int main() {
    std::locale::global(std::locale("ko_KR.UTF-8"));
    std::wcin.imbue(std::locale());

    if (stdin_is_tty()) {
        ReadLine read_line;
        while (read_line) {
            auto ast = READ_LINE(read_line);
            if (!ast) {
                if (!read_line) break;
                continue;
            }

            try {
                ps::eval(*ast);
            } catch (ps::RuntimeError& e) {
                wcerr << e.wwhat() << endl;
            }
        }
    } else {
        wstring str((istreambuf_iterator<wchar_t>(wcin)),
                    istreambuf_iterator<wchar_t>());
    
        auto tokens = ps::tokenize(str);
#if 0
        for (auto& token: tokens) {
            wcout << L'<' << token << L'>' << endl;
        }
#endif
        try {
            auto ast = ps::parse(tokens);
            try {
                ps::eval(ast);
            } catch (ps::RuntimeError& e) {
                wcerr << e.wwhat() << endl;
                exit(1);
            }
        } catch (ps::SyntaxError& e) {
            wcerr << e.wwhat() << endl;
            exit(1);
        }
    }

    return 0;
}
