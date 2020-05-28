#include "../avl/avlbst.h"
#include "../splay/splay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
	clock_t start = 0;
	double duration = 0;

	if(argc < 3)
	{
		cout << "Please enter input and output files" << endl;
		return 0;
	}

	ifstream input(argv[1]);
	ofstream output(argv[2]);

	vector<string> words;
	int insertions = 0;
	int bad_insertions = 0;

	string line;

    while(getline(input, line)) 
    {
        string word;
        stringstream stream(line);
        while(stream >> word)
        {
	        words.push_back(word);
	        insertions++;
	    }
    }



    start = clock();
    int r = 0;

    for(r = 0; r < 500; r++)
    {
	    AVLTree<string, int> avl;

	    for(unsigned int i = 0; i < words.size(); i++)
	    {
	    	avl.insert({words[i], 0});
	    }
	    avl.clear();
	}

    duration = (clock() - start) / (double) CLOCKS_PER_SEC;

    output << insertions << " insertions" << endl;
    output << "AVL: " << duration/r << " seconds" << endl;

    duration = 0;

    start = clock();
    for(r = 0; r < 500; r++)
    {
	    SplayTree<string,int> splay;

	    for(unsigned int i = 0; i < words.size(); i++)
	    {
	    	splay.insert({words[i], 0});
	    }
	    bad_insertions = splay.report();
	    splay.clear();
	}

    duration = (clock() - start) / (double) CLOCKS_PER_SEC;

    output << "Splay: " << duration/r << " seconds" << endl;
    output << "Splay expensive: " << bad_insertions << endl;

}