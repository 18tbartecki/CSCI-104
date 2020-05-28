#include "company.h"
#include <iostream>

using namespace std;

CompanyTracker::CompanyTracker (int n)
  // initializes the tracker with n students and their 1-person companies
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

CompanyTracker::~CompanyTracker ()
  // deallocates all dynamically allocated memory
{
    for(int i = 0; i < numCompanies; i++)
    {
      //calls split to make sure all parent companies are deallocated before they get lost
      for(int j = 0; j < numCompanies; j++)
      {
          split(i);
      }
      delete companies[i];
    }
    delete[] companies;
}

Company* CompanyTracker::findlargest(int i)
  //finds largest company a student belongs to
{
    Company* temp = companies[i];

    //traverses up the tree of parent companies until at the top
    while(temp->parent != nullptr)
    {
        temp = temp->parent;
    }

    return temp;
}

void CompanyTracker::merge (int i, int j)
  /* Merges the largest companies containing students i and j,
     according to the rules described above.
     That is, it generates a new Company object which will
     become the parent company of the largest companies currently
     containing students i and j.
     If i and j already belong to the same company (or are the same),
     merge doesn't do anything.
     If either i or j are out of range, merge doesn't do anything. */
{
    //if same student, students in same company, or student out of bounds do nothing
    if(inSameCompany(i, j) || i > numCompanies-1 || j > numCompanies-1)
    {
      return;
    }

    //find largest comapnies each student is a part of 
    Company* largest_i = findlargest(i);
    Company* largest_j = findlargest(j);

    //create new company with largest parent companies and set parent pointers to the newly formed company
    Company* merger = new Company(largest_i, largest_j);
    largest_i->parent = merger;
    largest_j->parent = merger;

}

void CompanyTracker::split (int i)
  /* Splits the largest company that student i belongs to,
     according to the rules described above.
     That is, it deletes that Company object, and makes sure that
     the two subcompanies have no parent afterwards.
     If i's largest company is a 1-person company, split doesn't do anything.
     If i is out of range, split doesn't do anything. */
{
    if(i > numCompanies-1)
    {
        return;
    }

    Company* largest = findlargest(i);
    //if in a single person company do nothing
    if(largest->merge1 == nullptr)
    {
        return;
    }
    //set parent pointers in each child company to null and remove the largest company student belonged to
    else
    {
        largest->merge1->parent = nullptr;
        largest->merge2->parent = nullptr;
        delete largest;
    }
}

bool CompanyTracker::inSameCompany (int i, int j)
  /* Returns whether students i and j are currently in the same company.
     Returns true if i==j.
     Returns false if i or j (or both) is out of range. */
{
    if(i > numCompanies-1 || j > numCompanies-1)
    {
        return false;
    }

    Company* largest_i = findlargest(i);
    Company* largest_j = findlargest(j);

    //if same student they are in same company
    if(i == j)
    {
        return true;
    }
    //if largest parent company is the same they are in same company
    else if(largest_i == largest_j)
    {
        return true;
    }
    //neither same student or same parent comapany
    else
    {
      return false;
    }
}
