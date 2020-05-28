#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <map>
#include <iostream>

class NumericExpression {
public:
    virtual ~NumericExpression() {}
    virtual std::string format() const = 0;
    virtual int getValue(std::map<std::string, int> &variables) const = 0;

};

class Constant : public NumericExpression {
public:
	Constant(int value);
	std::string format() const;
	int getValue(std::map<std::string, int> &variables) const;
	~Constant();

private:
	int _value;
};

//While a numeric expression, a variable is also in its own category so it has a pure virtual format
class Variable : public NumericExpression {
public:
	Variable(std::string name);
	virtual ~Variable();
	virtual std::string format() const = 0;
	virtual int getValue(std::map<std::string, int> &variables) const = 0;

//Allows subclasses to inherit name
protected:
	std::string _name;
};

class NumericVariable : public Variable {
public:
	NumericVariable(std::string name);
	~NumericVariable();

    std::string format() const;
    int getValue() const;
    int getValue(std::map<std::string, int> &variables) const;

   private:
   	int _value;

};

class ArrayVariable : public Variable {
public:
	ArrayVariable(std::string name, NumericExpression* index);
	~ArrayVariable();

    std::string format() const;
    int getValue(std::map<std::string, int> &variables) const;

//Should not allow access to any other object types 
private:
	NumericExpression* _index;
	int _value;
};

class BinaryExpression : public NumericExpression {
public:
	BinaryExpression(NumericExpression* left, NumericExpression* right);
	virtual ~BinaryExpression();

	virtual std::string format() const;
	virtual int getValue(std::map<std::string, int> &variables) const;

//All binary expressions contain two more numeric expressions on each side
protected:
	NumericExpression* _left;
	NumericExpression* _right;
};

class AdditionExpression : public BinaryExpression {
public:
    AdditionExpression(NumericExpression* left, NumericExpression* right);
    ~AdditionExpression();

    std::string format() const;
    int getValue(std::map<std::string, int> &variables) const;
};

class SubtractionExpression : public BinaryExpression {
public:
    SubtractionExpression(NumericExpression* left, NumericExpression* right);
    ~SubtractionExpression();

    std::string format() const;
    int getValue(std::map<std::string, int> &variables) const;

};

class MultiplicationExpression : public BinaryExpression {
public:
    MultiplicationExpression(NumericExpression* left, NumericExpression* right);
    ~MultiplicationExpression();

    std::string format() const;
    int getValue(std::map<std::string, int> &variables) const;
};

class DivisionExpression : public BinaryExpression {
public:
    DivisionExpression(NumericExpression* left, NumericExpression* right);
    ~DivisionExpression();

    std::string format() const;
    int getValue(std::map<std::string, int> &variables) const;
};


#endif
