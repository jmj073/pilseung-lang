#include <fstream>

#include "eval.h"

static std::wifstream term("/dev/tty");

namespace ps {
    Env env;
    Memory mem;

    static void eval_stmt(const Stmt& stmt);
    static int64_t eval_expr(const Expr& expr);
    static void eval_while(const While& _while);
    static void eval_print(const Print& print);
    static void eval_array(const Array& array);

    static int64_t eval_varset(const VarSet& varset);

    void eval(const Program& prog) {
        for (auto& stmt: prog.stmts) {
            eval_stmt(stmt);
        }
    }

    static void eval_stmt(const Stmt& stmt) {
        std::visit([](auto&& v) -> void {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, Expr>) eval_expr(v);
            if constexpr (is_same_v<T, Print>) eval_print(v);
            if constexpr (is_same_v<T, While>) eval_while(v);
            if constexpr (is_same_v<T, Array>) eval_array(v);
        }, stmt);
    }

    static int64_t eval_expr(const Expr& expr) {
        return std::visit([](auto&& v) {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, VarSet>)
                return eval_varset(v);
            if constexpr (is_same_v<T, Add>)
                return eval_add(v);
            if constexpr (is_same_v<T, Mul>)
                return eval_mul(v);
            if constexpr (is_same_v<T, Symbol>)
                return eval_symbol(v);
        }, stmt);
    }

    static void eval_while(const While& _while) {
        while (true) {
            auto v = eval_symbol(_while.symbol);
            bool cond =  gt ? v > 0 : v != 0;
            if (!cond) break;

            for (auto& stmt: _while.stmts) {
                eval_stmt(stmt);
            }
        }
    }

    static void eval_print(const Print& print) {
        auto v = eval_expr(*print.expr);
        wcout << v << endl;
    }

    static void eval_array(const Array& array) {
        auto ptr = mem.alloc(array.size);
        if (!ptr) {
            throw RuntimeError("can't alloc memory");
        }
        env[array.name] = *ptr;
    }

    static int64_t eval_varset(const VarSet& varset) {
        int64_t num;
        if (varset.pslit) {
            num = *varset.pslit;
        } else {
            term >> num;
        }

        if (varset.symbol.call_word) {
            auto it = env.find(varset.symbol.name);
            uintptr_t ptr;
            if (it == env.end()) {
                auto tmp = mem.alloc(varset.
            }
        } else {
            env[varset.symbol.name] = num;
        }

        return num;
    }

} // namespace ps
