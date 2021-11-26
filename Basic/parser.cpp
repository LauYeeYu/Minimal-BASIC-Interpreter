/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    }
    return exp;
}

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
    }
    scanner.saveToken(token);
    return exp;
}

Expression *readT(TokenScanner &scanner) {
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));
    if (token != "(") error("SYNTAX ERROR");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("SYNTAX ERROR");
    }
    return exp;
}

int precedence(const std::string &token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}
