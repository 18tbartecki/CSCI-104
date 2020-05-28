#include "command.h"

using namespace std;

//All commands initialized with parent class constructor for line number

Command::Command(int line) : _line(line)
{}

Print::Print(int line, NumericExpression* value) : Command(line), _value(value)
{}

Let::Let(int line, NumericExpression* var_name, NumericExpression* copied_value) : Command(line), _var_name(var_name), _copied_value(copied_value)
{}

Goto::Goto(int line, int new_line) : Command(line), _new_line(new_line)
{}

If::If(int line, BooleanExpression* compare, int new_line) : Command(line), _compare(compare), _new_line(new_line)
{}

Gosub::Gosub(int line, int new_line) : Command(line), _new_line(new_line)
{}

Return::Return(int line) : Command(line)
{}

End::End(int line) : Command(line)
{}

Print::~Print() {
    delete this->_value;
}

Let::~Let() {
	delete this->_var_name;
    delete this->_copied_value;
}

Goto::~Goto() {}

If::~If() 
{
    delete this->_compare;
}

Gosub::~Gosub() {}


Return::~Return() {}


End::~End() {}


//All commands start with line number and personal command then call formats of the objects they contain

string Print::format() const {
    return to_string(_line) + " PRINT " + this->_value->format();
}

string Let::format() const {
    return to_string(_line) + " LET " + this->_var_name->format() + " " + this->_copied_value->format();
}

string Goto::format() const {
    return to_string(_line) + " GOTO <" + to_string(this->_new_line) + ">";
}

string If::format() const {
    return to_string(_line) + " IF " + this->_compare->format() + " THEN <" + to_string(this->_new_line) + ">";
}

string Gosub::format() const {
    return to_string(_line) + " GOSUB <" + to_string(this->_new_line) + ">";
}

string Return::format() const {
    return to_string(_line) + " RETURN";
}

string End::format() const {
    return to_string(_line) + " END";
}

