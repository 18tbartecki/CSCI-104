#include "command.h"

using namespace std;

//All commands initialized with parent class constructor for line number

Command::Command(int line) : _line(line)
{}

Print::Print(int line, NumericExpression* printed) : Command(line), _printed(printed)
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
    delete this->_printed;
}

Let::~Let() {
	delete this->_var_name;
    delete this->_copied_value;
}

Goto::~Goto() {
}

If::~If() 
{
    delete this->_compare;
}

Gosub::~Gosub() {
}


Return::~Return() {
}


End::~End() {
}


//All commands start with line number and personal command then call formats of the objects they contain

string Print::format() const {
    return to_string(_line) + " PRINT " + this->_printed->format();
}

string Let::format() const {
    return to_string(_line) + " LET " + this->_var_name->format() + " " + this->_copied_value->format();
}

string Goto::format() const {
    return to_string(_line) + " GOTO <" + to_string(_new_line) + ">";
}

string If::format() const {
    return to_string(_line) + " IF " + this->_compare->format() + " THEN <" + to_string(_new_line) + ">";
}

string Gosub::format() const {
    return to_string(_line) + " GOSUB <" + to_string(_new_line) + ">";
}

string Return::format() const {
    return to_string(_line) + " RETURN";
}

string End::format() const {
    return to_string(_line) + " END";
}

//All execute functions take map and stack by reference so the original can be modified with copying

//Gets the value of the arithmetic expression to be printed
int Print::execute(map<string, int> &variables, stack<int> &jumps)
{
    cout << _printed->getValue(variables) << endl;
    return 0;
}

int Let::execute(map<string, int> &variables, stack<int> &jumps)
{
    map<string, int>::iterator it;
    it = variables.find(_var_name->format());
    //If Variable has already been declared just update its value 
    if(it != variables.end())
    {
        it->second = _copied_value->getValue(variables);
        return 0;
    }
    //Otherwise insert new variable with formatted name (includes [expr] for array) and value 
    variables.insert(pair<string, int> (_var_name->format(), _copied_value->getValue(variables)));
    return 0;
}

int Goto::execute(map<string, int> &variables, stack<int> &jumps)
{
    return this->_new_line;
}

int If::execute(map<string, int> &variables, stack<int> &jumps)
{
    //Check boolean and return the new line if true
    if(_compare->getValue(variables))
    {
        return this->_new_line;
    }
    return 0;
}

int Gosub::execute(map<string, int> &variables, stack<int> &jumps)
{ 
    //Add current line number to remember where to return to
    jumps.push(this->_line);
    return this->_new_line;
}

int Return::execute(map<string, int> &variables, stack<int> &jumps)
{
    //No matching GOSUB for RETURN if stack is empty
    if(jumps.empty())
    {
        string error = "No matching GOSUB for RETURN.";
        throw logic_error(error);
    }
    else
    {
        //Otherwise return the line number before the one we return to
        int return_to = jumps.top();
        return return_to;
    }
    return 0;
}

//Unique return value to terminate program
int End::execute(map<string, int> &variables, stack<int> &jumps)
{
    return -1;
}

//Appropriate error messages 
string If::error() const
{
    return "IF jump ";
}

string Goto::error() const
{
    return "GOTO ";
}

string Gosub::error() const
{
    return "GOSUB ";
}

string Print::error() const
{
    return "";
}

string Let::error() const
{
    return "";
}

string Return::error() const
{
    return "";
}

string End::error() const
{
    return "";
}

