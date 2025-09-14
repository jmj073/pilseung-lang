#include <regex>

#include "parse.h"
#include "tokenize.h"

using namespace std;

namespace ps {
    template <typename It>
    Print parse_print(It& cur, It end) {
        while (cur != end && !is_singo_token(*cur)) {
            ++cur;
        }

        if (cur == end) {
            throw EOFError(L"singo");
        }

        ++cur;

        return Print();
    }

    template <typename It>
    Stmt parse_stmt(It& cur, It end) {
        auto last = cur;
        while (last != end && !is_endline_token(*last)) {
            ++last;
        }

        if (is_singo_token(last[-1])) {
            return parse_print(cur, end);
        }
        cur = last;
        return Array();
    }

    template <typename It>
    Program parse_program(It& cur, It end) {
        Program program;

        while (true) {
            while (cur != end && is_endline_token(*cur)) {
                ++cur;
            }
            if (cur == end) break;

            auto stmt = parse_stmt(cur, end);
            program.stmts.push_back(std::move(stmt));

            if (cur != end && !is_endline_token(*cur)) {
                EOFError(L"endline");
            }
        }

        return program;
    }

    Program parse(std::vector<std::wstring> tokens) {
        auto token_begin = tokens.begin();
        auto token_end = tokens.end();

        return parse_program(token_begin, token_end);
    }
} // namespace ps
