#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "boolean.h"

//Every command calls a constructor with the appropriate variables 

class Command {
public:
	Command(int line);
    virtual std::string format() const = 0;
    virtual ~Command() {}

//Every command has a line number associated with it but no other universal similarities
protected:
	int _line;

};

class Print : public Command {
public:
	Print(int line, NumericExpression* value);
	std::string format() const;
	~Print();

//All command subclasses' data members should only be accessed by them
private:
	NumericExpression* _value;
};

class Let : public Command {
public:
	Let(int line, NumericExpression* var_name, NumericExpression* copied_value);
	std::string format() const;
	~Let();

private:
	NumericExpression* _var_name;
	NumericExpression* _copied_value;
};


class Goto : public Command {
public:
	Goto(int line, int new_line);
	std::string format() const;
	~Goto();

private:
	int _new_line;
};

class If : public Command {
public:
	If(int line, BooleanExpression* compare, int new_line);
	std::string format() const;
	~If();

private:
	BooleanExpression* _compare;
	int _new_line;
};

class Gosub : public Command {
public:
	Gosub(int line, int new_line);
	std::string format() const;
	~Gosub();

private:
	int _new_line;
};

class Return : public Command {
public:
	Return(int line);
	std::string format() const;
	~Return();
};

class End : public Command {
public:
	End(int line);
	std::string format() const;
	~End();
};


#endif
