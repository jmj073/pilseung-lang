#include <regex>
#include <map>
#include <iostream>

#include "parse.h"
#include "tokenize.h"
#include "util.h"

using namespace std;

namespace ps {


    template <typename It>
    int64_t parse_pilseung_literal(It&cur, It end);
    template <typename It>
    Symbol parse_symbol(It& cur, It end);
    template <typename It>
    Expr parse_expr(It& cur, It end);
    template <typename It>
    Array parse_array(It& cur, It end);
    template <typename It>
    While parse_while(It& cur, It end);
    template <typename It>
    Print parse_print(It& cur, It end);
    template <typename It>
    Stmt parse_stmt(It& cur, It end);
    template <typename It>
    Program parse_program(It& cur, It end);

    template <typename It>
    int64_t parse_pilseung_literal(It&cur, It end) {
        // wcout << "parse_pilseung_literal" << endl;
        if (cur == end) throw EOFError(L"<pilseung_literal>");
        if (!is_pilseung_literal_token(*cur)) {
            throw Unexpected(*cur, L"<pilseung_literal>");
        }

        wregex re(LR"([^!](!)*)");
        auto tmp = wregex_view(*cur, re)
            | views::transform([](auto&& m) { return m.str().size() - 1; });
        vector<size_t> ex_mark_cnt(tmp.begin(), tmp.end());

        int64_t ret = 1;

        for (size_t i = 0; i < ex_mark_cnt.size() - 1; ++i) {
            ret *= ex_mark_cnt[i];
        }

        ++cur;
        return ret * (ex_mark_cnt.back() % 2 ? 1 : -1);
    }

    template <typename It>
    Symbol parse_symbol(It& cur, It end) {
        // wcout << "parse_symbol" << endl;
        auto symbol = Symbol();

        if (cur == end) throw EOFError(L"<name>");
        if (!is_normal_token(*cur)) throw Unexpected(*cur);
        symbol.name = *cur;

        if (++cur == end) throw EOFError(L"<rank>");
        if (!is_normal_token(*cur)) throw Unexpected(*cur);
        symbol.rank = str2rank(*cur);

        if (++cur == end) return symbol;
        if (is_callword_token(*cur)) {
            symbol.call_word = true;
            ++cur;
        }

        return symbol;
    }

    template <typename It>
    Expr parse_expr(It& cur, It end) {
        // wcout << "parse_expr" << endl;
        Symbol sym = parse_symbol(cur, end);
        if (cur == end || is_endline_token(*cur)) return sym;

        if (*cur == L"!") {
            ++cur;
            if (cur == end || !is_pilseung_literal_token(*cur)) {
                auto vs = VarSet();
                vs.symbol = sym;
                return vs;
            }

            auto pslit = parse_pilseung_literal(cur, end);

            if (cur == end || !is_normal_token(*cur)) {
                auto vs = VarSet();
                vs.symbol = sym;
                vs.pslit = pslit;
                return vs;
            }

            auto mul = Mul();
            mul.symbol = sym;
            mul.pslit = pslit;
            mul.expr = make_unique<Expr>(parse_expr(cur, end));

            return mul;
        }

        if (!is_pilseung_literal_token(*cur)) {
            return sym;
        }

        auto add = Add();
        add.symbol = sym;
        add.pslit = parse_pilseung_literal(cur, end);

        return add;
    }

    template <typename It>
    Array parse_array(It& cur, It end) {
        // wcout << "parse_array" << endl;
        auto sym = parse_symbol(cur, end);

        if (sym.call_word) {
            throw Unexpected(L"<call_word>");
        }

        auto array = Array();
        array.name = sym.name;
        array.rank = sym.rank;

        size_t sz = rank2size(array.rank);

        --cur;
        while (cur != end && is_normal_token(*cur)) {
            if (++cur == end) throw EOFError(L"!+");
            if (!is_ex_mark_token(*cur)) {
                throw Unexpected(*cur, L"!+");
            }
            sz *= cur->size();
            ++cur;
        }

        array.size = sz;
        return array;
    }

    template <typename It>
    While parse_while(It& cur, It end) {
        auto _while = While();

        _while.symbol = parse_symbol(cur, end);

        if (cur == end) {
            throw EOFError(L"경례!");
        }

        if (is_daehayeo_token(*cur)) {
            _while.gt = true;
            ++cur;
        }

        if (cur == end) {
            throw EOFError(L"경례!");
        }
        if (!is_salute_token(*cur)) {
            throw Unexpected(*cur, L"경례!");
        }

        ++cur;

        while (true) {
            if (cur != end) {
                if (!is_endline_token(*cur)) {
                    throw Unexpected(*cur, L"<endline>");
                }
                ++cur;
            }

            while (cur != end && is_endline_token(*cur)) {
                ++cur;
            }

            if (cur == end) {
                if (stdin_is_tty()) {
                    throw NotYet();
                }
                throw EOFError(L"<stmt> or 바로!");
            }

            if (is_baro_token(*cur)) {
                break;
            }

            _while.stmts.push_back(parse_stmt(cur, end));
        }

        ++cur;

        return _while;
    }

    template <typename It>
    Print parse_print(It& cur, It end) {
        // wcout << "parse_print" << endl;
        auto last = cur;
        while (last != end && !is_singo_token(*last)) {
            ++last;
        }

        if (last == end) {
            throw EOFError(L"singo");
        }

        auto ret = Print();
        ret.expr = make_unique<Expr>(parse_expr(cur, end));
        ret.ascii = (*last == L"신!!고!합니다!");

        ++cur;
        
        return ret;
    }

    template <typename It>
    Stmt parse_stmt(It& cur, It end) {
        // wcout << "parse_stmt" << endl;
        auto last = cur;
        while (last != end && !is_endline_token(*last)) {
            ++last;
        }

        if (is_singo_token(last[-1])) {
            return parse_print(cur, end);
        }
        if (is_salute_token(last[-1])) {
            return parse_while(cur, end);
        }
        
        auto tmp = cur;
        auto sym = parse_symbol(tmp, end);
        if (!sym.call_word
                && tmp != end
                && is_array_ex_mark_token(*tmp)) {
            return parse_array(cur, end);
        }

        return parse_expr(cur, end);
    }

    template <typename It>
    Program parse_program(It& cur, It end) {
        // wcout << "parse_program" << endl;
        Program program;

        while (true) {
            while (cur != end && is_endline_token(*cur)) {
                ++cur;
            }
            if (cur == end) break;

            auto stmt = parse_stmt(cur, end);
            program.stmts.push_back(std::move(stmt));

            if (cur != end && !is_endline_token(*cur)) {
                throw Unexpected(*cur, L"<endline>");
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
