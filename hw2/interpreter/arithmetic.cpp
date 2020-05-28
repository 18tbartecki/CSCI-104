#include "arithmetic.h"

using namespace std;

Constant::Constant(int value) : _value(value)
{}

BinaryExpression::BinaryExpression(NumericExpression* left, NumericExpression* right) : _left(left), _right(right)
{
	
}

//Subclasses initialized with their parent class constructors
AdditionExpression::AdditionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right) 
{

}

SubtractionExpression::SubtractionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right) 
{

}

MultiplicationExpression::MultiplicationExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right) 
{

}

DivisionExpression::DivisionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right) 
{

}

Variable::Variable(string name) : _name(name) {}

NumericVariable::NumericVariable(string name) : Variable(name) {}

ArrayVariable::ArrayVariable(string name, NumericExpression* index) : Variable(name), _index(index) {}


Variable::~Variable()
{}

Constant::~Constant()
{}

BinaryExpression::~BinaryExpression()
{}


//Must deallocate the numeric expression pointers each operation has
AdditionExpression::~AdditionExpression() {
    delete this->_left;
    delete this->_right;
}

SubtractionExpression::~SubtractionExpression() {
    delete this->_left;
    delete this->_right;
}

MultiplicationExpression::~MultiplicationExpression() {
    delete this->_left;
    delete this->_right;
}

DivisionExpression::~DivisionExpression() {
    delete this->_left;
    delete this->_right;
}

NumericVariable::~NumericVariable()
{

}

ArrayVariable::~ArrayVariable()
{
	delete this->_index;
}

string BinaryExpression::format() const {return "";}

//All binary expression nearly identical except for operand
string AdditionExpression::format() const {
    return "(" + this->_left->format() + " + " + this->_right->format() + ")";
}

string SubtractionExpression::format() const {
    return "(" + this->_left->format() + " - " + this->_right->format() + ")";
}

string MultiplicationExpression::format() const {
    return "(" + this->_left->format() + " * " + this->_right->format() + ")";
}

string DivisionExpression::format() const {
    return "(" + this->_left->format() + " / " + this->_right->format() + ")";
}

string Variable::format() const {return "";}

string NumericVariable::format() const {
    return this->_name;
}

//Array must return brackets along with expression inside brackets
string ArrayVariable::format() const {
    return this->_name + "[" + this->_index->format() + "]";
}

string Constant::format() const {
    return to_string(this->_value);
}