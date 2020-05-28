#include "arithmetic.h"

using namespace std;

Constant::Constant(int value) : _value(value)
{}

BinaryExpression::BinaryExpression(NumericExpression* left, NumericExpression* right) : _left(left), _right(right)//, _value(0)
{
	
}

//Subclasses initialized with their parent class constructors
AdditionExpression::AdditionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right)//, _value(_left->getValue() + _right->getValue()) 
{

}

SubtractionExpression::SubtractionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right)//, _value(_left->getValue() - _right->getValue()) 
{

}

MultiplicationExpression::MultiplicationExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right)//, _value(_left->getValue() * _right->getValue())
{

}

DivisionExpression::DivisionExpression(NumericExpression* left, NumericExpression* right) : BinaryExpression(left, right)//, _value(_left->getValue() / _right->getValue()) 
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

//getValue functions receive map by reference so it can update the variables within

int BinaryExpression::getValue(std::map<std::string, int> &variables) const {return 0;}

//getting value for binary expressions identical except operand and divison by 0
int AdditionExpression::getValue(std::map<std::string, int> &variables) const
{
    return (this->_left->getValue(variables) + this->_right->getValue(variables));
}

int SubtractionExpression::getValue(std::map<std::string, int> &variables) const
{
    return (this->_left->getValue(variables) - this->_right->getValue(variables));
}

int MultiplicationExpression::getValue(std::map<std::string, int> &variables) const
{
    return ((this->_left->getValue(variables))*(this->_right->getValue(variables)));
}

int DivisionExpression::getValue(std::map<std::string, int> &variables) const
{
    if(this->_right->getValue(variables) < 1 && this->_right->getValue(variables) > -1)
    {
        //Throws exception witb the properly formatted statement by using format and getValue functions
        string error =  "Division by 0: " + this->_left->format() + " = " + to_string(this->_left->getValue(variables)) +
        ", " + this->_right->format() + " = " + to_string(this->_right->getValue(variables)) + ".";
        throw logic_error(error);
    }
    return ((this->_left->getValue(variables))/(this->_right->getValue(variables)));
}

int Variable::getValue(std::map<std::string, int> &variables) const {return 0;}

int NumericVariable::getValue(map<string, int> &variables) const
{
    map<string, int>::iterator it;
    it = variables.find(this->_name);
    //Checks if variable exists and returns its value if it does, otherwise just returns 0
    if(it != variables.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

int ArrayVariable::getValue(std::map<std::string, int> &variables) const
{
    //Format function will include [aexpr] in fullname
    string fullname = this->format();
    map<string, int>::iterator it;
    it = variables.find(fullname);
    //Searches the map in the same way as a regular variable
    if(it != variables.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

int Constant::getValue(std::map<std::string, int> &variables) const
{
    return this->_value;
}

