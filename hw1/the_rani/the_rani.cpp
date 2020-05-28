#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <math.h>

using namespace std;

class TheRani {
public:
    TheRani(char* input_path, char* output_path);
    ~TheRani();

    // Call execute and handles exceptions
    void main();

private:
    int experiment_count;       // You will need to track the number of experiments
    int* subject_counts;        // For each, the number of subjects
    string** subject_history;   // And for each subject, their history
    int subject_pool_count;
    int line_counter = 0;       // Tracker for what number line error falls on
    bool experiment_started = false;


    ifstream input;             // Input file stream
    ofstream output;            // Output file stream

    // Called in the main method
    void execute(const string& line);
    void start(int n);
    void add();                             
    void move(int x, int y, int n, int m);  
    void query(int x,int n);

    void destructorhelper();    // Helper to deallocate when multiple START commands called

};

TheRani::TheRani(char* input_path, char* output_path) : experiment_count(0), input(input_path), output(output_path) {

}

TheRani::~TheRani() 
{

}


void TheRani::main() {
    string line;
    while (getline(input, line)) {
        try {
            this->execute(line);
        } catch(exception& e) {
            this->output << "Error on line " + to_string(line_counter) + " : " << e.what() << endl;
        }
    }
    TheRani::destructorhelper();
}

void TheRani::execute(const string& line)
 {
    line_counter++;             // Increment counter after each execute call (new line)
    string command;
    stringstream stream(line);  // Initialize the stream with the line
    stream >> command;          // Read the first word, which is the command

    // Deallocating  and restarting for a second start call
    if(command == "START" && line_counter != 1 && !stream.eof() && experiment_started)
    {
        TheRani::destructorhelper();
        experiment_count = 0;
    }

    // If first line is not START
    if(command != "START" && experiment_started == false)
    {
        throw logic_error("no subjects yet");
    }
    
    if(command == "START")
    { 
        // No arguments following command
        if(stream.eof())           
        {
            throw invalid_argument("too few arguments");
        }
        else
        {
            double double_tester, intpart, fractpart;
            stream >> double_tester;

            // Argument is a string
            if(stream.fail())
            {
                stream.clear();
                throw invalid_argument("expected integer argument");
            }
            // Check if argument is double 
            else
            {
                // Obtain fraction portion of number of subjects
                fractpart = modf(double_tester, &intpart);     
                if(fractpart != 0.0)
                {
                    throw invalid_argument("expected integer argument");
                }
                if(fractpart == 0)
                {
                    // Convert the double to int if an integer and test that it is positive 
                    subject_pool_count = int(double_tester);
                    if (subject_pool_count < 0)   
                    {
                        throw out_of_range("argument out of range");
                    }
                    else
                    {
                        TheRani::start(subject_pool_count);
                    }
                }
            }
        }
    }
    else if(command == "ADD")
    {
        TheRani::add();        
    }
    else if(command == "MOVE")
    {
        // Declaring variables to check exceptions
        int* move_values[4];
        int x,y,n,m;
        move_values[0] = &x;
        move_values[1] = &y;
        move_values[2] = &n;
        move_values[3] = &m;

        double fractpart, intpart, tester;
        
        for(int i = 0; i < 4; i++)
        {
            // Nothing following command/next argument
            if(stream.eof())           
            {
                throw invalid_argument("too few arguments");
            }
            else
            {
                stream >> tester;
                // Fails if string
                if(stream.fail())       
                {
                    stream.clear();
                    throw invalid_argument("expected integer argument");
                }
                else
                {
                    fractpart = modf(tester, &intpart);     // Obtain fraction portion of number of subjects
                    if(fractpart != 0.0)
                    {
                        throw invalid_argument("expected integer argument");
                    }
                    if(fractpart == 0)
                    {
                        // Convert the double to int if an integer 
                        *move_values[i] = int(tester);                    
                    }
                }
            }         
        }

        TheRani::move(x, y, n, m);
     }
     else if(command == "QUERY")
     {
        // Variables for exception checking
        int x, n;
        int* query_values[2];
        query_values[0] = &x;
        query_values[1] = &n;

        double fractpart, intpart, tester;

        for(int i = 0; i < 2; i++)
        {
            // Fails if no next argument
            if(stream.eof())           
            {
                throw invalid_argument("too few arguments");
            }
            else
            {  
                stream >> tester;
                // Fails if string
                if(stream.fail())       
                {
                    stream.clear();
                    throw invalid_argument("expected integer argument");
                }
                else
                {
                    // Obtain fraction portion of number of subjects
                    fractpart = modf(tester, &intpart);     
                    if(fractpart != 0.0)
                    {
                        throw invalid_argument("expected integer argument");
                    }
                    if(fractpart == 0)
                    {
                        // Convert the double to int if an integer  
                        *query_values[i] = int(tester);                    
                    }
                }
            }
        }
        TheRani::query(x, n);
    }
    //If not a real command
    else
    {
        throw logic_error("command does not exist");        
    }
 }


void TheRani::start(int n)
{
    experiment_started = true;

    //Only one experiment at start so all subjects there
    subject_counts = new int[1];            
    subject_counts[0] = subject_pool_count;

    subject_history = new string*[1];

    //Max size of any experiment is subject_pool_count as no new subjects can be added
    subject_history[experiment_count] = new string[subject_pool_count];        

    //Initializing histories with blank strings as subject pool isn't recorded
    for(int i = 0; i < subject_pool_count; i++)
    {
        subject_history[0][i] = "";         
    }
}
            

     
void TheRani::add()
{
     experiment_count++;


     string** temp_hist = subject_history;                  // Ensuring the address of subject_history is not lost
     subject_history = new string*[experiment_count+1];     // New experiment so array grows by one
     
     // Giving each new pointer an array to point to
     for(int i = 0; i <= experiment_count; i++)
     {
        subject_history[i] = new string[subject_pool_count]; 
     }

     // Accessing the old data and copying it into the larger array
     for(int i = 0; i < experiment_count; i++)
     {
        for(int j = 0; j < subject_pool_count; j++)
        {
            subject_history[i][j] = temp_hist[i][j];        
        }
     }

     // Initializing the added experiment with empty strings for subjects
     for(int i = 0; i < subject_pool_count; i++)
     {
        subject_history[experiment_count][i] = "";          
     }

     // Deallocating the old subject_history to prevent a memory leak
     for(int i = 0; i < experiment_count; i++)              
     {
        delete[] temp_hist[i];
     }
     delete[] temp_hist;
     


     int* temp_counts = subject_counts;                     // Saving address of subject_counts
     subject_counts = new int[experiment_count+1];          // Expanding subject_counts to count the subjects in new experiment

     // Copying old numbers of subjects
     for(int i = 0; i < experiment_count; i++)
     {
        subject_counts[i] = temp_counts[i];                 
     }

     subject_counts[experiment_count] = 0;                  // No subjects in new experiment 

     delete[] temp_counts;                                  
     
}

void TheRani::move(int x, int y, int n, int m)
{ 
    if(experiment_count < x || experiment_count < y || x < 0 || y < 0 || n < 0 || m < 0
        || n >= subject_counts[x] || m >= subject_counts[x] || subject_pool_count == 0)
    // Checking for all invalid arguments: negatives, nonexistent experiments/subject counts, at least one subject
    {
         throw out_of_range("argument out of range");       
    }
    //Can't remove subjects backwards
    else if(n > m)
    {
         throw out_of_range("invalid range of subjects to move");  
    }
    else 
    {
        // Subjects moving to different experiment
        if(x != y)  
        {
            // Assigns first empty space in new experiment with each subject's history in old experiment and updates counts
            for(int i = n; i <= m; i++)               
            {
               subject_history[y][subject_counts[y]] = subject_history[x][i];   

               //Write new experiment in history, subject pool doesn't count
               if(y != 0)
               {
                 subject_history[y][subject_counts[y]] = subject_history[y][subject_counts[y]] + to_string(y) + " ";
               }
               subject_counts[x]--;
               subject_counts[y]++;
               subject_history[x][i] = "";
            }

            int incrementer = m;        // Allows value of m to be incremented w/o infinite loop

            // Moves subjects into new position in original experiment
            for(int i = n; i <= m; i++)
            {
                if(incrementer < subject_pool_count - 1)
                {
                    subject_history[x][i] = subject_history[x][incrementer+1];
                    subject_history[x][incrementer+1] = ""; 
                    incrementer++;
                }
            }
        }
        // Subjects staying in same experiment
        else
        {
            int temp_tracker = n;
            int incrementer = m;
            string* temp = new string[subject_pool_count];

            for(int i = 0; i < subject_pool_count; i++)
            {
                temp[i] = "";
            }

            // Copies histories of each subject being re-added to temp array and adds their experiment value again
            for(int i = n; i <= m; i++)
            {
                temp[i] = subject_history[x][i];
                if(y != 0)
                {
                  temp[i] = temp[i] + to_string(y) + " ";
                }
                subject_history[x][i] = "";
                subject_counts[x]--;
            }

            // If only one subject in experiment
            if(subject_counts[x] == 1)
            {
                subject_history[x][0] = temp[0];
            }
            
            incrementer = m;

            // Moving entire subject group
            if(subject_counts[x] == 0)
            {
                 for(int i = n; i <= m; i++)
                 {
                    subject_history[x][i] = temp[i];
                    subject_counts[x]++;
                 }
            }
            // Moving some of subjects in experiment
            else
            {
                // Adjust everyone a position up
                for(int i = n; i < subject_pool_count; i++)
                {
                    if(incrementer + 1 < subject_pool_count)
                    {
                        subject_history[x][i] = subject_history[x][incrementer + 1];
                        subject_history[x][incrementer+1] = "";
                        incrementer++;
                    }
                }
                // Replace empty values at the end with temp values 
                for(int i = temp_tracker; i <= m; i++)
                {
                    subject_history[x][subject_counts[x]] = temp[i];
                    temp_tracker++;
                    subject_counts[x]++;
                }
            }
            delete[] temp;
        }
    }

}

void TheRani::query(int x, int n)
{
        //Checking for any illegitimate inputs such as negative numbers or numbers above what is contained in experiment
        if(x < 0 || n < 0 || x > experiment_count || n > subject_counts[x]-1 || subject_pool_count == 0)
        {
            throw out_of_range("argument out of range");        
        }
        else
        {
            output << subject_history[x][n] << endl;  
        }
}

void TheRani::destructorhelper()
{
    if(experiment_started)
    {
        delete[] subject_counts;

        //Deallocating all experiments and subject's data
         for(int i = 0; i <= experiment_count; i++)
         {
           delete[] subject_history[i];
        }
        delete[] subject_history;
    }

    experiment_started = false;
}




int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Please provide an input and output file!" << endl;   
        return 1;
    }

    TheRani tr(argv[1], argv[2]);
    tr.main();
    return 0;
}