#include <iostream>

#include "readline.h"
#include "reader.h"

using namespace std;

int main() {
    const string prompt = "^^> ";
    ReadLine read_line;

    while (auto in = read_line.get(prompt)) {
        // cout << *in << endl;

        for (auto token: tokenize(*in + '\n')) {
            cout << "<" << token.str() << ">" << endl;
        }
    }
}
