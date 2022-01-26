#include <vector>
#include <stdexcept>
#include <array>
#include <string_view>
#include <regex>
#include <optional>
#include <utility>
#include <string>

namespace {
enum class Operator : size_t {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE
};

constexpr std::array<size_t, 5> arity{
    /* ADD */      2,
    /* SUBTRACT */ 2,
    /* MULTIPLY */ 2,
    /* DIVIDE */   2,
    /* NEGATE */   1
};

// Quick and dirty arithmetic expression evaluator.
class ExpressionEvaluator {
public:
    void clear() {
        stack.clear();
    }

    double value() const {
        return stack.back();
    }

    void push(double value) {
        stack.push_back(value);
    }

    void push(Operator op) {
        checkArity(op);
        switch (op) {
            case Operator::ADD: {            
                stack[stack.size() - 2] += stack[stack.size() - 1];
                stack.pop_back();
                break;
            }
            case Operator::SUBTRACT: {
                stack[stack.size() - 2] -= stack[stack.size() - 1];
                stack.pop_back();
                break;
            }
            case Operator::MULTIPLY: {
                stack[stack.size() - 2] *= stack[stack.size() - 1];
                stack.pop_back();
                break;
            }
            case Operator::DIVIDE: {
                stack[stack.size() - 2] /= stack[stack.size() - 1];
                stack.pop_back();
                break;
            }
            case Operator::NEGATE: {
                stack[stack.size() - 1] *= -1;
                break;
            }
            default:
                throw std::runtime_error("invalid operator");
        }
    }

private:
    void checkArity(Operator op) const {
        if (stack.size() < arity[static_cast<size_t>(op)]) {
            throw std::runtime_error("invalid nunmber of parameters");
        }
    }

private:
    std::vector<double> stack;
};

class ExpressionParser {
public:
    std::pair<std::string, double> eval(std::string_view input) {
        try {
            clear();
            this->input = input;
            expr();
            if (getLookAhead()) { error(); }
            return std::pair(std::string(), evaluator.value());
        } catch (const std::exception& ex) {
            return std::pair(ex.what(), double());
        }
    }

private:
    void clear() {
        lookAhead = 0;
        evaluator.clear();
    }

    void expr() {
        term();
        for (auto symbol = getLookAhead(); symbol; symbol = getLookAhead()) {
            if (*symbol == '+') {
                match(*symbol);
                term();
                evaluator.push(Operator::ADD);
            } else if (*symbol == '-') {
                match(*symbol);
                term();
                evaluator.push(Operator::SUBTRACT);
            } else {
                break;
            }
        }
    }

    void term() {
        factor();
        for (auto symbol = getLookAhead(); symbol; symbol = getLookAhead()) {
            if (*symbol == '*') {
                match(*symbol);
                factor();
                evaluator.push(Operator::MULTIPLY);
            } else if (*symbol == '/') {
                match(*symbol);
                factor();
                evaluator.push(Operator::DIVIDE);
            } else {
                break;
            }
        }
    }

    void factor() {
        auto symbol = getLookAhead();
        if (!symbol) { error(); }
        if (*symbol == '(') {
            match(*symbol);
            expr();
            match(')');
        } else if (*symbol == '-') {
            match(*symbol);
            checkNoBlank();
            factor();
            evaluator.push(Operator::NEGATE);
        } else {
            evaluator.push(number());
        }
    }

    double number() {
        std::cmatch what;
        const char* begin = &input[lookAhead];
        const char* end = &input[0] + input.length();
        if (!std::regex_search(begin, end, what, numberPattern)
            || what[0].first != &input[lookAhead]) {
            error();
        }
        lookAhead += what[0].length();
        return std::stod(what[0].str());
    }

    std::optional<char> getLookAhead() {
        if (lookAhead == input.length()) { return {}; }
        lookAhead = input.find_first_not_of(' ', lookAhead);
        if (lookAhead == std::string::npos) {
            lookAhead = input.length();
            return {};
        }
        return input[lookAhead];
    }

    void checkNoBlank() {
        if (input.find_first_not_of(' ', lookAhead) != lookAhead) {
            error();
        }
    }

    void match(char ch) {
        if (input[lookAhead] == ch) {
            ++lookAhead;
        } else {
            error();
        }
    }

    void error() {
        throw std::runtime_error("syntax error");
    }

private:
    std::string_view input;
    ExpressionEvaluator evaluator;
    size_t lookAhead = 0;
    static const std::regex numberPattern;
};

const std::regex ExpressionParser::numberPattern = std::regex("-?[0-9]+(?:\\.[0-9]+)?([eE][+-]?[0-9]+)?");
}
