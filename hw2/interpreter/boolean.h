#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include "arithmetic.h"


class BooleanExpression{
public:
	virtual ~BooleanExpression(){};
	virtual std::string format() const = 0;
};

//Useful if more booleans are added in the future
class IntegerCompare : public BooleanExpression {
public:
	IntegerCompare(NumericExpression* left, NumericExpression* right);
	virtual std::string format() const;
	virtual ~IntegerCompare();

//Ensures subclasses can inherit these values
protected:
	NumericExpression* _left;
	NumericExpression* _right;
};

class GreaterThan : public IntegerCompare {
public:
	GreaterThan(NumericExpression* left, NumericExpression* right);
	std::string format() const;
	~GreaterThan();
};

class LessThan : public IntegerCompare {
public:
	LessThan(NumericExpression* left, NumericExpression* right);
	std::string format() const;
	~LessThan();
};

class EqualTo : public IntegerCompare {
public:
	EqualTo(NumericExpression* left, NumericExpression* right);
	std::string format() const;
	~EqualTo();
};

#endif