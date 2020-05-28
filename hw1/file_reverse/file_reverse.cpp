using namespace std;

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

int main(int argc, char* argv[])
{
	
	if(argc < 1)
	{
		cout << "Please input file you'd like to reverse";
	}

	ifstream ifile (argv[1]);
	int characters;

	if(ifile.fail())
	{
		cout << "File could not open";
		return 0;
	}

	ifile >> characters;
	char* character_string = new char[characters];

	string words;
	getline(ifile, words);
	getline(ifile, words);

	strcpy(character_string, words.c_str());

	for(int i = characters-1; i >= 0; i--) 
	{
		cout << character_string[i];
	}

	delete[] character_string;
}