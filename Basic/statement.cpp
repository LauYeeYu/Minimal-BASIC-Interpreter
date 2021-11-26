/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"

/** Implementation of the Statement class */

Statement::Statement()  = default;

Statement::~Statement() = default;

std::ostream &operator<<(ostream &os, const Statement &stmt) {
    os << stmt._line;
    return os;
}

Statement::Statement(string line) : _line(std::move(line)) {}

/** REM */
REM::REM() = default;

REM::REM(const string &line) : Statement(line) {}

REM::~REM() = default;

void REM::execute(Program &program, EvalState &state) {
    program.nextLine();
}

/** LET */
LET::LET() = default;

LET::LET(const std::string &line) : Statement(line) {}

LET::~LET() = default;

void LET::execute(Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(_line);
    scanner.nextToken();

    // Cannot just use calculate(scanner, state) because it cannot tell "LET x"
    // is a SYNTAX ERROR.
    std::string identifier = scanner.nextToken();
    if (!identifierCheck(identifier)) error("SYNTAX ERROR");
    if (scanner.nextToken() == "=") {
        int value = calculate(scanner, state);
        state.setValue(identifier, value);
    } else {
        error("SYNTAX ERROR");
    }
    program.nextLine();
}

/** PRINT */
PRINT::PRINT() = default;

PRINT::PRINT(const std::string &line) : Statement(line) {}

PRINT::~PRINT() = default;

void PRINT::execute(Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(_line);
    scanner.nextToken();
    std::cout << calculate(scanner, state) << std::endl;
    program.nextLine();
}

/** INPUT */
INPUT::INPUT() = default;

INPUT::INPUT(const std::string &line) : Statement(line) {}

INPUT::~INPUT() = default;

void INPUT::execute(Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(_line);
    scanner.nextToken();

    std::string identifier = scanner.nextToken();
    if (!identifierCheck(identifier)) error("SYNTAX ERROR");
    if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
    std::cout << " ? ";

    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            getLine(std::string());
            std::cout << "INVALID NUMBER" << std::endl << " ? ";
            continue;
        }
        while (std::cin.peek() == ' ') std::cin.get();
        if (std::cin.get() != '\n') {
            getLine(std::string());
            std::cout << "INVALID NUMBER" << std::endl << " ? ";
            continue;
        }
        break;
    }
    state.setValue(identifier, value);
    program.nextLine();
}

/** END */
END::END() = default;

END::END(const string &line) : Statement(line) {}

END::~END() = default;

void END::execute(Program &program, EvalState &state) {
    program.end();
}

/** GOTO */
GOTO::GOTO() = default;

GOTO::GOTO(const std::string &line) : Statement(line) {}

GOTO::~GOTO() = default;

void GOTO::execute(Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(_line);
    scanner.nextToken();

    std::string token = scanner.nextToken();
    int lineNumber = stringToInt(token);
    if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
    program.goTo(lineNumber);
}

/** IF */
IF::IF() = default;

IF::IF(const std::string &line) : Statement(line) {}

IF::~IF() = default;

void IF::execute(Program &program, EvalState &state) {
    // Find '=', '<', or '>'
    std::string tempLine = _line;
    tempLine = tempLine.substr(3);
    int op = 0;
    while (tempLine[op] != '=' && tempLine[op] != '<' && tempLine[op] != '>') {
        ++op;
    }
    std::string lhsString = tempLine.substr(0, op);
    TokenScanner lhsScanner;
    lhsScanner.ignoreWhitespace();
    lhsScanner.scanNumbers();
    lhsScanner.setInput(lhsString);
    int lhs = calculate(lhsScanner, state);

    // Calculate rhs
    int end = op + 1;
    while (tempLine[end] == ' ') ++end;
    while (tempLine[end] != ' ') ++end;

    std::string rhsString = tempLine.substr(op + 1, end - op - 1);
    TokenScanner rhsScanner;
    rhsScanner.ignoreWhitespace();
    rhsScanner.scanNumbers();
    rhsScanner.setInput(rhsString);
    int rhs = calculate(rhsScanner, state);

    // Check
    if (check(tempLine[op], lhs, rhs)) {
        tempLine = tempLine.substr(end + 1);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(tempLine);
        if (scanner.nextToken() == "THEN") {
            std::string token = scanner.nextToken();
            int lineNumber = stringToInt(token);
            if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
            program.goTo(lineNumber);
        } else {
            error("SYNTAX ERROR");
        }
    } else {
        program.nextLine();
    }
}

int calculate(TokenScanner &scanner, EvalState &state) {
    Expression *exp = parseExp(scanner);
    return exp->eval(state);
}

bool isDigit(const char c) {
    if (c > 47 && c < 58) return true;
    else return false;
}

bool isLetter(const char c) {
    if ((c > 64 && c < 91) || (c > 96 && c < 123)) return true;
    else return false;
}

bool isLetterOrDigit(const char c) {
    if ((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)) return true;
    else return false;
}

bool isValidChar(const char c) {
    if ((c > 46 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)
        || c == 42 || c == 43 || c == 45 || c == 61) return true;
    else return false;
}

bool identifierCheck(const std::string &identifier) {
    if (identifier.empty()) return false;
    if (!isLetter(identifier[0])) return false;
    for (int i = 1; i < identifier.length(); ++i) {
        if (!isLetterOrDigit(identifier[i])) return false;
    }
    if (identifier == "REM" || identifier == "LET" || identifier == "PRINT"
        || identifier == "END" || identifier == "RUN" || identifier == "INPUT"
        || identifier == "GOTO" || identifier == "IF" || identifier == "THEN"
        || identifier == "QUIT" || identifier == "LIST" || identifier == "CLEAR"
        || identifier == "HELP") return false;
    return true;
}

bool numberCheck(const std::string &identifier) {
    if (identifier.empty()) return false;
    for (char i : identifier) {
        if (!isDigit(i)) return false;
    }
    return true;
}

bool check(const char op, const int lhs, const int rhs) {
    if (op == '=') return lhs == rhs;
    if (op == '<') return lhs < rhs;
    if (op == '>') return lhs > rhs;
    error("SYNTAX ERROR");
    return false;
}

int stringToInt(std::string s) {
    int number = 0;
    bool isPositive = true;
    if (s[0] == '-') {
        s = s.substr(1);
        isPositive = false;
    }
    for (char i : s) {
        if (i < 48 || i > 57) error("INVALID NUMBER");
        number = number * 10 + i - 48;
    }
    if (!isPositive) number = -number;
    return number;
}
