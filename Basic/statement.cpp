/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}
/*
SequentialStmt::SequentialStmt() = default;

SequentialStmt::~SequentialStmt() = default;

SequentialStmt::SequentialStmt(string &s)
{
    _sequentialStmt = _scan(s);
}

void SequentialStmt::modify(string &s)
{
    _sequentialStmt = _scan(s);
}

Sequential SequentialStmt::_scan(string &s)
{
    if (s == "REM") return REM;
    if (s == "LET") return LET;
    if (s == "PRINT") return PRINT;
    if (s == "END") return END;
}
*/
