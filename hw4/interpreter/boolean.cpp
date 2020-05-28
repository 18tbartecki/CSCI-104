#include "boolean.h"

using namespace std;

//Parent class that the possible boolean expressions inherit from
IntegerCompare::IntegerCompare(NumericExpression* left, NumericExpression* right) : _left(left), _right(right)
{

}

GreaterThan::GreaterThan(NumericExpression* left, NumericExpression* right) : IntegerCompare(left, right)
{
}

LessThan::LessThan(NumericExpression* left, NumericExpression* right) : IntegerCompare(left, right)
{

}

EqualTo::EqualTo(NumericExpression* left, NumericExpression* right) : IntegerCompare(left, right)
{

}

IntegerCompare::~IntegerCompare()
{}

//Need to deallocate each objects left and right arithmetic expressions
GreaterThan::~GreaterThan() {
    delete this->_left;
    delete this->_right;
}

LessThan::~LessThan() {
    delete this->_left;
    delete this->_right;
}

EqualTo::~EqualTo() {
    delete this->_left;
    delete this->_right;
}

string IntegerCompare::format() const {return "";}

//Format functions identical except for logical operator and the switch for greater than
string GreaterThan::format() const {
    return "[" + this->_right->format() + " < " + this->_left->format() + "]";
}

string LessThan::format() const {
    return "[" + this->_left->format() + " < " + this->_right->format() + "]";
}

string EqualTo::format() const {
    return "[" + this->_left->format() + " = " + this->_right->format() + "]";
}

bool IntegerCompare::getValue(map<string, int> &variables) const {return false;}

//Comapres the values of the left and right numeric expressions and evaluates 
bool GreaterThan::getValue(map<string, int> &variables) const 
{
	return (this->_left->getValue(variables) > this->_right->getValue(variables));
}

bool LessThan::getValue(map<string, int> &variables) const 
{
	return (this->_left->getValue(variables) < this->_right->getValue(variables));
}

bool EqualTo::getValue(map<string, int> &variables) const
{
	return (this->_left->getValue(variables) == this->_right->getValue(variables));
}
