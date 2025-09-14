#include <iostream>

#include "ast.h"

using namespace std;

namespace ps {

    static wstring rank2wstring(Rank rank) {
        switch (rank) {
            case Rank::R1: return L"R1";
            case Rank::R2: return L"R2";
            case Rank::R3: return L"R3";
            case Rank::R4: return L"R4";
            case Rank::OTHER: return L"OTHER";
        }
        return L"<unknown>";
    }

    static void print_symbol(const Symbol& symbol);
    static void print_mul(const Mul& mul);
    static void print_add(const Add& add);
    static void print_varset(const VarSet& varset);
    static void print_expr(const Expr& expr);

    static void print_print(const Print& print);
    static void print_while(const While& _while);
    static void print_array(const Array& array);
    static void print_stmt(const Stmt& stmt);

    static void print_symbol(const Symbol& symbol) {
        wcout << L"Symbol{ ";
        wcout << L"name: " << symbol.name;
        wcout << L", rank: " << rank2wstring(symbol.rank);
        wcout << L", callword: " << boolalpha << symbol.call_word;
        wcout << L" }";
    }

    static void print_mul(const Mul& mul) {
        wcout << L"Mul{ ";
        wcout << L"symbol: "; print_symbol(mul.symbol);
        wcout << L", pslit: " << mul.pslit;
        wcout << L", expr: ";
        if (!mul.expr) wcout << "null";
        else print_expr(*mul.expr);
        wcout << L" }";
    }

    static void print_add(const Add& add) {
        wcout << L"Add{ ";
        wcout << L"symbol: "; print_symbol(add.symbol);
        wcout << L", pslit: " << add.pslit;
        wcout << L" }";
    }

    static void print_varset(const VarSet& varset) {
        wcout << L"VarSet{ ";
        wcout << L"symbol: "; print_symbol(varset.symbol);
        wcout << L", pslit: ";
        if (varset.pslit) wcout << *varset.pslit;
        else wcout << "null";
        wcout << L" }";
    }

    static void print_expr(const Expr& expr) {
        visit([](auto&& v) {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, VarSet>)
                return print_varset(v);
            if constexpr (is_same_v<T, Add>)
                return print_add(v);
            if constexpr (is_same_v<T, Mul>)
                return print_mul(v);
            if constexpr (is_same_v<T, Symbol>)
                return print_symbol(v);
        }, expr);
    }

    static void print_print(const Print& print) {
        wcout << L"Print{ ";
        wcout << L"expr: "; 
        if (!print.expr) wcout << "null";
        else print_expr(*print.expr);
        wcout << L", ascii: " << boolalpha << print.ascii;
        wcout << L" }";
    }

    static void print_while(const While& _while) {
        wcout << L"While{ ";
        wcout << L"symbol: "; print_symbol(_while.symbol);
        wcout << L", gt: " << boolalpha << _while.gt;
        wcout << L", stmts: [";
        for (auto& stmt: _while.stmts) {
            print_stmt(stmt); wcout << L", ";
        }
        wcout << L"] }";
    }

    static void print_array(const Array& array) {
        wcout << L"Array{ ";
        wcout << L"name: " << array.name;
        wcout << L", rank: " << rank2wstring(array.rank);
        wcout << L", size: " << array.size;
        wcout << L" }";
    }

    static void print_stmt(const Stmt& stmt) {
        visit([](auto&& v) {
            using T = decay_t<decltype(v)>;
            if constexpr (is_same_v<T, Expr>)
                return print_expr(v);
            if constexpr (is_same_v<T, Print>)
                return print_print(v);
            if constexpr (is_same_v<T, While>)
                return print_while(v);
            if constexpr (is_same_v<T, Array>)
                return print_array(v);
        }, stmt);
    }

    void print_ast(const Program& program) {
        wcout << L"Program{ stmts: [";
        for (auto& stmt: program.stmts) {
            print_stmt(stmt); wcout << L", ";
        }
        wcout << L"] }";
    }

} // namespace ps
