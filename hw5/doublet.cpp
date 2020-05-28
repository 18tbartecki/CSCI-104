#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <map>
#include <algorithm>
#include <utility>
#include "heap.h"

using namespace std;

//Calculates the h value for a word
int calculateHeuristic(string word, string target)
{
	int heuristic = 0;
	for(unsigned int i = 0; i < word.size(); i++)
	{
		if(word[i] != target[i])
		{
			heuristic++;
		}
	}
	return heuristic;
}


//Calculates the f value for each word
int Scorer(string word, string end_word, int g)
{
	int h = calculateHeuristic(word, end_word);
	int f = g + h;
	return (f*(word.size()+1)+ h);
}

//Creates all possible words and checks if they are legal words
vector<string> createAllWords(string word, map<string, pair<int, int>>& legalwords, map<string, int>& gmap, MinHeap<string>& heap, string end_word, bool &just_updated, int& expansions)
{
	vector<string> possiblewords;
	for(unsigned int i = word.size(); i > 0; i--)
    {
    	//Sets a temp so word can be back to normal
    	char temp = word[i-1];
    	for(int j = 65; j < 91; j++)
    	{
    		word[i-1] = j;
    		if(word[i-1] != temp)
    		{	
    			map<string, pair<int, int>>::iterator it;
				it = legalwords.find(word);
				//If word is found in list
				if(it != legalwords.end()) 
				{
					//If not visited before add the word to the heap otherwise update
					if(it->second.second == -1)
					{
	    				possiblewords.push_back(word);
	    				it->second.second = Scorer(word, end_word, gmap[heap.peek()]+1);
	    			}
    				else 
    				{
    					//Set g to g of parent + 1 and set all new values
    					int new_g = gmap[heap.peek()];
    					if(gmap[heap.peek()] != gmap[word])
    					{
    						new_g++;
    					}
    					int new_priority = Scorer(word, end_word, new_g);
    					int pos = it->second.first;
    					gmap[word] = new_g;
    					it->second.second = new_priority;
    					//Must remove element before updating otherwise wrong element could be at top of heap
    					if(!just_updated)
    					{
    						legalwords.erase(heap.peek());
	    					heap.remove();
	    					just_updated = true;
	    					expansions++;
	    				}
    					heap.update(pos, new_priority);	
    				}
    			}
    			
    		}
    	}
    	word[i-1] = temp;
    }
    return possiblewords;
}

int main(int argc, char* argv[])
{
	if (argc < 4) 
	{
        cerr << "Please provide an input file." << endl;
        return 0;
    }

    string start_word = argv[1];
    string end_word = argv[2];
    ifstream input(argv[3]);

    //Transform words to uppercase
    transform(start_word.begin(), start_word.end(), start_word.begin(), ::toupper);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::toupper);

    if(start_word.size() != end_word.size())
    {
    	throw logic_error("Words not same size");
    }

    int num_words;
    int add_order = 0;
    //Map of the word to its order added and whether it has been visited
    map<string, pair<int, int>> legalwords;
    //Map of g values
    map<string, int> gmap;
    int expansions = 0;
    bool just_updated = false;

    
    input >> num_words;
    //Transform all words to uppercase and add them to legal words
    for(int i = 0; i < num_words; i++)
    {
    	string new_word;
    	input >> new_word;
    	transform(new_word.begin(), new_word.end(), new_word.begin(), ::toupper);
    	legalwords[new_word] = {i, -1};  //unvisited
    }

    MinHeap<string> heap(2);
    //Add the start word with correct values
    heap.add(start_word, calculateHeuristic(start_word, end_word));
    legalwords[start_word] = {add_order, Scorer(start_word, end_word, 0)};
    add_order++;
    gmap[start_word] = 0;

    while(!heap.isEmpty())
    {
    	int g = gmap[heap.peek()]+1;

    	//If path found end the loop
    	if(calculateHeuristic(heap.peek(), end_word) == 0)
    	{
    		break;
    	}

    	//Create a vector of all newly added words one letter different
    	cout << heap.peek() << endl;
    	vector<string> newwords = createAllWords(heap.peek(), legalwords, gmap, heap, end_word, just_updated, expansions);
    	
    	//All g values one more than parent
    	for(unsigned int i = 0; i < newwords.size(); i++)
    	{
    		gmap[newwords[i]] = g;
    	}

    	//If word not already removed 
    	if(!just_updated)
    	{
    		legalwords.erase(heap.peek());
    		heap.remove();
    		expansions++;
    	}
    	just_updated = false;

    	//For each new word add it to heap and update legal words to be found
    	for(unsigned int i = 0; i < newwords.size(); i++)
    	{
    		int priority = Scorer(newwords[i], end_word, g);
    		heap.add(newwords[i], priority);
    		legalwords[newwords[i]] = {add_order, priority};
    		add_order++;
    	}

    } 

 	//If heap isn't empty then path is found 
    int steps = gmap[end_word];
    if(!heap.isEmpty())
    {
    	cout << steps << endl << expansions << endl;
    }
    else
    {
    	cout << "No transformation" << endl << expansions << endl;
    }
    
}