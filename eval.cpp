#include <fstream>
#include <cassert>
#include <iostream>

#include "eval.h"

using namespace std;

static wifstream console("/dev/tty");

namespace ps {
    Env env;
    Memory mem;

    static void setvar(const Symbol& sym, int64_t num) {
        auto it = env.find(sym.name);
        assert(it != env.end());
        uintptr_t ptr = it->second;
        bool success = false;

        switch (sym.rank) {
            case Rank::R1:
                success = mem.set<int8_t>(ptr, num);
                break;
            case Rank::R2:
                success = mem.set<int16_t>(ptr, num);
                break;
            case Rank::R3:
                success = mem.set<int32_t>(ptr, num);
                break;
            case Rank::R4:
                success = mem.set<int64_t>(ptr, num);
                break;
            case Rank::OTHER:
                success = mem.set<int>(ptr, num);
                break;
        }
        
        assert(success);
    }

    static optional<int64_t> getvar(const Symbol& sym) {
        auto it = env.find(sym.name);
        assert(it != env.end());
        uintptr_t ptr = it->second;

        switch (sym.rank) {
            case Rank::R1:
                return mem.get<int8_t>(ptr);
            case Rank::R2:
                return mem.get<int16_t>(ptr);
            case Rank::R3:
                return mem.get<int32_t>(ptr);
            case Rank::R4:
                return mem.get<int64_t>(ptr);
            case Rank::OTHER:
                return mem.get<int>(ptr);
        }

        assert(0);
    }

    static void eval_stmt(const Stmt& stmt);
    static int64_t eval_expr(const Expr& expr);
    static void eval_while(const While& _while);
    static void eval_print(const Print& print);
    static void eval_array(const Array& array);

    static int64_t eval_varset(const VarSet& varset);
    static int64_t eval_add(const Add& add);
    static int64_t eval_mul(const Mul& mul);
    static int64_t eval_symbol(const Symbol& sym);

    void eval(const Program& prog) {
        for (auto& stmt: prog.stmts) {
            eval_stmt(stmt);
        }
    }

    static void eval_stmt(const Stmt& stmt) {
        visit([](auto&& v) -> void {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, Expr>) eval_expr(v);
            if constexpr (is_same_v<T, Print>) eval_print(v);
            if constexpr (is_same_v<T, While>) eval_while(v);
            if constexpr (is_same_v<T, Array>) eval_array(v);
        }, stmt);
    }

    static int64_t eval_expr(const Expr& expr) {
        return visit([](auto&& v) {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, VarSet>)
                return eval_varset(v);
            if constexpr (is_same_v<T, Add>)
                return eval_add(v);
            if constexpr (is_same_v<T, Mul>)
                return eval_mul(v);
            if constexpr (is_same_v<T, Symbol>)
                return eval_symbol(v);
        }, expr);
    }

    static void eval_while(const While& _while) {
        while (true) {
            auto v = eval_symbol(_while.symbol);
            bool cond =  _while.gt ? v > 0 : v != 0;
            if (!cond) break;

            for (auto& stmt: _while.stmts) {
                eval_stmt(stmt);
            }
        }
    }

    static void eval_print(const Print& print) {
        auto v = eval_expr(*print.expr);
        if (print.ascii) wcout << char(v);
        else wcout << v;
        if (stdin_is_tty() && stdout_is_tty()) {
            wcout << flush;
        }
    }

    static void eval_array(const Array& array) {
        auto ptr = mem.alloc(array.size);
        if (!ptr) {
            throw RuntimeError(L"can't alloc memory");
        }
        env[array.name] = *ptr;
    }

    static int64_t eval_varset(const VarSet& varset) {
        auto& sym = varset.symbol;

        int64_t num;
        if (varset.pslit) {
            num = *varset.pslit;
        } else {
            console >> num;
        }

        if (sym.call_word) {
            auto sz = rank2size(sym.rank);
            auto it = env.find(sym.name);
            if (it == env.end()) {
                auto ptr = mem.alloc(sz);
                if (!ptr) {
                    throw RuntimeError(L"can't alloc memory");
                }
                env[sym.name] = *ptr;
            } else {
                uintptr_t ptr = it->second;
                if (!mem.is_alloced(ptr, sz)) {
                    if (!mem.alloc(sz, ptr)) {
                        throw RuntimeError(L"can't alloc memory");
                    }
                }
            }

            setvar(sym, num);
        } else {
            env[sym.name] = num;
        }

        return num;
    }

    static int64_t eval_add(const Add& add) {
        auto& sym = add.symbol;

        auto it = env.find(sym.name);
        if (it == env.end()) {
            throw RuntimeError(L"can't find variable '" + sym.name + L"'");
        }

        uintptr_t ptr = it->second;

        if (sym.call_word) {
            auto tmp = getvar(sym);
            if (!tmp) {
                throw RuntimeError(L"uninitialized variable '" + sym.name + L"'");
            }
            int64_t num = *tmp + add.pslit;
            setvar(sym, num);
            return num;
        } 

        return it->second = ptr + add.pslit;
    }

    static int64_t eval_mul(const Mul& mul) {
        int64_t num = mul.pslit * eval_expr(*mul.expr);
        
        Add add{
            .symbol = mul.symbol,
            .pslit = num,
        };

        return eval_add(add);
    }

    static int64_t eval_symbol(const Symbol& sym) {
        auto it = env.find(sym.name);
        
        if (it == env.end()) {
            throw RuntimeError(L"can't find variable '" + sym.name + L"'");
        }

        if (!sym.call_word) {
            return it->second;
        }

        auto v = getvar(sym);

        if (!v) {
            throw RuntimeError(L"uninitialized variable '" + sym.name + L"'");
        }

        return *v;
    }

} // namespace ps
