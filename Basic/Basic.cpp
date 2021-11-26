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

/* Function prototypes */

void processLine(std::string &line, Program &program, EvalState &state);
int calculate(TokenScanner &scanner, EvalState &state);
void scan(std::string &line, Program &program, EvalState &state);
Statement *newStatement(const std::string &line);
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
            std::cout << ex.getMessage() << std::endl;
        }
    }
}

/**
 * @param line The Input Line
 * @param program The place where program is stored
 * @param state Evaluation State to Store the Value of identifiers
 *
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
            Statement *stmt = newStatement(line);
            program.addSourceLine(number, stmt);
        }
        return;
    }

    // For the case of directly executed BASIC program
    scan(line, program, state);
}

/**
 * @param scanner The Token Scanner
 * @param state Evaluation State to Store the Value of identifiers
 * @return The Answer of a valid statement
 *
 * This function parses an calculating expression (including identifiers)
 * and return the answer.  When there is some error it will throw relevant
 * error and go back.
 */
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

void scan(std::string &line, Program &program, EvalState &state) {
    Statement *newStmt = nullptr;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string stmt = scanner.nextToken();
    if (stmt == "LET") {
        newStmt = new LET(line);
    } else if (stmt == "PRINT") {
        newStmt = new PRINT(line);
    } else if (stmt == "INPUT") {
        newStmt = new INPUT(line);
    } else if (stmt == "REM" ||stmt == "END"
            || stmt == "GOTO" || stmt == "IF") {
        error("SYNTAX ERROR");
    } else if (stmt == "RUN") {
        program.run(state);
        return;
    } else if (stmt == "LIST") {
        program.list();
        return;
    } else if (stmt == "CLEAR") {
        program.clear();
        state.clear();
        return;
    } else if (stmt == "QUIT") {
        exit(0);
    } else if (stmt == "HELP") {
        std::cout << "Yet another basic interpreter" << std::endl;
        return;
    } else {
        error("SYNTAX ERROR");
    }
    newStmt->execute(program, state);
    delete newStmt;
}

Statement *newStatement(const std::string &line) {
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
        return new LET(line);
    }

    if (stmt == "PRINT") {
        for (int i = 6; i < line.length(); ++i) {
            if (!isValidChar(line[i])) error("SYNTAX ERROR");
        }
        return new PRINT(line);
    }

    if (stmt == "REM") return new REM(line);

    if (stmt == "INPUT") {
        std::string identifier = scanner.nextToken();
        if (!identifierCheck(identifier)) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new INPUT(line);
    }

    if (stmt == "END") {
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new END(line);
    }

    if (stmt == "GOTO") {
        std::string lineNumber = scanner.nextToken();
        if (!numberCheck(lineNumber)) error("SYNTAX ERROR");
        if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
        return new GOTO(line);
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
        return new IF(line);
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
