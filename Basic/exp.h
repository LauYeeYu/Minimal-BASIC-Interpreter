/**
 * @file exp.h
 *
 * This interface defines a class hierarchy for expressions,
 * which allows the client to represent and manipulate simple
 * binary expression trees.
 */

#ifndef _exp_h
#define _exp_h

#include "evalstate.h"

/**
 * @enum ExpressionType
 *
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */
enum ExpressionType {
    CONSTANT, IDENTIFIER, COMPOUND
};

/**
 * @class Expression
 *
 * This class is used to represent a node in an expression tree.
 * Expression is an example of an abstract class, which defines
 * the structure and behavior of a set of classes but has no
 * objects of its own. Any object must be one of the three
 * concrete subclasses of Expression: <br>
 * <br>
 *  1. ConstantExp   -- an integer constant <br>
 *  2. IdentifierExp -- a string representing an identifier <br>
 *  3. CompoundExp   -- two expressions combined by an operator <br>
 * <br>
 * The Expression class defines the interface common to all
 * Expression objects; each subclass provides its own specific
 * implementation of the common interface.
 * <br> <br>
 * Note on syntax: Each of the virtual methods in the Expression
 * class is marked with the designation = 0 on the prototype line.
 * This notation is used in C++ to indicate that this method is
 * purely virtual and will always be supplied by the subclass.
 */
class Expression {
public:
    Expression();

    /**
     * Destructor of Expression <br>
     * The destructor deallocates the storage for this expression.
     * It must be declared virtual to ensure that the correct subclass
     * destructor is called when deleting an expression.
     */
    virtual ~Expression();

    virtual int eval(EvalState &state) = 0;

    virtual std::string toString() = 0;

    virtual ExpressionType getType() = 0;
};

/**
 * @class: ConstantExp
 *
 * This subclass represents a constant integer expression.
 */
class ConstantExp : public Expression {
public:
    /**
     * the ConstantExp subclass
     * @param value
     *
     * The ConstantExp subclass declares a single instance variable that
     * stores the value of the constant.  The eval method doesn't use the
     * value of state but needs it to match the general prototype for eval.
     */
    ConstantExp(int value);

    /**
     * Prototypes for the virtual methods
     * <br>
     * These methods have the same prototypes as those in the Expression
     * base class and don't require additional documentation.
     */
    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

    int getValue() const;

private:
    int value;
};

/**
 * @class: IdentifierExp
 *
 * This subclass represents an expression corresponding to a variable.
 */
class IdentifierExp : public Expression {
public:
    IdentifierExp(std::string name);

    /**
     * Prototypes for the virtual methods
     * @param state
     *
     * These methods have the same prototypes as those in the Expression
     * base class and don't require additional documentation.
     */
    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

    std::string getName();

private:
    std::string name;
};

/**
 * @class: CompoundExp
 *
 * This subclass represents a compound expression consisting of
 * two subexpressions joined by an operator.
 */
class CompoundExp : public Expression {
public:
    /**
     * Constructor: CompoundExp
     * @param op
     * @param lhs
     * @param rhs
     *
     * The constructor initializes a new compound expression
     * which is composed of the operator (op) and the left and
     * right subexpression (lhs and rhs).
     */
    CompoundExp(std::string op, Expression *lhs, Expression *rhs);

    ~CompoundExp() override;

    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

    std::string getOp() const;

    Expression *getLHS() const;

    Expression *getRHS() const;

private:
    std::string op;
    Expression *lhs, *rhs;
};

#endif
