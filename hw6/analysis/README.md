# CSCI 104 Student Repository

- **Name**: Tommy Bartecki
- **USC ID**: 4018782058
- **USC Email**: bartecki@usc.edu

I tested my program on cases from n = 10  to n = 2000 and m from 1 to 10. N affected time but m did not. I used 500 repetitions. 

For n = 10, AVL was taking 5.43e-06 seconds and Splay was taking 6.8e-06 seconds
For n = 100 AVL was taking .000189 seconds and Splay was taking .000129 seconds
For n = 1000 AVL was taking .0128 seconds and Splay was taking .0020
For n = 2000 AVL was taking .0534 seconds and Splay was taking .00447

AVL was taking an average of 1.048e-05 seconds per operation and Splay was taking 1.55e-06 seconds.

Time taken depended on n with larger n taking more time, but m did not affect time. Alphabetical 
order also made a difference as the Splay tree was far more efficient than the AVL tree. I beleive
this is due to the fact that when added, the values will splay to the top, meaning every value will
get added to the root then put the remaining values on the left. This also reuslts in 0 bad inserts 
for Splay since it is always added at the root. When randomized, there is less of a guarantee that
Splay will be faster than AVL.



Unsorted List
Would be slower than either tree as inserting would always take n as opposed to logn 

Sorted list
Same case as an Unsorted list, takes worst case runtime n

Binary search tree, non-rotating variety
Slower as well as the rotations keep the balance and make searching be contained by logn while a nonrotating tree wouldnt necessarily

Hash Table, with universal hash function
Would most likely be faster than inserting into a tree as it should amortized runtime constant