/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 */

#ifndef _program_h
#define _program_h

#include <string>
#include "statement.h"
#include "evalstate.h"
#include <map>

/*
 * This class stores the lines in a BASIC program. nEach line
 * in the program is stored in order according to its line number.
 * Moreover, each line in the program is associated with two
 * components:
 *
 * 1. The source line, which is the complete line (including the
 *    line number) that was entered by the user.
 *
 * 2. The parsed representation of that statement, which is a
 *    pointer to a Statement.
 */

class Statement;
class EvalState;

class Program {
public:
    Program();

    /**
     * Frees any heap storage associated with the program.
     */
    ~Program();

    /**
     * Removes all lines from the program.
     */
    void clear();

    /**
     * @param lineNumber
     * @param line A String of the Line
     *
     * Adds a source line to the program with the specified line number.
     * If that line already exists, the text of the line replaces
     * the text of any existing line and the parsed representation
     * (if any) is deleted.  If the line is new, it is added to the
     * program in the correct sequence.
     */
    void addSourceLine(int lineNumber, Statement *stmt);

    /**
     * @param lineNumber
     *
     * Removes the line with the specified number from the program,
     * freeing the memory associated with any parsed representation.
     * If no such line exists, this method simply returns without
     * performing any action.
     */
    void removeSourceLine(int lineNumber);

    /**
     * @param lineNumber
     * @return
     * The program line with the specified line number.
     * If no such line exists, this method returns the empty string.
     */
    Statement *getSourceLine(int lineNumber);

    /**
     * @return the line number of the first line in the program.
     * If the program has no lines, this method returns -1.
     */
    int getFirstLineNumber();

    /**
     * @param lineNumber
     * @return the line number of the first line in the program whose
     * number is larger than the specified one, which must already exist
     * in the program.  If no more lines remain, this method returns -1.
     * Note that the lineNumber MUST be a valid line.
     */
    int getNextLineNumber(int lineNumber);

    /**
     * @param lineNumber
     *
     * Returns the boolean of whether a line is in this program.
     */
    bool noSuchLine(int lineNumber);

    /**
     * Initialize the _currentLine.  If the _program is not empty, then the
     * _currentLine will be set to the first line.  If the _program is empty,
     * then the _currentLine will be set to -1.
     */
    void initCurrentLine();

    /**
     * Set the _currentLine to next line number.  If the current line is not the
     * end, then the _currentLine will be set to the next line.  Otherwise,
     * the _currentLine will be set to -1.
     */
    void nextLine();

    void run(EvalState &state);

    void goTo(int lineNumber);

    void list();

    void end();

private:
    std::map<int, Statement *> _program;
    int _currentLine = -1;
};

#endif
