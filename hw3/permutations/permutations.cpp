#include <iostream>
#include <string>

using namespace std;

//swaps characters at indices passed in by pointer
void swap(char* first, char* second)
{
	char temp = *first;
	*first = *second;
	*second = temp;
}

void permutations_helper(std::string in, size_t start, size_t end)
{
	//sufficient number of swaps called (size-1)
	if(start == end)
	{
		cout << in << endl;
	}
	else
	{
		//goes through input string and recusively reorders input string to print permutations
		for(size_t i = start; i <= end; i++)
		{
			//Essentially keeps same input string at start then recursively swaps until all permutations
			//of size-1 are done before swapping the next char for the first and repeating
			swap(in[start], in[i]);
			permutations_helper(in, start+1, end);
		}
	}
}

void permutations(std::string in)
{
	//if empty string just print empty string otherwise recurse
	if(in.size() > 0)
	{
		//size_t used to avoid compiler warnings
		size_t end = in.size();
		//calls helper function with indices of first and last character in string
		permutations_helper(in, 0, end-1);
	}
	else
	{
		cout << "" << endl;
	}
}


