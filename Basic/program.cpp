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

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto &line : _program) {
        delete line.second;
    }
    _program.clear();
}

void Program::addSourceLine(int lineNumber, Statement *stmt) {
    _program[lineNumber] = stmt;
}

void Program::removeSourceLine(int lineNumber) {
    if (_program.count(lineNumber)) {
        delete _program[lineNumber];
        _program.erase(lineNumber);
    }
}

Statement *Program::getSourceLine(int lineNumber) {
    if (_program.count(lineNumber)) return _program[lineNumber];
    else return nullptr;
}

int Program::getFirstLineNumber() {
    if (_program.empty()) return -1;
    else return _program.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    if (_currentLine == -1) return -1;
    auto Temp = _program.find(lineNumber);
    ++Temp;
    if (Temp == _program.end()) return -1;
    else return Temp->first;
}

bool Program::noSuchLine(int lineNumber) {
    if (_program.count(lineNumber)) return false;
    else return true;
}

void Program::initCurrentLine() {
    _currentLine = getFirstLineNumber();
}

void Program::nextLine() {
    _currentLine = getNextLineNumber(_currentLine);
}

void Program::run(EvalState &state) {
    initCurrentLine();
    Statement *currentStmt;
    while (_currentLine != -1) {
        currentStmt = _program[_currentLine];
        currentStmt->execute(*this, state);
    }
}

void Program::goTo(int lineNumber) {
    if (_program.count(lineNumber)) _currentLine = lineNumber;
    else error("LINE NUMBER ERROR");
}

void Program::list() {
    for (auto &line : _program) {
        std::cout << line.first << " " << *(line.second) << std::endl;
    }
}

void Program::end() {
    _currentLine = -1;
}
