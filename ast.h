#pragma once

#include <vector>
#include <variant>
#include <optional>
#include <memory>
#include <string>

namespace ps {
    enum class Rank {
        R1, R2, R3, R4, OTHER
    };

    struct VarSet;
    struct Add;
    struct Mul;
    struct Symbol;
    using Expr = std::variant<VarSet, Add, Mul, Symbol>;

    struct Symbol {
        std::wstring name;
        Rank rank;
        bool call_word;
    };

    struct VarSet {
        Symbol symbol;
        std::optional<int64_t> pslit;
    };

    struct Add {
        Symbol symbol;
        int64_t pslit;
    };

    struct Mul {
        Symbol symbol;
        int64_t pslit;
        std::unique_ptr<Expr> expr;
    };

    struct Print;
    struct While;
    struct Array;
    using Stmt = std::variant<Expr, Print, While, Array>;

    struct Print {
        std::unique_ptr<Expr> expr;
        bool ascii;
    };

    struct While {
        Symbol symbol;
        bool gt;
        std::vector<Stmt> stmts;
    };

    struct Array {
        std::wstring name;
        Rank rank;
        size_t size;
    };

    struct Program {
        std::vector<Stmt> stmts;
    };

    void print_ast(const Program& ast);
}
