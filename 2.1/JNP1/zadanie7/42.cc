#include <stack>
#include <string>
#include <functional>
#include <exception>
#include <cassert>

#include <map>
#include <memory>

class SyntaxError : public std::exception {};

class OperatorAlreadyDefined : public std::exception {};

class UnknownOperator : public std::exception {};

using Lazy = std::function<int()>;

namespace util {
static std::map<char, Lazy> const LITERALS = {{'0', []() { return 0; }},
                                              {'2', []() { return 2; }},
                                              {'4', []() { return 4; }}
};  // couldn't init inside of class definition (and none of the code
    // below that definition was lost from the original solution)
}

class LazyCalculator {
    private:
        using LazyOperator = std::function<int(Lazy, Lazy)>;

        std::map<char, LazyOperator> operators = {
            {'+', [](Lazy a, Lazy b) { return a() + b(); }},
            {'-', [](Lazy a, Lazy b) { return a() - b(); }},
            {'*', [](Lazy a, Lazy b) { return a() * b(); }},
            {'/', [](Lazy a, Lazy b) { return a() / b(); }}
        };

    public:
        LazyCalculator(){};

        Lazy parse(const std::string& s) const {
            std::stack<Lazy> st;
            for (const char c : s) {
                auto l_it = util::LITERALS.find(c);
                auto o_it = operators.find(c);
                bool l_found = l_it != util::LITERALS.end(),
                    o_found = o_it != operators.end();
                if (l_found == o_found) { throw UnknownOperator(); }
                if (l_found) { st.push(l_it->second); }
                else {  // o_found
                    if (st.size() < 2) throw SyntaxError();
                    Lazy l2 = st.top();
                    st.pop();
                    Lazy l1 = st.top();
                    st.pop();
                    st.push([l1, l2, o_it]() { return o_it->second(l1, l2); });
                }
            }
            if (st.size() != 1) throw SyntaxError();
            return st.top();
        }

        int calculate(const std::string& s) const {
            return parse(s)();
        }

        void define(char c, std::function<int(Lazy, Lazy)> fn) {
            auto l_it = util::LITERALS.find(c);
            auto o_it = operators.find(c);
            bool l_found = l_it != util::LITERALS.end(),
                o_found = o_it != operators.end();
            if (l_found || o_found) throw OperatorAlreadyDefined();
            operators[c] = fn;
        }
};

std::function<void(void)> operator*(int n, std::function<void(void)> fn) {
    return [=]() {
        for (int i = 0; i < n; i++)
            fn();
    };
}

int manytimes(Lazy n, Lazy fn) {
    (n() * fn)();  // Did you notice the type cast?
    return 0;
}

int main() {
    LazyCalculator calculator;

    // The only literals...
    assert(calculator.calculate("0") == 0);
    assert(calculator.calculate("2") == 2);
    assert(calculator.calculate("4") == 4);

    // Built-in operators.
    assert(calculator.calculate("42+") == 6);
    assert(calculator.calculate("24-") == -2);
    assert(calculator.calculate("42*") == 8);
    assert(calculator.calculate("42/") == 2);

    assert(calculator.calculate("42-2-") == 0);
    assert(calculator.calculate("242--") == 0);
    assert(calculator.calculate("22+2-2*2/0-") == 2);

    // The fun.
    calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
    assert(calculator.calculate("42!") == 42);

    std::string buffer;
    calculator.define(',', [](Lazy a, Lazy b) { a(); return b(); });
    calculator.define('P', [&buffer](Lazy, Lazy) { buffer += "pomidor"; return 0; });
    assert(calculator.calculate("42P42P42P42P42P42P42P42P42P42P42P42P42P42P42P4"
                                "2P,,,,42P42P42P42P42P,,,42P,42P,42P42P,,,,42P,"
                                ",,42P,42P,42P,,42P,,,42P,42P42P42P42P42P42P42P"
                                "42P,,,42P,42P,42P,,,,,,,,,,,,") == 0);
    assert(buffer.length() == 42 * std::string("pomidor").length());

    std::string buffer2 = std::move(buffer);
    buffer.clear();
    calculator.define('$', manytimes);
    assert(calculator.calculate("42!42P$") == 0);
    // Notice, how std::move worked.
    assert(buffer.length() == 42 * std::string("pomidor").length());

    calculator.define('?', [](Lazy a, Lazy b) { return a() ? b() : 0; });
    assert(calculator.calculate("042P?") == 0);
    assert(buffer == buffer2);

    assert(calculator.calculate("042!42P$?") == 0);
    assert(buffer == buffer2);

    calculator.define('1', [](Lazy, Lazy) { return 1; });
    assert(calculator.calculate("021") == 1);

    for (auto bad: {"", "42", "4+", "424+"}) {
        try {
            calculator.calculate(bad);
            assert(false);
        }
        catch (SyntaxError) {
        }
    }

    try {
        calculator.define('!', [](Lazy a, Lazy b) { return a()*10 + b(); });
        assert(false);
    }
    catch (OperatorAlreadyDefined) {
    }

    try {
        calculator.define('0', [](Lazy, Lazy) { return 0; });
        assert(false);
    }
    catch (OperatorAlreadyDefined) {
    }

    try {
        calculator.calculate("02&");
        assert(false);
    }
    catch (UnknownOperator) {
    }

    return 0;
}
