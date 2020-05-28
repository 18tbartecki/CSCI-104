#include <vector>
#include <exception>
#include <iostream>


template <typename T>
  class MinHeap {
     public:
       MinHeap (int d);
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/

       ~MinHeap ();

       int add (T item, int priority);
         /* adds the item to the heap, with the given priority. 
            multiple identical items can be in the heap simultaneously. 
            Returns the number of times add has been called prior to this
            call (for use with the update function).*/

       const T & peek () const;
         /* returns the element with smallest priority.  
            If two elements have the same priority, use operator< on the 
            T data, and return the one with smaller data.*/

       void remove ();
         /* removes the element with smallest priority, with the same tie-breaker
            as peek. */

       void update (int nth, int priority);
         /* finds the nth item (where nth is 0-based) that has ever been added 
            to the heap (the node that was created on the nth call to add), 
            and updates its priority accordingly. */

       bool isEmpty ();
         /* returns true iff there are no elements on the heap. */


   private:
        struct Node
        {
            T item;
            int priority;
            int position;
        };
        int add_calls = 0;
        int d_ary;
        std::vector<Node*> nodes;
        std::vector<Node*> added_order; //To keep track of order added
        void bubbleUp(int position);
        void trickleDown(int position);
        void swap(int first, int second);
};

template <typename T>
MinHeap<T>::MinHeap(int d)
{
    if(d > 1)
    {
        d_ary = d;
    }
}

template <typename T>
MinHeap<T>::~MinHeap()
{
    while(!isEmpty())
    {
        remove();
    }
}

template <typename T>
int MinHeap<T>::add(T item, int priority)
{
    //Create a new node and add to heap
    Node* node = new Node();
    node->item = item; 
    node->priority = priority;
    nodes.push_back(node);
    added_order.push_back(node);

    bubbleUp((int)nodes.size()-1);

    add_calls++;
    return add_calls-1;
}

template <typename T>
const T& MinHeap<T>::peek() const
{
  //Correct node at position 0 from bubbleUp
  if(!nodes.empty())
  {
    return nodes[0]->item;
  }
  else
  {
      throw std::logic_error("No value in heap");
  }
}

template<typename T>
void MinHeap<T>::remove()
{
  if(!nodes.empty())
  {
    if(nodes.size() == 1)
    {
        Node* temp = nodes[0];
        delete temp;
        nodes.pop_back();
    }
    else
    {
        //Swap the first and last elements and trickle it down
        swap(0, (int)nodes.size()-1);
        Node* temp = nodes[nodes.size()-1];
        delete temp;
        nodes.pop_back();
        trickleDown(0);
    }
  }
}

template <typename T>
void MinHeap<T>::update(int nth, int priority)
{
  if(nth < (int)added_order.size())
  {
      //Pointer to the correct node to update
      Node* locator = added_order[nth];
      if(locator != NULL)
      {
          //If priority gets smaller raise node otherwise lower it
          if(locator->priority > priority)
          {
              locator->priority = priority;
              bubbleUp(locator->position);
          }
          else
          {
              locator->priority = priority;
          }   trickleDown(locator->position);
      }
  }
  else
  {
      throw std::logic_error("Element doesn't exist");
  }
}

template <typename T>
bool MinHeap<T>::isEmpty()
{
    return nodes.empty();
}

template <typename T>
void MinHeap<T>::bubbleUp(int position)
{
    int parent = (position-1)/d_ary;
    nodes[position]->position = position;
    //If less than parent bring it upward
    if(position > 0 && nodes[position]->priority < nodes[parent]->priority)
    {  
        swap(position, parent);
        bubbleUp(parent);  
    }
    //If equal compare T values to determine which should be higher
    else if(position > 0 && nodes[position]->priority == nodes[parent]->priority)
    {
        if(nodes[position]->item < nodes[parent]->item)
        {
            swap(position, parent);
            bubbleUp(parent);
        }
    }
}

template <typename T>
void MinHeap<T>::trickleDown(int position)
{
    //Set to position of first child
    unsigned int smallest_child = d_ary*position + 1;
    nodes[position]->position = position;

    if(smallest_child < nodes.size())
    {
        //Iterate through each child and compare to find the smallest priority and T value
        for(int i = 1; i < d_ary; i++)
        {
            if(smallest_child + i < nodes.size() && nodes[smallest_child]->priority > nodes[smallest_child + i]->priority)
            {
                smallest_child += i;
                i = 0;
            }
            else if(smallest_child + i < nodes.size() && nodes[smallest_child]->priority == nodes[smallest_child + i]->priority)
            {
                if(nodes[smallest_child]->item > nodes[smallest_child + i]->item)
                {
                    smallest_child += i;
                    i = 0;
                }
            }
        }

        //If child smaller than parent swap them and keep trickling down
        if(nodes[position]->priority > nodes[smallest_child]->priority)
        {
            swap(position, smallest_child);
            trickleDown(smallest_child);
        }
        else if(nodes[position]->priority == nodes[smallest_child]->priority)
        {
            if(nodes[position]->item > nodes[smallest_child]->item)
            {
                swap(position, smallest_child);
                trickleDown(smallest_child);
            }
        }
    }
}


template <typename T>
void MinHeap<T>::swap(int first, int second)
{
    Node* temp = nodes[first];
    nodes[first] = nodes[second];
    nodes[second] = temp;
}

