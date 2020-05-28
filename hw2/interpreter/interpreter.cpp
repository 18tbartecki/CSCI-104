#include "interpreter.h"

using namespace std;

Interpreter::Interpreter(istream& in) {
    this->parse(in);
}

Interpreter::~Interpreter() {

}


int Interpreter::bignumbers(stringstream& stream)
{
	int line;
	stream >> line;
    return line;
}

void Interpreter::remove_whitespace(stringstream& stream)
{
	//Checks if whitespace then removes it
	while(isspace(stream.peek()))
    {
        stream.get();
    }	        	
}

NumericExpression* Interpreter::parse_constant(stringstream& stream)
{
	string value;
	//Checks if negative and starts by removing dash
	if(stream.peek() == '-')
	{
		stream.get();
		while(isdigit(stream.peek()))
		{
			value += stream.get();
		}
		//Returns Constant object formatted with integer
		return new Constant(-(stoi(value)));
	}
	else
	{
		while(isdigit(stream.peek()))
		{
			value += stream.get();
		}
		return new Constant(stoi(value));
	}
}

string Interpreter::parse_variable_name(stringstream& stream)
{
	string name;
	//Ensures to stop for brackets or other values
	while(isalpha(stream.peek()))
	{
		name += stream.get();
	}
	return name;
}

NumericExpression* Interpreter::parse_numeric_expression(stringstream& stream)
{
	remove_whitespace(stream);
	//Calls parse constant if negative or a digit
	if(stream.peek() == '-' || isdigit(stream.peek()))
	{
	    return parse_constant(stream);
	}
	//Expression is a variable 
	else if(isalpha(stream.peek()))
	{
	    string name = parse_variable_name(stream);
	    remove_whitespace(stream);
	    if(stream.peek() == '[')
	    {
	    	//Removes bracket if there
	    	stream.get();
	    	remove_whitespace(stream);
	    	//Parses the value inside the array bracket after removing any space before
	        NumericExpression* index = parse_numeric_expression(stream);
	        remove_whitespace(stream);
	        //Ensures second bracket found and removed
	        if(stream.peek() == ']')
	        {
	        	stream.get();
	        	remove_whitespace(stream);
	        }
	        return new ArrayVariable(name, index);
	    }
	    //If just a numeric variable
	    else
	    {
	     	return new NumericVariable(name);
	    }
	}
	//Expression is an operation (found a parenthesis)
	else 
	{
		char function;
		//Remove (
		stream.get();
		//All expressions have further expressions on left and right
	 	NumericExpression* left = parse_numeric_expression(stream);
	 	remove_whitespace(stream);
	 	//Takes in operator
	 	function = stream.get();
	 	remove_whitespace(stream);
	 	NumericExpression* right = parse_numeric_expression(stream);
	 	remove_whitespace(stream);
	 	//Checks for other parenthesis and removes
	 	if(stream.peek() == ')')
	 	{
	 		stream.get();
	 		remove_whitespace(stream);
	 	}
	 		//Calls appropriate constructor
	 		if(function == '+')
	 		{
	 			return new AdditionExpression(left, right);
	 		}
	 		else if(function == '-')
	 		{
	 			return new SubtractionExpression(left, right);
	 		}
	 		else if(function == '*')
	 		{
	 			return new MultiplicationExpression(left, right);
	 		}
	 		else
	 		{
	 			return new DivisionExpression(left, right);
	 		}
	}

}

BooleanExpression* Interpreter::parse_boolean_expression(stringstream& stream)
{
	//string to remove the then from the middle of statement
	string then;
	remove_whitespace(stream);
	char comparison;
	NumericExpression* left = parse_numeric_expression(stream);
	remove_whitespace(stream);
	//Takes in logical operator
	comparison = stream.get();
	remove_whitespace(stream);
	NumericExpression* right = parse_numeric_expression(stream);
	remove_whitespace(stream);

	//Takes in "then" and makes sure not to go over
	while(isalpha(stream.peek()))
	{
		then += stream.get();
	}
	remove_whitespace(stream);
	
	//Calls apprpriate constructor
	if(comparison == '>')
	{
		return new GreaterThan(left, right);
	}
	else if(comparison == '<')
	{
		return new LessThan(left, right);
	}
	else
	{
		return new EqualTo(left, right);
	}
}

void Interpreter::parse(istream& in) {
    string line;

    while (getline(in, line)) {
        size_t line_number;
        string command;
        stringstream stream(line);
        stream >> line_number;
        remove_whitespace(stream);

        //Keeps adding to command until one command is found; in case of no space following command
		while(isalpha(stream.peek()))
		{
			command += stream.get();
		
			//Calls appropriate command constructor with correct objects after removing any extra space
			//Adds the fully filled in new command object to the vector for printing out later
	        if(command == "PRINT")
	        {
	        	remove_whitespace(stream);
	        	Print* print_command = new Print(line_number, parse_numeric_expression(stream));
	        	final_lines.push_back(print_command);
	        }
			else if(command == "LET")
			{
				remove_whitespace(stream);
				Let* let_command = new Let(line_number, parse_numeric_expression(stream), parse_numeric_expression(stream));
				final_lines.push_back(let_command);
			}
			else if(command == "GOTO")
			{
				remove_whitespace(stream);
				Goto* goto_command = new Goto(line_number, bignumbers(stream));
				final_lines.push_back(goto_command);
			}
			else if(command == "IF")
			{
				remove_whitespace(stream);
				If* if_command = new If(line_number, parse_boolean_expression(stream), bignumbers(stream));
				final_lines.push_back(if_command);
			}
			else if(command == "GOSUB")
			{
				remove_whitespace(stream);
				Gosub* gosub_command = new Gosub(line_number, bignumbers(stream));
				final_lines.push_back(gosub_command);
			}
			else if(command == "RETURN")
			{
				remove_whitespace(stream);
				Return* return_command = new Return(line_number);
				final_lines.push_back(return_command);
			}
			else if(command == "END")
			{
				remove_whitespace(stream);
				End* end_command = new End(line_number);
				final_lines.push_back(end_command);
			}
		}


    }
}

void Interpreter::write(std::ostream& out) 
{
	for(unsigned int i = 0; i < final_lines.size(); i++)
	{
		//Calls format function of each command object which calls format of all other objects
		out << final_lines[i]->format() << endl;
		delete final_lines[i];
	}
	 
}
