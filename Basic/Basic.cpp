/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <iostream>
#include <string>

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
 * Scan the New Line
 * @param line the Entered Line
 * @param program the Stored BASIC Program
 * @param state the Place where variables are stored
 *
 * This function parses a line and execute every valid statements.
 * If statement is invalid, it will print an error report.
 */
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

/**
 * Syntax Checking and Create a New Statement Pointer if no Problem in Syntax
 * @param line the Entered Line
 * @return Statement Pointer of its Derived Class
 *
 * This function serves for the full program.  It process a line (without the
 * number head) and check the syntax of a line.  To avoid extra problems like
 * memory leak, syntax error MUST be checked before construct a statement
 * class.
 */
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

    error("SYNTAX ERROR");
    return nullptr;
}
