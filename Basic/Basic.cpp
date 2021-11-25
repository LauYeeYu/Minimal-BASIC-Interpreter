/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <utility>

#include "exp.h"
#include "parser.h"
#include "program.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

using namespace std;

/* Function prototypes */

void processLine(std::string &line, Program &program, EvalState &state);
int calculate(TokenScanner &scanner, EvalState &state);
void scan(TokenScanner &scanner, Program &program, EvalState &state);
void LETStatement(TokenScanner &scanner, EvalState &state);
void PRINTStatement(TokenScanner &scanner, EvalState &state);
void INPUTStatement(TokenScanner &scanner, EvalState &state);
void RUNStatement(TokenScanner &inScanner, Program &program, EvalState &state);
int GOTOStatement(TokenScanner &scanner, Program &program);
int IFStatement(std::string &line, EvalState &state, Program &program, int currentLine);
void LISTStatement(TokenScanner &scanner, Program &program);
void syntaxCheck(std::string line);
bool isDigit (char c);
bool isLetter(char c);
bool isLetterOrDigit(char c);
bool isValidChar(char c);
bool identifierCheck(std::string &identifier);
bool numberCheck(std::string &identifier);
bool check(char op, int lhs, int rhs);
int stringToInt(std::string &s);

/* Main program */

int main() {
    EvalState state;
    Program program;
    while (true) {
        try {
            string input = getLine();
            if (input.empty()) continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cerr << ex.getMessage() << endl;
        }
    }
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */
void processLine(std::string &line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    // For the case of BASIC program
    if (line[0] > 47 && line[0] < 58) {
        int i = 1;
        int number = line[0] - 48;
        for (; i < line.length(); ++i) {
            if (line[i] == ' ') break;
            if (line[i] < 48 || line[i] > 57) error("SYNTAX ERROR");
            number = number * 10 + line[i] - 48;
        }
        if (i == line.length()) {
            program.removeSourceLine(number);
        } else {
            ++i;
            line = line.substr(i);
            syntaxCheck(line);
            program.addSourceLine(number, line);
        }
        return;
    }

    // For the case of directly executed BASIC program
    scan(scanner, program, state);
}

int calculate(TokenScanner &scanner, EvalState &state) {
    Expression *exp = parseExp(scanner);
    return exp->eval(state);
}

bool isDigit (char c) {
    if (c > 47 && c < 58) return true;
    else return false;
}

bool isLetter(char c) {
    if ((c > 64 && c < 91) || (c > 96 && c < 123)) return true;
    else return false;
}

bool isLetterOrDigit(char c) {
    if ((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)) return true;
    else return false;
}

bool isValidChar(char c) {
    if ((c > 46 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123)
        || c == 42 || c == 43 || c == 45 || c == 61) return true;
    else return false;
}

bool identifierCheck(std::string &identifier) {
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

bool numberCheck(std::string &identifier) {
    if (identifier.empty()) return false;
    for (char i : identifier) {
        if (!isDigit(i)) return false;
    }
    return true;
}

bool check(char op, int lhs, int rhs) {
    if (op == '=') return lhs == rhs;
    if (op == '<') return lhs < rhs;
    if (op == '>') return lhs > rhs;
    error("SYNTAX ERROR");
}

void scan(TokenScanner &scanner, Program &program, EvalState &state) {
    std::string stmt = scanner.nextToken();
    if (stmt == "LET") {
        LETStatement(scanner, state);
        return;
    }

    if (stmt == "PRINT") {
        PRINTStatement(scanner, state);
        return;
    }

    if (stmt == "INPUT") {
        INPUTStatement(scanner, state);
        return;
    }

    if (stmt == "REM") {
        error("SYNTAX ERROR");
    }

    if (stmt == "END") {
        error("SYNTAX ERROR");
    }

    if (stmt == "GOTO") {
        error("SYNTAX ERROR");
    }

    if (stmt == "IF") {
        error("SYNTAX ERROR");
    }

    if (stmt == "RUN") {
        RUNStatement(scanner, program, state);
        return;
    }

    if (stmt == "LIST") {
        LISTStatement(scanner, program);
        return;
    }

    if (stmt == "CLEAR") {
        program.clear();
        state.clear();
        return;
    }

    if (stmt == "QUIT") {
        exit(0);
    }

    if (stmt == "HELP") {
        std::cout << "Yet another basic interpreter" << std::endl;
        return;
    }

    error("SYNTAX ERROR");
}

void LETStatement(TokenScanner &scanner, EvalState &state) {
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
}

void PRINTStatement(TokenScanner &scanner, EvalState &state) {
    std::cout << calculate(scanner, state) << std::endl;
}

void INPUTStatement(TokenScanner &scanner, EvalState &state) {
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
}

int GOTOStatement(TokenScanner &scanner, Program &program) {
    std::string token = scanner.nextToken();
    int lineNumber = stringToInt(token);
    if (program.noSuchLine(lineNumber)) {
        error("LINE NUMBER ERROR");
    }
    return lineNumber;
}

int IFStatement(std::string &line, EvalState &state, Program &program, int currentLine) {
    // Find '=', '<', or '>'
    line = line.substr(3);
    int op = 0;
    while (line[op] != '=' && line[op] != '<' && line[op] != '>') {
        ++op;
    }
    std::string lhsString = line.substr(0, op);
    TokenScanner lhsScanner;
    lhsScanner.ignoreWhitespace();
    lhsScanner.scanNumbers();
    lhsScanner.setInput(lhsString);
    int lhs = calculate(lhsScanner, state);

    // Calculate rhs
    int end = op + 1;
    while (line[end] == ' ') ++end;
    while (line[end] != ' ') ++end;

    std::string rhsString = line.substr(op + 1, end - op - 1);
    TokenScanner rhsScanner;
    rhsScanner.ignoreWhitespace();
    rhsScanner.scanNumbers();
    rhsScanner.setInput(rhsString);
    int rhs = calculate(rhsScanner, state);

    // Check
    if (check(line[op], lhs, rhs)) {
        line = line.substr(end + 1);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        if (scanner.nextToken() == "THEN") {
            return GOTOStatement(scanner, program);
        }
        error("SYNTAX ERROR");
    } else {
        return program.getNextLineNumber(currentLine);
    }
}

void LISTStatement(TokenScanner &scanner, Program &program) {
    if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
    int lineNumber = program.getFirstLineNumber();
    while (lineNumber != -1) {
        std::cout << lineNumber << " " << program.getSourceLine(lineNumber)
                  << std::endl;
        lineNumber = program.getNextLineNumber(lineNumber);
    }
}

void syntaxCheck(std::string line) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string stmt = scanner.nextToken();

    if (stmt == "LET") {
        std::string identifier = scanner.nextToken();
        if (!identifierCheck(identifier)) error("SYNTAX ERROR");
        if (scanner.nextToken() != "=") error("SYNTAX ERROR");
        std::string token = scanner.nextToken();
        while (!token.empty()) {
            if (token == "=") error("SYNTAX ERROR");
            token = scanner.nextToken();
        }
        return;
    }

    if (stmt == "PRINT") {
        for (int i = 6; i < line.length(); ++i) {
            if (!isValidChar(line[i])) error("SYNTAX ERROR");
        }
        return;
    }

    if (stmt == "REM") return;

    if (stmt == "INPUT") {
        std::string identifier = scanner.nextToken();
        if (!identifierCheck(identifier)) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return;
    }

    if (stmt == "END") {
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return;
    }

    if (stmt == "GOTO") {
        std::string lineNumber = scanner.nextToken();
        if (!numberCheck(lineNumber)) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return;
    }

    if (stmt == "IF") {
        std::string token = scanner.nextToken();

        // Check first value
        while (!token.empty() && token != "<" && token != ">" && token != "=") {
            for (char i : token) {
                if (!isValidChar(i)) error("SYNTAX ERROR");
            }
            token = scanner.nextToken();
        }

        // Check second value
        if (token.empty()) error("SYNTAX ERROR");
        token = scanner.nextToken();
        while (!token.empty() && token != "THEN") {
            if (token == "<" || token == ">" || token == "=") error("SYNTAX ERROR");
            for (char i : token) {
                if (!isValidChar(i)) error("SYNTAX ERROR");
            }
            token = scanner.nextToken();
        }

        // Check THEN
        if (token.empty()) error("SYNTAX ERROR");

        // Check number
        std::string lineNumber = scanner.nextToken();
        if (!numberCheck(lineNumber)) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return;
    }

    if (stmt == "RUN") {
        error("SYNTAX ERROR");
    }

    if (stmt == "LIST") {
        error("SYNTAX ERROR");
    }

    if (stmt == "CLEAR") {
        error("SYNTAX ERROR");
    }

    if (stmt == "QUIT") {
        error("SYNTAX ERROR");
    }

    if (stmt == "HELP") {
        error("SYNTAX ERROR");
    }

    error("SYNTAX ERROR");
}

/**
 * Implementation of RUN Statement
 * @param inScanner the previous scanner (to check whether it has more tokens
 * @param program the place where program is stored
 * Note that it is impossible to find RUN, LIST, CLEAR, QUIT and HELP in the
 * program (The syntaxCheck function has excluded it.), so these statements
 * won't be enumerated here.
 */

void RUNStatement(TokenScanner &inScanner, Program &program, EvalState &state) {
    if (inScanner.hasMoreTokens()) error("SYNTAX ERROR");
    int lineNumber = program.getFirstLineNumber();
    std::string line;
    while (lineNumber != -1) {
        line = program.getSourceLine(lineNumber);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        std::string stmt = scanner.nextToken();
        if (stmt == "LET") {
            LETStatement(scanner, state);
        } else if (stmt == "PRINT") {
            PRINTStatement(scanner, state);
        } else if (stmt == "INPUT") {
            INPUTStatement(scanner, state);
        } else if (stmt == "END") {
            return;
        } else if (stmt == "GOTO") {
            lineNumber = GOTOStatement(scanner, program);
            continue;
        } else if (stmt == "IF") {
            lineNumber = IFStatement(line, state, program, lineNumber);
            continue;
        }
        lineNumber = program.getNextLineNumber(lineNumber);
    }
}

int stringToInt(std::string &s) {
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
