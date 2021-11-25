/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include <utility>
#include "program.h"
#include "statement.h"

using namespace std;

Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    _program.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    _program[lineNumber] = std::move(line);
}

void Program::removeSourceLine(int lineNumber) {
    if (_program.count(lineNumber)) _program.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) { // May probably be optimized
    if (_program.count(lineNumber)) return _program[lineNumber];
    else return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // todo : Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
    // todo : Replace this stub with your own code
    return nullptr;
}

int Program::getFirstLineNumber() {
    if (_program.empty()) return -1;
    else return _program.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto Temp = _program.find(lineNumber);
    ++Temp;
    if (Temp == _program.end()) return -1;
    else return Temp->first;
}

bool Program::noSuchLine(int lineNumber)
{
    if (_program.count(lineNumber)) return false;
    else return true;
}
