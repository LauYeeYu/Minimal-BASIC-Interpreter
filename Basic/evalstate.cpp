/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"

using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
    /* Empty */
}

EvalState::~EvalState() {
    /* Empty */
}

void EvalState::setValue(const string& var, int value) {
    _symbolTable[var] = value;
}

int EvalState::getValue(const string& var) {
    return _symbolTable[var];
}

bool EvalState::isDefined(const string& var) {
    return _symbolTable.count(var);
}

void EvalState::clear()
{
    _symbolTable.clear();
}
