/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include <map>
#include "../StanfordCPPLib/map.h"

/**
 * @class EvalState
 *
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * In your implementation, you may include additional information
 * in the EvalState class.
 */
class EvalState {
public:
    EvalState();

    ~EvalState();

    /**
     * Value Setting
     * @param var variable
     * @param value
     *
     * Sets the value associated with the specified var.
     */
    void setValue(const std::string& var, int value);

    /**
     * Value Getter
     * @param var
     * @return The Value of Variable
     */
    int getValue(const std::string& var);

    /**
     * To Tell whether a Variable is Defined
     * @param var
     * @return whether a Variable is Defined
     */
    bool isDefined(const std::string& var);

    /**
     * To Clear the Store Data Map
     */
    void clear();

private:
    std::map<std::string, int> _symbolTable;
};

#endif
