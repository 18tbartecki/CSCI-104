#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include "command.h"
#include <sstream>

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();

    void evaluate();

private:
	std::vector<Command*> final_lines;    //Completely parsed lines of input
    std::map<std::string, int> variables; //Variable names and values associated with them
    std::vector<int> line_numbers;        //Every line number in program
    std::stack<int> jumps;                //Line numbers remembered by GOSUB
    void parse(std::istream& in);

    //Removes space between any values
    void remove_whitespace(std::stringstream& line);
    //Allows for new line numbers over one digit to be put into constructors for commands that need them
    int bignumbers(std::stringstream& line);

    NumericExpression* parse_constant(std::stringstream& line);
    BooleanExpression* parse_boolean_expression(std::stringstream& line);
    NumericExpression* parse_numeric_expression(std::stringstream& line);
    std::string parse_variable_name(std::stringstream& line);
};

#endif
