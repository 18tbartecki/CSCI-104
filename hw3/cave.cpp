#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>

using namespace std;

int main(int argc, char* argv[])
{
	//Checks to ensure input file given
	if(argc < 2)
	{
		cout << "Please enter an input file." << endl;
		return 1;
	}

	int distance = 0;
	string line;
	//stack used for its LIFO qualities
	stack <char> directions;
	ifstream input(argv[1]);

	//while there are still new directions get new direction
	while(getline(input, line))
	{
		char c;
		stringstream stream(line);
		stream >> c;

		//Nothing in stack but still moving so add direction
		if(directions.empty())
		{
			directions.push(c);
			distance++;
		}
		//If the opposite direction of the new direction is at the top of the stack they can cancel each other out
		else if((c == 'N' && directions.top() == 'S') || (c == 'S' && directions.top() == 'N'))
		{
			directions.pop();
			distance--;
		}
		else if((c == 'E' && directions.top() == 'W') || (c == 'W' && directions.top() == 'E'))
		{
			directions.pop();
			distance--;
		}
		//top is not opposite direction of new direction so we move deeper into the cave
		else
		{
			directions.push(c);
			distance++;
		}
	}
	cout << distance << endl;
	return distance;
}