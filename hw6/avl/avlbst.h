#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <algorithm>
#include "../bst/bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
	AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
	virtual ~AVLNode();

	// Getter/setter for the node's height.
	int getHeight() const;
	void setHeight(int height);

	// Getters for parent, left, and right. These need to be redefined since they 
	// return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
	// for more information.
	virtual AVLNode<Key, Value>* getParent() const override;
	virtual AVLNode<Key, Value>* getLeft() const override;
	virtual AVLNode<Key, Value>* getRight() const override;

protected:
	int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
	: Node<Key, Value>(key, value, parent), mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
	mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);

private:
	int calculateHeight(Node<Key,Value>* tracker);
	void unbalanceSetUp(AVLNode<Key,Value>*& tracker, Node<Key,Value>*& z, Node<Key,Value>*& y, Node<Key,Value>*& x);
	bool checkBalance(AVLNode<Key, Value>*& tracker, bool& unbalanced);
	void updateHeights(AVLNode<Key,Value>* tracker);
	void balance(Node<Key, Value>*& z, Node<Key, Value>*& y, Node<Key, Value>*&x);
	void removingOneChild(Node<Key, Value>*& removing, Node<Key, Value>*& successor, AVLNode<Key, Value>*& updater, bool& unbalanced );
	bool almostEmpty();

};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	//If first node
	if(this->mRoot == NULL)
	{
		this->mRoot = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
	}
	else
	{
		bool unbalanced = false;
		Node<Key, Value>* duplicate = BinarySearchTree<Key,Value>::internalFind(keyValuePair.first);
		//If element already added
		if(duplicate != NULL)
		{
			duplicate->setValue(keyValuePair.second);
		}
		else
		{
			Node<Key, Value>* comparer = this->mRoot;
			Node<Key, Value>* setter = comparer;
			//Find location in which node should be placed
			while(comparer != NULL)
			{
				if(comparer->getKey() > keyValuePair.first)
				{
					setter = comparer;
					comparer = comparer->getLeft();
				}
				else
				{
					setter = comparer;
					comparer = comparer->getRight();
				}
			}

			AVLNode<Key, Value>* newnode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, static_cast<AVLNode<Key,Value>*>(setter));
			//Check which side new node belongs on
			if(setter->getKey() > keyValuePair.first)
			{
				setter->setLeft(newnode);
			}
			else
			{
				setter->setRight(newnode);
			}

			newnode->setHeight(0);
			AVLNode<Key, Value>* tracker = newnode->getParent();
			updateHeights(tracker);

			//Declare a tracker for checking the balance by going up the tree
			tracker = newnode->getParent();
			Node<Key, Value>* z = NULL;
			Node<Key, Value>* y = NULL;
			Node<Key, Value>* x = NULL;
			
			if(checkBalance(tracker,unbalanced))
			{
				unbalanceSetUp(tracker, z, y, x);
			}

			if(x != NULL && y != NULL && z != NULL)
			{
				balance(z, y, x);
			}
			

			unbalanced = false;
		}
	}
}



template<typename Key, typename Value>
int AVLTree<Key, Value>::calculateHeight(Node<Key,Value>* tracker)
{
	if(tracker == NULL)
	{
		return 0;
	}

	return 1 + std::max(calculateHeight(tracker->getLeft()), calculateHeight(tracker->getRight()));
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::updateHeights(AVLNode<Key,Value>* tracker)
{
	//Travel up tree and recalculate all heights 
	while(tracker->getParent() != NULL)
	{
		int rheight = calculateHeight(tracker->getRight());
		int lheight = calculateHeight(tracker->getLeft());
		tracker->setHeight(std::max(lheight, rheight));
		tracker = tracker->getParent();
	}
}

template<typename Key, typename Value>
bool AVLTree<Key, Value>::checkBalance(AVLNode<Key, Value>*& tracker, bool& unbalanced)
{
	while(tracker->getParent() != NULL && !unbalanced)
	{
		//If left child
		if(tracker->getParent()->getKey() > tracker->getKey())
		{
			//Check if height from sister node varies by more than 1 or isnt there
			if(tracker->getParent()->getRight() == NULL || 
				std::abs(tracker->getHeight() - tracker->getParent()->getRight()->getHeight()) > 1)
			{
				unbalanced = true;
				break;
			}

		}
		//Mirrored version of above
		else
		{
			if(tracker->getParent()->getLeft() == NULL || 
				std::abs(tracker->getHeight() - tracker->getParent()->getLeft()->getHeight()) > 1)
			{
				unbalanced = true;
				break;
			}
		}
		tracker = tracker->getParent();
	}

	return unbalanced;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::unbalanceSetUp(AVLNode<Key, Value>*& tracker, Node<Key, Value>*& z, Node<Key, Value>*& y, Node<Key, Value>*&x)
{
	int rheight = 0;
	int lheight = 0;

	//Tracker passed in so unbalance is at it
	z = tracker->getParent();
	y = tracker;

	if(tracker->getRight() != NULL)
	{
		rheight = tracker->getRight()->getHeight();
	}

	if(tracker->getLeft() != NULL)
	{
		lheight = tracker->getLeft()->getHeight();
	}

	//Apprpropriately set the value of x depending on longer branch
	if(rheight > 0 || lheight > 0)
	{
		if(rheight > lheight)
		{
			x = tracker->getRight();
		}
		else if(rheight < lheight)
		{
			x = tracker->getLeft();
		}
		else
		{
			if(y == z->getLeft())
			{
				x = tracker->getLeft();
			}
			else
			{
				x = tracker->getRight();
			}
		}
	}
	else
	{
		//If they are the same height pick zigzig over zigzag
		if(tracker->getRight() != NULL && tracker->getLeft() != NULL)
		{
			if(y == z->getLeft())
			{
				x = tracker->getLeft();
			}
			else
			{
				x = tracker->getRight();
			}	
		}
		else if(tracker->getLeft() != NULL)
		{
			x = tracker->getLeft();
		}
		else
		{
			x = tracker->getRight();
		}
	}
	
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::balance(Node<Key, Value>*& z, Node<Key, Value>*& y, Node<Key, Value>*&x)
{	
	if((x == y->getRight() && y == z->getRight()) || (x == y->getLeft() && y == z->getLeft()))
	{
		//Call appropriate formation and update heights and root if necessary when finished
		BinarySearchTree<Key, Value>::zigzig(z, y, x);
		if(z == this->mRoot)
		{
			this->mRoot = y;
		}

		updateHeights(static_cast<AVLNode<Key,Value>*>(x));
		updateHeights(static_cast<AVLNode<Key,Value>*>(z));
					
	}
	else
	{
		BinarySearchTree<Key, Value>::zigzag(z, y, x);
		if(z == this->mRoot)
		{
			this->mRoot = x;
		}

		updateHeights(static_cast<AVLNode<Key,Value>*>(y));
		updateHeights(static_cast<AVLNode<Key,Value>*>(z));			
	}
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::removingOneChild(Node<Key, Value>*& removing, Node<Key, Value>*& successor, AVLNode<Key, Value>*& updater, bool& unbalanced )
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
			return;
		}
		else
		{
			removing->getRight()->setParent(NULL);
			this->mRoot = removing->getRight();
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
		}
		//Otherwise have parent adopt the remaining nodes
		else if(removing->getLeft() == NULL)
		{
			removing->getParent()->setLeft(removing->getRight());
			removing->getRight()->setParent(removing->getParent());
		}
		else
		{
			removing->getParent()->setLeft(removing->getLeft());
			removing->getLeft()->setParent(removing->getParent());
		}
		//Update all heights in tree starting from the successor value
		updateHeights(updater);
	}
	//Removing right child, identical to left child but mirrored
	else
	{
		if(removing->getLeft() == NULL && removing->getRight() == NULL)
		{
			//Largest element removed so set successor to previous element to update heights
			if(successor == NULL)
			{
				updater = static_cast<AVLNode<Key,Value>*>(removing->getParent());
			}
			removing->getParent()->setRight(NULL);
		}
		else if(removing->getLeft() == NULL)
		{
			removing->getParent()->setRight(removing->getRight());
			removing->getRight()->setParent(removing->getParent());
		}
		else
		{
			//Largest element removed so update successor to update heights
			if(successor == NULL)
			{
				updater = static_cast<AVLNode<Key,Value>*>(removing->getLeft());
			}
			removing->getParent()->setRight(removing->getLeft());
			removing->getLeft()->setParent(removing->getParent());
		}
		updateHeights(updater);
	}
}

//Checks if only 1 or 2 nodes left
template<typename Key, typename Value>
bool AVLTree<Key, Value>::almostEmpty()
{
	int nodes = 1;

	if(this->mRoot->getLeft() != NULL)
	{
		nodes++;
		if(this->mRoot->getLeft()->getLeft() != NULL)
		{
			nodes++;
		}
		if(this->mRoot->getLeft()->getRight() != NULL)
		{
			nodes++;
		}
	}
	if(this->mRoot->getRight() != NULL)
	{
		nodes++;
		if(this->mRoot->getRight()->getLeft() != NULL)
		{
			nodes++;
		}
		if(this->mRoot->getRight()->getRight() != NULL)
		{
			nodes++;
		}
	}

	if(nodes <= 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}


template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	bool unbalanced = false;
	Node<Key, Value>* removing = BinarySearchTree<Key, Value>::internalFind(key);
	if(removing == NULL)
	{
		return;
	}
	Node<Key, Value>* successor = BinarySearchTree<Key, Value>::getSuccessor(removing);
	AVLNode<Key, Value>* updater = static_cast<AVLNode<Key,Value>*>(successor);

	if(removing->getParent() != NULL && removing->getParent() != this->mRoot)
	{
		//If special case of successor node to update heights later
		if(successor == removing->getParent()->getParent())
		{
			updater = static_cast<AVLNode<Key,Value>*>(successor)->getLeft();
		}
	}

	//Removing when one child only
	if((removing->getLeft() == NULL || removing->getRight() == NULL))
	{
		removingOneChild(removing, successor, updater, unbalanced); 
		if(removing != NULL)
		{
			delete removing;
		}

		//Tree empty
		if(this->mRoot == NULL)
		{
			return;
		}	
	}
	//Node has both children
	else
	{
		BinarySearchTree<Key, Value>::remove(removing, successor);

		if(removing != NULL)
		{
			delete removing;
		}

		if(updater == this->mRoot && updater->getRight() != NULL)
		{
			updater = updater->getRight();
		}
		updateHeights(updater);
	}

	Node<Key, Value>* z = NULL;
	Node<Key, Value>* y = NULL;
	Node<Key, Value>* x = NULL;

	//Checker: if only 2 nodes left in tree then do not check for balance
	if(almostEmpty())
	{
		return;
	}

	//After removing checks if heigths are inbalanced
	if((updater == this->mRoot || updater->getHeight() >= 2))
	{
		if(updater->getLeft() != NULL)
		{
			//Only if unbalanced at root
			if(updater->getLeft()->getHeight() >= 1)
			{
				updater = updater->getLeft();
				unbalanced = true;
			}
		}
		else if(updater->getRight() != NULL)
		{
			if(updater->getRight()->getHeight() >= 1)
			{
				updater = updater->getRight();
				unbalanced = true;
			}
		}

		if(unbalanced)
		{
			unbalanceSetUp(updater, z, y, x);
		}

		if(x != NULL && y != NULL && z != NULL)
		{
			balance(z, y, x);
			unbalanced = false;
		}
	}
	
	//Rechecks balance 
	if(checkBalance(updater, unbalanced) && updater != this->mRoot)
	{
		if(updater->getKey() < updater->getParent()->getKey())
		{
			updater = updater->getParent()->getRight();
		}
		else
		{
			updater = updater->getParent()->getLeft();
		}

		unbalanceSetUp(updater, z, y, x);
		if(x != NULL && y != NULL && z != NULL)
		{
			balance(z, y, x);
		}
	}

	unbalanced = false;
}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
