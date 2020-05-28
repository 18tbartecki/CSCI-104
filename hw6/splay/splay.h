#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include "../bst/bst.h"

/**
* A templated binary search tree implemented as a Splay tree.
*/
template <class Key, class Value>
class SplayTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods.
	SplayTree();
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);
	int report() const;

private:
	/* You'll need this for problem 5. Stores the total number of inserts where the
	   node was added at level strictly worse than 2*log n (n is the number of nodes
	   including the added node. The root is at level 0). */
	int badInserts;
	int totalNodes;
	void splay(const Key& key);
	void seesaw(Node<Key, Value>*& z, Node<Key, Value>*& y, Node<Key, Value>*&x);
	void removingOneChild(Node<Key, Value>*& removing, Node<Key, Value>*& successor);



	/* Helper functions are encouraged. */
};

/*
--------------------------------------------
Begin implementations for the SplayTree class.
--------------------------------------------
*/

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree() : badInserts(0), totalNodes(0) { }

template<typename Key, typename Value>
int SplayTree<Key, Value>::report() const {
	return badInserts;
}

/**
* Insert function for a key value pair. Finds location to insert the node and then splays it to the top.
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	if(BinarySearchTree<Key, Value> ::internalFind(keyValuePair.first) == NULL)
	{
		totalNodes++;
	}
	BinarySearchTree<Key,Value>::insert(keyValuePair);
	splay(keyValuePair.first);
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then splays the parent
* of the deleted node to the top.
*/

template<typename Key, typename Value>
void SplayTree<Key, Value>::removingOneChild(Node<Key, Value>*& removing, Node<Key, Value>*& successor)
{
	if(removing == this->mRoot)
	{
		//Final node, simply set root to NULL
		if(removing->getLeft() == NULL && removing->getRight() == NULL)
		{
			this->mRoot = NULL;
			return;
		}

		//If left side or right side not empty, shift all nodes up one
		if(removing->getLeft() != NULL)
		{
			removing->getLeft()->setParent(NULL);
			this->mRoot = removing->getLeft();
			//delete removing;
			return;
		}
		else
		{
			removing->getRight()->setParent(NULL);
			this->mRoot = removing->getRight();
			//delete removing; 
			return;
		}
	}

	//Removing a left child node
	if(removing->getKey() < removing->getParent()->getKey())
	{
		//Leaf node so just set parent's child to NULL
		if(removing->getLeft() == NULL && removing->getRight() == NULL)
		{
			removing->getParent()->setLeft(NULL);
			//delete removing;
		}
		//Otherwise have parent adopt the remaining nodes
		else if(removing->getLeft() == NULL)
		{
			removing->getParent()->setLeft(removing->getRight());
			removing->getRight()->setParent(removing->getParent());
			//delete removing;
		}
		else
		{
			removing->getParent()->setLeft(removing->getLeft());
			removing->getLeft()->setParent(removing->getParent());
			//delete removing;
		}
		//Update all heights in tree starting from the successor value
	}
	//Removing right child, identical to left child but mirrored
	else
	{
		if(removing->getLeft() == NULL && removing->getRight() == NULL)
		{			
			removing->getParent()->setRight(NULL);
			//delete removing;
		}
		else if(removing->getLeft() == NULL)
		{
			removing->getParent()->setRight(removing->getRight());
			removing->getRight()->setParent(removing->getParent());
			//delete removing;
		}
		else
		{
			removing->getParent()->setRight(removing->getLeft());
			removing->getLeft()->setParent(removing->getParent());
			//delete removing;
		}
		//std::cout << "before update heights successor is: " << successor->getKey() << std::endl;
	}
}


template<typename Key, typename Value>
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* removing = BinarySearchTree<Key, Value>::internalFind(key);
	Node<Key, Value>* successor = BinarySearchTree<Key, Value>::getSuccessor(removing);
	if(removing == NULL)
	{
		return;
	}

	if((removing->getLeft() == NULL || removing->getRight() == NULL))
	{
		removingOneChild(removing, successor); 
		
	}
	//Node has both children
	else
	{
		BinarySearchTree<Key, Value>::remove(removing, successor);
	}

	//If not root then splay
	if(removing->getParent() != NULL)
	{
		splay(removing->getParent()->getKey());
	}

	if(removing != NULL)
	{
		delete removing;
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::splay(const Key& key)
{
	double levels = 0;
	Node<Key, Value>* splaying = BinarySearchTree<Key, Value>::internalFind(key);

	Node<Key, Value>* z = NULL;
	Node<Key, Value>* y = NULL;
	Node<Key, Value>* x = NULL;

	if(this->mRoot != NULL)
	{
		//While not at root
		while(splaying != this->mRoot)
		{
			//Node one away from root so call single rotation
			if(splaying == this->mRoot->getLeft() || splaying == this->mRoot->getRight())
			{
				levels++;
				z = this->mRoot;
				y = splaying;
				this->mRoot = y;
				if(splaying == this->mRoot->getLeft())
				{
					x = splaying->getLeft();
				}
				else
				{
					x = splaying->getRight();
				}
				BinarySearchTree<Key, Value>::zigzig(z, y, x);
			}
			else 
			{
				levels += 2;
				z = splaying->getParent()->getParent();
				y = splaying->getParent();
				x = splaying;
				if(z == this->mRoot)
				{
					this->mRoot = x;
				}

				//Node in zigzag pattern with parent nodes
				if((splaying == y->getRight() && y == z->getLeft()) || (splaying == y->getLeft() && y == z->getRight()))
				{
					BinarySearchTree<Key, Value>::zigzag(z, y, x);
				}
				//Node in straight pattern 
				else
				{
					seesaw(z, y, x);
				}
			}
		}

		if(levels > 2*log2(totalNodes))
		{
			badInserts++;
		}
	}
}

template<typename Key, typename Value>
void SplayTree<Key, Value>::seesaw(Node<Key, Value>*& z, Node<Key, Value>*& y, Node<Key, Value>*&x)
{
	//If right seesaw reset all nodes pointing to z, y, x so x ends up on top
	if(x == y->getRight() && y == z->getRight())
	{
		if(z->getParent() != NULL)
		{
			if(z->getParent()->getKey() > z->getKey())
			{
				z->getParent()->setLeft(x);
			}
			else
			{
				z->getParent()->setRight(x);
			}
		}
		if(y->getLeft() != NULL)
		{
			y->getLeft()->setParent(z);
		}
		if(x->getLeft() != NULL)
		{
			x->getLeft()->setParent(y);
		}
		x->setParent(z->getParent());
		y->setRight(x->getLeft());
		z->setRight(y->getLeft());
		x->setLeft(y);
		y->setParent(x);
		y->setLeft(z);
		z->setParent(y);
	}
	//If left seesaw reset all nodes pointing to z, y, x so x ends up on top and y, x go down to right
	else
	{
		if(z->getParent() != NULL)
		{
			if(z->getParent()->getKey() > z->getKey())
			{
				z->getParent()->setLeft(x);
			}
			else
			{
				z->getParent()->setRight(x);
			}
		}
		if(y->getRight() != NULL)
		{
			y->getRight()->setParent(z);
		}
		if(x->getRight() != NULL)
		{
			x->getRight()->setParent(y);
		}
		x->setParent(z->getParent());
		y->setLeft(x->getRight());
		z->setLeft(y->getRight());
		x->setRight(y);
		y->setParent(x);
		y->setRight(z);
		z->setParent(y);
	}
}



/*
------------------------------------------
End implementations for the SplayTree class.
------------------------------------------
*/

#endif
