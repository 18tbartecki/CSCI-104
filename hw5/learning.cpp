#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

struct Class
{
	double work;
	double learned;
};

void findBenefits(vector<Class*>& info, vector<Class*>& benefits, double workload, double work, double benefit, unsigned int increment)
{
	//Do not add combo and terminate call
	if(work > workload)
	{
		return;
	}
	else 
	{
		//Add the combo of classes to the benefits vector
		Class* aclass = new Class();
	    aclass->work = work;
	    aclass->learned = benefit;
		benefits.push_back(aclass);
	
		//Include the following class then remove it as you move through the vector containing all classes
		for(unsigned int i = increment; i < info.size(); i++)
		{
			work += info[i]->work; benefit += info[i]->learned;
			findBenefits(info, benefits, workload, work, benefit, i + 1);		
			work -= info[i]->work; benefit -= info[i]->learned;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
        cerr << "Please provide an input file." << endl;
        return 0;
    }

    ifstream input(argv[1]);
    vector<Class*> info;      //Vector of all classes
    int num_classes;
    double workload;
    double maxbenefit = -1;
    vector<Class*> benefits;  //Vector of all possible combinations of benefits < workload
    
  
    input >> num_classes >> workload;

    string line;
    getline(input, line);

    //Initializing the vector containing classes and their values
    for(int i = 0; i < num_classes; i++)
    {   
    	getline(input, line);
        stringstream stream(line);
        string newclass;
        double work, learned;
        stream >> newclass >> work >> learned;

        Class* theclass = new Class();
        theclass->work = work;
        theclass->learned = learned;
        info.push_back(theclass);
    }

    findBenefits(info, benefits, workload, 0, 0, 0);

    //Looping through benefits vector to locate max benefit
    for(unsigned int i = 0; i < benefits.size(); i++)
    {
    	if(benefits[i]->learned > maxbenefit)
    	{
    		maxbenefit = benefits[i]->learned;
    	}
    	delete benefits[i];
    }

    //Deallocating all class objects
    for(unsigned int i = 0; i < info.size(); i++)
    {
    	delete info[i];
    }

    cout << maxbenefit << endl;

	return 0;
}

