#ifndef __BST_H
#define __BST_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
using namespace std;

#ifndef __BST_H
#define __BST_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
using namespace std;


/* ----------------------------------------------------------------------------
---- Below is declaration of BST class, referring to textbook, Figure 4.16 ----
---------------------------------------------------------------------------- */

template <typename Comparable>
class BST
{
public:
	BST();
	~BST();
	void makeEmpty();

	const Comparable& findMin() const;
	const Comparable& findMax() const;

	bool contains(const Comparable& x) const;
	void insert(const Comparable& x);
	void remove(const Comparable& x);
	int treeSize() const;
	int treeHeight() const;
	void printInOrder() const;
	void printLevels() const;
	void printMaxPath() const;

private:
	struct BinaryNode
	{
		Comparable element;
		BinaryNode* left;
		BinaryNode* right;

		BinaryNode(const Comparable& theElement, BinaryNode* lt = nullptr, BinaryNode* rt= nullptr) : element(theElement), left(lt), right(rt) {}
		BinaryNode(Comparable&& theElement, BinaryNode* lt, BinaryNode* rt) : element(move(theElement)), left(lt), right(rt) {}
	};

	BinaryNode* root;

	BinaryNode* findMin(BinaryNode* t) const;
	BinaryNode* findMax(BinaryNode* t) const;
	void makeEmpty(BinaryNode*& t);
	
	bool contains(const Comparable& x, BinaryNode* t)const
	{
		if (t == nullptr)
		{
			return false;
		}
		if (x == t->element)
		{
			return true;
		}
		if (x < t->element)
		{
			return contains(x, t->left);
		}
		else
		{
			return contains(x, t->right);
		}
	}
	void insert(const Comparable& x, BinaryNode*& t)const
	{
		if ( t == nullptr)
		{
			t = new BinaryNode(x);
		}
		else if (x < t->element)
		{
			insert(x, t->left);
		}
		else if (x > t->element)
		{
			insert(x, t->right);
		}
	
		return;
	}
	void remove(const Comparable& x, BinaryNode*& t)const
	{
		if (t == nullptr)
		{
			return;
		}
		if (x < t->element)
		{
			remove(x, t->left);
		}
		if (t->element < x)
		{
			remove(x, t->right);
		}
		else if (t->left == nullptr)
		{
			BinaryNode* oldNode = t;
			t = t->left;
			delete oldNode;
		}
		else if (t->right == nullptr)
		{
			BinaryNode* oldNode = t;
			t = t->right;
			delete oldNode;
		}
		else
		{
			BinaryNode* minnode = findMin(t->right);
			t->element = minnode->element;
			remove(t->element, t->right);
		}

	}
	void printInOrder(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return;
		}
		printInOrder(node->left);
		cout << node->element << " -> ";
		printInOrder(node->right);
	}
	int treeHeight(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return -1;
		}
		
		int left = treeHeight(node->left);
		int right = treeHeight(node->right);
		
		return 1 + max(left, right);
	}
	void printLevels(BinaryNode* node) const
	{
		int height = treeHeight(root);
		for (int i = 0; i <= height; i++)
		{
			cout << "\nAt level:" << i << endl;
			printTotalLevels(root,i);
		}
		return;
	}
	void printTotalLevels(BinaryNode* node, int level) const
	{
		if (node == nullptr)
		{
			return;
		}
		if (level == 0)
		{
			cout << " " << node->element;
		}
		else if (level > 0)
		{
			printTotalLevels(node->left, level - 1);
			printTotalLevels(node->right, level - 1);
		}
	}
	int treeSize(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return 0;
		}
		else 
		{
			return 1 + treeSize(node->left) + treeSize(node->right);

		}

	}
	vector<int> printMaxPath(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return {};
		}
			vector<int> right = printMaxPath(node->right);
			vector<int> left = printMaxPath(node->left);
			if (left.size() > right.size())
			{
				left.push_back(node->element);
				return left;
			}
			else
			{
				right.push_back(node->element);
				return right;
			}
			
	}
	
	};


/* --------------------------------------------------------------
---- Below is implementation of public and private functions ----
-------------------------------------------------------------- */

// constructor
template<typename Comparable>
BST<Comparable>::BST() : root(NULL) {}

// destructor, refer to textbook, Figure 4.27
template<typename Comparable>
BST<Comparable>::~BST() {
	makeEmpty();
}

// public makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty() {
	makeEmpty(root);
}

// private recursive makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty(BinaryNode*& t) {
	if (t != NULL) {
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
		t = NULL;
	}
}

// // public findMin
template <typename Comparable>
const Comparable& BST<Comparable>::findMin() const {
	if (root == NULL) {
		throw underflow_error("Tree is empty");
	}
	return findMin(root)->element;
}

// private findMin: refer to textbook, Figure 4.20
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMin(BinaryNode* t) const {
	if (t == NULL) {
		return NULL;
	}
	else if (t->left == NULL) {
		return t;
	}
	else {
		return findMin(t->left);
	}
}

// public findMax
template <typename Comparable>
const Comparable& BST<Comparable>::findMax() const {
	if (root == NULL) {
		throw underflow_error("Tree is empty");
	}
	return findMax(root)->element;
}

// private findMax: refer to textbook, Figure 4.21
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMax(BinaryNode* t) const {
	if (t == NULL) {
		return NULL;
	}
	else if (t->right == NULL) {
		return t;
	}
	else {
		return findMin(t->right);
	}
}

// public contains: refer to textbook, Figure 4.17, Line 4 - 7
template<typename Comparable>
bool BST<Comparable>::contains(const Comparable& x) const {
	return contains(x, root);
	
}

// public insert: refer to textbook, Figure 4.17, Line 12 - 15
template<typename Comparable>
void BST<Comparable>::insert(const Comparable& x) 
{
	insert(x, root);
	/*cout << "**TODO**: insert function" << endl;*/
}


// public remove: refer to textbook, Figure 4.17, Line 20 - 23
template<typename Comparable>
void BST<Comparable>::remove(const Comparable& x) {
	remove(x, root);
	/*cout << "**TODO**: remove function" << endl;*/
}

// public treeSize
template <typename Comparable>
int BST<Comparable>::treeSize() const 
{
	return treeSize(root); 
}

// public treeHeight
template <typename Comparable>
int BST<Comparable>::treeHeight() const {
	BinaryNode* node = root;
	return treeHeight(node);
	/*cout << "**TODO**: treeHeight function" << endl;*/
	return 0;
}

// public printInOrder: refer to textbook, Figure 4.60
template<typename Comparable>
void BST<Comparable>::printInOrder() const 
{
	printInOrder(root);
	/*cout << "**TODO**: printInOrder function" << endl;*/
}

// public printLevels
template <typename Comparable>
void BST<Comparable>::printLevels()const{
	printLevels(root);
	/*cout << "**TODO**: printLevels function" << endl;*/
}

// public printMaxPath
template <typename Comparable>
void BST<Comparable>::printMaxPath() const 
{
	vector<int> maxpath = printMaxPath(root);
	for (size_t i = maxpath.size(); i>0; --i)
	{
		cout << maxpath[i-1];
		if (i>1)
		{
			cout << " -> ";
		}
	}
	cout << endl;
	/*cout << "**TODO**: printMaxPath function" << endl;*/
}

#endif



/* ----------------------------------------------------------------------------
---- Below is declaration of BST class, referring to textbook, Figure 4.16 ----
---------------------------------------------------------------------------- */

template <typename Comparable>
class BST
{
public:
	BST();
	~BST();
	void makeEmpty();

	const Comparable& findMin() const;
	const Comparable& findMax() const;

	bool contains(const Comparable& x) const;
	void insert(const Comparable& x);
	void remove(const Comparable& x);
	int treeSize() const;
	int treeHeight() const;
	void printInOrder() const;
	void printLevels() const;
	void printMaxPath() const;

private:
	struct BinaryNode
	{
		Comparable element;
		BinaryNode* left;
		BinaryNode* right;

		BinaryNode(const Comparable& theElement, BinaryNode* lt = nullptr, BinaryNode* rt= nullptr) : element(theElement), left(lt), right(rt) {}
		BinaryNode(Comparable&& theElement, BinaryNode* lt, BinaryNode* rt) : element(move(theElement)), left(lt), right(rt) {}
	};

	BinaryNode* root;

	BinaryNode* findMin(BinaryNode* t) const;
	BinaryNode* findMax(BinaryNode* t) const;
	void makeEmpty(BinaryNode*& t);
	
	bool contains(const Comparable& x, BinaryNode* t)const
	{
		if (t == nullptr)
		{
			return false;
		}
		if (x == t->element)
		{
			return true;
		}
		if (x < t->element)
		{
			return contains(x, t->left);
		}
		else
		{
			return contains(x, t->right);
		}
	}
	void insert(const Comparable& x, BinaryNode*& t)const
	{
		if ( t == nullptr)
		{
			t = new BinaryNode(x);
		}
		else if (x < t->element)
		{
			insert(x, t->left);
		}
		else if (x > t->element)
		{
			insert(x, t->right);
		}
	
		return;
	}
	void remove(const Comparable& x, BinaryNode*& t)const
	{
		if (t == nullptr)
		{
			return;
		}
		if (x < t->element)
		{
			remove(x, t->left);
		}
		if (t->element < x)
		{
			remove(x, t->right);
		}
		else if (t->left == nullptr)
		{
			BinaryNode* oldNode = t;
			t = t->left;
			delete oldNode;
		}
		else if (t->right == nullptr)
		{
			BinaryNode* oldNode = t;
			t = t->right;
			delete oldNode;
		}
		else
		{
			BinaryNode* minnode = findMin(t->right);
			t->element = minnode->element;
			remove(t->element, t->right);
		}

	}
	void printInOrder(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return;
		}
		printInOrder(node->left);
		cout << node->element << " -> ";
		printInOrder(node->right);
	}
	int treeHeight(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return -1;
		}
		
		int left = treeHeight(node->left);
		int right = treeHeight(node->right);
		
		return 1 + max(left, right);
	}
	void printLevels(BinaryNode* node) const
	{
		int height = treeHeight(root);
		for (int i = 0; i <= height; i++)
		{
			cout << "\nAt level:" << i << endl;
			printTotalLevels(root,i);
		}
		return;
	}
	void printTotalLevels(BinaryNode* node, int level) const
	{
		if (node == nullptr)
		{
			return;
		}
		if (level == 0)
		{
			cout << " " << node->element;
		}
		else if (level > 0)
		{
			printTotalLevels(node->left, level - 1);
			printTotalLevels(node->right, level - 1);
		}
	}
	int treeSize(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return 0;
		}
		else 
		{
			return 1 + treeSize(node->left) + treeSize(node->right);

		}

	}
	vector<int> printMaxPath(BinaryNode* node)const
	{
		if (node == nullptr)
		{
			return {};
		}
			vector<int> right = printMaxPath(node->right);
			vector<int> left = printMaxPath(node->left);
			if (left.size() > right.size())
			{
				left.push_back(node->element);
				return left;
			}
			else
			{
				right.push_back(node->element);
				return right;
			}
			
	}
	
	};


/* --------------------------------------------------------------
---- Below is implementation of public and private functions ----
-------------------------------------------------------------- */

// constructor
template<typename Comparable>
BST<Comparable>::BST() : root(NULL) {}

// destructor, refer to textbook, Figure 4.27
template<typename Comparable>
BST<Comparable>::~BST() {
	makeEmpty();
}

// public makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty() {
	makeEmpty(root);
}

// private recursive makeEmpty: refer to textbook, Figure 4.27
template <typename Comparable>
void BST<Comparable>::makeEmpty(BinaryNode*& t) {
	if (t != NULL) {
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
		t = NULL;
	}
}

// // public findMin
template <typename Comparable>
const Comparable& BST<Comparable>::findMin() const {
	if (root == NULL) {
		throw underflow_error("Tree is empty");
	}
	return findMin(root)->element;
}

// private findMin: refer to textbook, Figure 4.20
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMin(BinaryNode* t) const {
	if (t == NULL) {
		return NULL;
	}
	else if (t->left == NULL) {
		return t;
	}
	else {
		return findMin(t->left);
	}
}

// public findMax
template <typename Comparable>
const Comparable& BST<Comparable>::findMax() const {
	if (root == NULL) {
		throw underflow_error("Tree is empty");
	}
	return findMax(root)->element;
}

// private findMax: refer to textbook, Figure 4.21
template <typename Comparable>
typename BST<Comparable>::BinaryNode* BST<Comparable>::findMax(BinaryNode* t) const {
	if (t == NULL) {
		return NULL;
	}
	else if (t->right == NULL) {
		return t;
	}
	else {
		return findMin(t->right);
	}
}

// public contains: refer to textbook, Figure 4.17, Line 4 - 7
template<typename Comparable>
bool BST<Comparable>::contains(const Comparable& x) const {
	return contains(x, root);
	
}

// public insert: refer to textbook, Figure 4.17, Line 12 - 15
template<typename Comparable>
void BST<Comparable>::insert(const Comparable& x) 
{
	insert(x, root);
	
}


// public remove: refer to textbook, Figure 4.17, Line 20 - 23
template<typename Comparable>
void BST<Comparable>::remove(const Comparable& x) {
	remove(x, root);
	
}

// public treeSize
template <typename Comparable>
int BST<Comparable>::treeSize() const 
{
	return treeSize(root); 
}

// public treeHeight
template <typename Comparable>
int BST<Comparable>::treeHeight() const {
	BinaryNode* node = root;
	return treeHeight(node);
	return 0;
}

// public printInOrder: refer to textbook, Figure 4.60
template<typename Comparable>
void BST<Comparable>::printInOrder() const 
{
	printInOrder(root);
}

// public printLevels
template <typename Comparable>
void BST<Comparable>::printLevels()const{
	printLevels(root);
}

// public printMaxPath
template <typename Comparable>
void BST<Comparable>::printMaxPath() const 
{
	vector<int> maxpath = printMaxPath(root);
	for (size_t i = maxpath.size(); i>0; --i)
	{
		cout << maxpath[i-1];
		if (i>1)
		{
			cout << " -> ";
		}
	}
	cout << endl;
}

#endif


