/**
 * @file evalstate.cpp
 *
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"

/** Implementation of the EvalState class */

EvalState::EvalState() = default;

EvalState::~EvalState() = default;

void EvalState::setValue(const std::string& var, int value) {
    _symbolTable[var] = value;
}

int EvalState::getValue(const std::string& var) {
    return _symbolTable[var];
}

bool EvalState::isDefined(const std::string& var) {
    return _symbolTable.count(var);
}

void EvalState::clear()
{
    _symbolTable.clear();
}
