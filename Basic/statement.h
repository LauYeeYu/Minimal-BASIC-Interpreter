/**
 * @file statement.h
 *
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "program.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"

/**
 * @param scanner The Token Scanner
 * @param state Evaluation State to Store the Value of identifiers
 * @return The Answer of a valid statement
 *
 * This function parses an calculating expression (including identifiers)
 * and return the answer.  When there is some error it will throw relevant
 * error and go back.
 */
int calculate(TokenScanner &scanner, EvalState &state);

bool isDigit(char c);

bool isLetter(char c);

bool isLetterOrDigit(char c);

bool isValidChar(char c);

bool identifierCheck(const std::string &identifier);

bool numberCheck(const std::string &identifier);

/**
 *
 * @param op Comparative Operator
 * @param lhs
 * @param rhs
 * @return the Boolean of the Comparing Statement
 */
bool check(char op, int lhs, int rhs);

int stringToInt(std::string s);

class Program;

/**
 * @class Statement
 *
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */
class Statement {
public:
    /**
     * @constructor
     *
     * The base class constructor is empty.  Each subclass must provide
     * its own constructor.
     */
    Statement();

    Statement(std::string line);

    /**
     * @destructor
     *
     * The destructor deallocates the storage for this expression.
     * It must be declared virtual to ensure that the correct subclass
     * destructor is called when deleting a statement.
     */
    virtual ~Statement();

    /**
     * Execute
     * @param state Evaluate State
     * @param program Program Storing Lines of Statements
     *
     * This method executes a BASIC statement.  Each of the subclasses
     * defines its own execute method that implements the necessary
     * operations.  As was true for the expression evaluator, this
     * method takes an EvalState object for looking up variables or
     * controlling the operation of the interpreter.
     */
    virtual void execute(Program &program, EvalState &state) = 0;

    friend std::ostream &operator<<(std::ostream &os, const Statement &stmt);

protected:
    std::string _line;
};

class REM : public Statement {
public:
    REM();

    explicit REM(const std::string &line);

    ~REM() override;

    void execute(Program &program, EvalState &state) override;
};

class LET : public Statement {
public:
    LET();

    explicit LET(const std::string &line);

    ~LET() override;

    void execute(Program &program, EvalState &state) override;
};

class PRINT : public Statement {
public:
    PRINT();

    explicit PRINT(const std::string &line);

    ~PRINT() override;

    void execute(Program &program, EvalState &state) override;
};

class INPUT : public Statement {
public:
    INPUT();

    explicit INPUT(const std::string &line);

    ~INPUT() override;

    void execute(Program &program, EvalState &state) override;
};

class END : public Statement {
public:
    END();

    explicit END(const std::string &line);

    ~END() override;

    void execute(Program &program, EvalState &state) override;
};

class GOTO : public Statement {
public:
    GOTO();

    explicit GOTO(const std::string &line);

    ~GOTO() override;

    void execute(Program &program, EvalState &state) override;
};

class IF : public Statement {
public:
    IF();

    explicit IF(const std::string &line);

    ~IF() override;

    void execute(Program &program, EvalState &state) override;
};

#endif
