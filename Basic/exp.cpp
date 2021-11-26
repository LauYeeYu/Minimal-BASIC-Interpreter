/*
 * File: exp.cpp
 * -------------
 * This file implements the Expression class and its subclasses.
 */

#include <string>
#include "evalstate.h"
#include "exp.h"

/*
 * Implementation notes: the Expression class
 * ------------------------------------------
 * The Expression class declares no instance variables and needs no code.
 */

Expression::Expression() = default;

Expression::~Expression() = default;

ConstantExp::ConstantExp(int value) {
    this->value = value;
}

int ConstantExp::eval(EvalState &state) {
    return value;
}

std::string ConstantExp::toString() {
    return integerToString(value);
}

ExpressionType ConstantExp::getType() {
    return CONSTANT;
}

int ConstantExp::getValue() const {
    return value;
}

/*
 * Implementation notes: the IdentifierExp subclass
 * ------------------------------------------------
 * The IdentifierExp subclass declares a single instance variable that
 * stores the name of the variable. The implementation of eval must
 * look this variable up in the evaluation state.
 */

IdentifierExp::IdentifierExp(std::string name) {
    this->name = std::move(name);
}

int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) error("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

std::string IdentifierExp::toString() {
    return name;
}

ExpressionType IdentifierExp::getType() {
    return IDENTIFIER;
}

std::string IdentifierExp::getName() {
    return name;
}

/*
 * Implementation notes: the CompoundExp subclass
 * ----------------------------------------------
 * The CompoundExp subclass declares instance variables for the operator
 * and the left and right subexpressions.  The implementation of eval 
 * evaluates the subexpressions recursively and then applies the operator.
 */

CompoundExp::CompoundExp(std::string op, Expression *lhs, Expression *rhs) {
    this->op = std::move(op);
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

/*
 * Implementation notes: eval
 * --------------------------
 * The eval method for the compound expression case must check for the
 * assignment operator as a special case.  Unlike the arithmetic operators
 * the assignment operator does not evaluate its left operand.
 */

int CompoundExp::eval(EvalState &state) {
    if (op == "=") {
        if (lhs->getType() != IDENTIFIER) {
            error("Illegal variable in assignment");
        }
        int val = rhs->eval(state);
        state.setValue(((IdentifierExp *) lhs)->getName(), val);
        return val;
    }
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right != 0) return left / right;
        else error("DIVIDE BY ZERO");
    }
    error("Illegal operator in expression");
    return 0;
}

std::string CompoundExp::toString() {
    return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
    return COMPOUND;
}

std::string CompoundExp::getOp() const {
    return op;
}

Expression *CompoundExp::getLHS() const {
    return lhs;
}

Expression *CompoundExp::getRHS() const {
    return rhs;
}
