#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

#include "arithmetic.h"


class BooleanExpression{
public:
	virtual ~BooleanExpression(){};
	virtual std::string format() const = 0;
	virtual bool getValue(std::map<std::string, int> &variables) const = 0;
};

//Useful if more booleans are added in the future
class IntegerCompare : public BooleanExpression {
public:
	IntegerCompare(NumericExpression* left, NumericExpression* right);
	virtual std::string format() const;
	virtual bool getValue(std::map<std::string, int> &variables) const;
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
	bool getValue(std::map<std::string, int> &variables) const;
	~GreaterThan();
};

class LessThan : public IntegerCompare {
public:
	LessThan(NumericExpression* left, NumericExpression* right);
	std::string format() const;
	bool getValue(std::map<std::string, int> &variables) const;
	~LessThan();
};

class EqualTo : public IntegerCompare {
public:
	EqualTo(NumericExpression* left, NumericExpression* right);
	std::string format() const;
	bool getValue(std::map<std::string, int> &variables) const;
	~EqualTo();
};

#endif