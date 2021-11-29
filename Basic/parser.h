/**
 * @file parser.h
 *
 * This file acts as the interface to the parser module.
 */

#ifndef _parser_h
#define _parser_h

#include <string>
#include "exp.h"

#include "../StanfordCPPLib/tokenscanner.h"

/**
 * To Parse an Expression
 * @param scanner
 * @return Expression Pointer
 *
 * This code just reads an expression and then checks for extra tokens.
 */
Expression *parseExp(TokenScanner &scanner);

/**
 * Read expression
 * @param scanner
 * @param prec Priority of operator
 * @return Expression Pointer
 *
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */
Expression *readE(TokenScanner &scanner, int prec = 0);

/**
 * Read Token
 * @param scanner
 * @return Expression Pointer
 *
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */
Expression *readT(TokenScanner &scanner);

/**
 * Precedence
 * @param token
 * @return The Rank of operator
 *
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */
int precedence(const std::string &token);

#endif
