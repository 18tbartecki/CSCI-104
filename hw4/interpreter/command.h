#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "boolean.h"
#include <stack>


//Every command calls a constructor with the appropriate variables 

class Command {
public:
	Command(int line);
    virtual std::string format() const = 0;
    virtual int execute(std::map<std::string, int> &variables, std::stack<int> &jumps) = 0;
    virtual std::string error() const = 0;
    virtual ~Command() {}

//Every command has a line number associated with it but no other universal similarities
protected:
	int _line;

};

class Print : public Command {
public:
	Print(int line, NumericExpression* printed);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	NumericExpression* getPrinted() const;
	
	~Print();

//All command subclasses' data members should only be accessed by them
private:
	NumericExpression* _printed;
};

class Let : public Command {
public:
	Let(int line, NumericExpression* var_name, NumericExpression* copied_value);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~Let();

private:
	NumericExpression* _var_name;
	NumericExpression* _copied_value;
};


class Goto : public Command {
public:
	Goto(int line, int new_line);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~Goto();

private:
	int _new_line;
};

class If : public Command {
public:
	If(int line, BooleanExpression* compare, int new_line);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~If();

private:
	BooleanExpression* _compare;
	int _new_line;
};

class Gosub : public Command {
public:
	Gosub(int line, int new_line);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~Gosub();

private:
	int _new_line;
};

class Return : public Command {
public:
	Return(int line);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~Return();
};

class End : public Command {
public:
	End(int line);
	std::string format() const;
	std::string error() const;
	int execute(std::map<std::string, int> &variables, std::stack<int> &jumps);
	~End();
};


#endif
