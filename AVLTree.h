#pragma once
#include <iostream>
#include <limits>
#include <vector>
using namespace std;


template <typename Comparable>
class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    void makeEmpty();
    const Comparable& findMin() const;
    const Comparable& findMax() const;

    bool contains(const Comparable& x) const;
    void insert(const Comparable& x);
    void remove(const Comparable& x);
    int treeSize() const;
    int computeHeight() const;
    int readRootHeight() const;
    bool isBalanced() const;
    bool isBST() const;

    double averageDepth() const;
    void removeByRank(int rank);

    // the next line follows textbook Figure 4.42, Line 19
    static const int ALLOWED_IMBALANCE = 1;
private:
    struct AVLNode  // refer to textbook, Figure 4.40
    {
        Comparable element;
        AVLNode* left;
        AVLNode* right;
        int height;

        AVLNode(const Comparable& theElement, AVLNode* lt, AVLNode* rt) : element(theElement), left(lt), right(rt) {}
        AVLNode(Comparable&& theElement, AVLNode* lt, AVLNode* rt) : element(move(theElement)), left(lt), right(rt) {}
    };

    AVLNode* root;

    AVLNode* findMin(AVLNode* t) const;
    AVLNode* findMax(AVLNode* t) const;
    void makeEmpty(AVLNode*& t);

    void balance(AVLNode*& t);
    int height(AVLNode* t) const;

    void rotateWithLeftChild(AVLNode*& t);
    void rotateWithRightChild(AVLNode*& t);
    void doubleWithLeftChild(AVLNode*& t);
    void doubleWithRightChild(AVLNode*& t);

    bool contains(const Comparable& x, AVLNode* t)const
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
    void insert(const Comparable& x, AVLNode*& t)
    {
        if (t == nullptr)
        {
            t = new AVLNode{ x, nullptr, nullptr };
            
        }
        else if (x < t->element)
        {
            insert(x, t->left);
        }
        else if (x > t->element)
        {
            insert(x, t->right);
        }
        balance(t);
    }
    void remove(const Comparable& x, AVLNode*& t)
    {
        if (t == nullptr)
        {
            return;
        }
        if (x < t->element)
        {
            remove(x, t->left);
        }
        else if (x > t->element)
        {
            remove(x, t->right);
        }
        else
        {
            if (t->left == nullptr)
            {
                AVLNode* oldNode = t;
                t = t->right;
                delete oldNode;
            }
            else if (t->right == nullptr)
            {
                AVLNode* oldNode = t;
                t = t->left;
                delete oldNode;
            }

            else
            {
                AVLNode* minnode = findMin(t->right);
                t->element = minnode->element;
                remove(t->element, t->right);
            }
        }
        balance(t);
    }

    int computeHeight(AVLNode* t)const
    {
        if (t == nullptr)
        {
            return -1;
        }
        int left = computeHeight(t->left);
        int right = computeHeight(t->right);
        int height = 1 + max(left, right);

        //return height;
        return 1 + max(left, right);
    }
    int treeSize(AVLNode* t)const
    {
        if (t == nullptr)
        {
            return 0;
        }
        else
        {
            int size = 1 + treeSize(t->left) + treeSize(t->right);
            return size;
        }

    }
    void removeByRank(int rank, AVLNode*& t)
    {
        if (t == nullptr)
        {
            return;
        }
        int lSize = treeSize(t->left);

        if (rank < lSize)
        {
            removeByRank(rank, t->left);
        }
        else if (rank > lSize)
        {
            removeByRank(rank - lSize - 1, t->right);
        }
        else
        {
            if (t->left == nullptr && t->right == nullptr)
            {
                delete t;
                t = nullptr;
            }
            else if (t->right == nullptr)
            {
                AVLNode* oldNode = t;
                t = t->left;
                delete oldNode;
            }
            else if (t->left == nullptr)
            {
                AVLNode* oldNode = t;
                t = t->right;
                delete oldNode;
            }
            else
            {
                AVLNode* minNode = findMin(t->right);
                t->element = minNode->element;
                removeByRank(0, t->right);
            }
        }

    }

    bool isBalanced(AVLNode* node)const
    {
        if (node == nullptr)
        {
            return true;
        }
        int lHeight = isBalanced(node->left);
        int rHeight = isBalanced(node->right);
        
        int balance = lHeight - rHeight;
        if (lHeight == -1 || rHeight == -1)
        {
            return -1;
        }

       

        return(balance >= -1 && balance <= 1) && isBalanced(node->left) && isBalanced(node->right);
   }

    bool isBST(AVLNode* node)const
    {
        if (node == nullptr)
        {
            return true;
        }
        if (node->left != nullptr && node->left->element >= node->element)
            return false;
        if (node->right != nullptr && node->right->element <= node->element)
            return false;
        return isBST(node->left) && isBST(node->right);
    }
    double averageDepth(AVLNode* node,int currDepth, int& nodeNum)const
    {
        if (node == nullptr)
        {
            return 0.0;
        }
        nodeNum++;
        double leftD = averageDepth(node->left,currDepth + 1, nodeNum);
        double rightD = averageDepth(node->right,currDepth + 1, nodeNum);
        double sum = leftD + rightD + currDepth;
        
        return sum;
       // int total = 1 + (node->left != nullptr) + (node->right != nullptr);

    }
};

   

    // constructor
    template <class Comparable>
    AVLTree<Comparable>::AVLTree() : root(NULL) {}

    // destructor
    template <class Comparable>
    AVLTree<Comparable>::~AVLTree()
    {
        makeEmpty();
    }

    // public makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
    template <typename Comparable>
    void AVLTree<Comparable>::makeEmpty() {
        makeEmpty(root);
    }

    // private recursive makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
    template <typename Comparable>
    void AVLTree<Comparable>::makeEmpty(AVLNode*& t) {
        if (t != NULL) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
            t = NULL;
        }
    }

    // public findMin: follow the findMin in BST, referring to textbook, Figure 4.20
    template <typename Comparable>
    const Comparable& AVLTree<Comparable>::findMin() const {
        if (root == NULL) {
            throw underflow_error("Tree is empty");
        }
        return findMin(root)->element;
    }

    // private findMin: follow the findMin in BST, referring to textbook, Figure 4.20
    template <typename Comparable>
    typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMin(AVLNode* t) const {
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

    // public findMax: follow the findMax in BST, referring to textbook, Figure 4.21
    template <typename Comparable>
    const Comparable& AVLTree<Comparable>::findMax() const {
        if (root == NULL) {
            throw underflow_error("Tree is empty");
        }
        return findMax(root)->element;
    }

    // private findMax: follow the findMax in BST, referring to textbook, Figure 4.21
    template <typename Comparable>
    typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMax(AVLNode* t) const {
        if (t == NULL) {
            return NULL;
        }
        else if (t->right == NULL) {
            return t;
        }
        else {
            return findMax(t->right);
        }
    }

    // start our implementation:
    // public contains: follow the contains in BST, referring to textbook, Figure 4.17 and Figure 4.18
    template<typename Comparable>
    bool AVLTree<Comparable>::contains(const Comparable& x)const {
        //cout << "TODO: contains function" << endl;
       return contains(x, root); //false;
    }

    // public insert: following BST, referring to textbook, Figure 4.17 and Figure 4.23
    template<typename Comparable>
    void AVLTree<Comparable>::insert(const Comparable& x) 
    {
        insert(x, root);
        
    }

    // public remove: refer to textbook, Figure 4.17 and Figure 4.26
    template<typename Comparable>
    void AVLTree<Comparable>::remove(const Comparable& x) {
        remove(x, root);
        /*cout << "TODO: remove function" << endl;*/
    }

    template<typename Comparable>
    int AVLTree<Comparable>::height(AVLNode* t)const
    {
        return t == nullptr ? -1 : t->height;
    }
    // private balance: refer to textbook, Figure 4.42, Line 21 - 40
    // assume t is the node that violates the AVL condition, and we then identify which case to use (out of 4 cases)
    template<typename Comparable>
    void AVLTree<Comparable>::balance(AVLNode*& t) 
    {
        if (t == nullptr)
        {
            return;
        }
        int balanceF = height(t->left) - height(t->right);
        if (balanceF > ALLOWED_IMBALANCE)
        {
            if (t->left != nullptr && height(t->left->left) >= height(t->left->right))
            {
                rotateWithLeftChild(t);
            }
            else
            {
                doubleWithLeftChild(t);
            }
        }
        else if (balanceF < -ALLOWED_IMBALANCE)
        {
            if (t->right != nullptr && height(t->right->right) >= height(t->right->left))
            {
                rotateWithRightChild(t);
            }
            else
            {
                doubleWithRightChild(t);
            }

        }
        //t->height = max(height(t->left), height(t->right)) + 1;

        t->height = max(height(t->left), height(t->right)) + 1;
        
        
        
        //cout << "TODO: balance function" << endl;
    }

    // private rotateWithLeftChild: for case 1, referring to textbook, Figure 4.44 (code) and Figure 4.43 (visualization)
    template<typename Comparable>
    void AVLTree<Comparable>::rotateWithLeftChild(AVLNode*& k2) 
    {
        /*if (k2 == nullptr || k2->left == nullptr)
        {
            return;
        }*/
        AVLNode* k1 = k2->left; //setting k1 to the left child of k2
        k2->left = k1->right; //k1's right to k2's left
        k1->right = k2; //rotating k2 down
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height)+ 1;
        k2 = k1; //new root

    }

    // private rotateWithRightChild: for case 4 (the mirrored case of case 1)
    template<typename Comparable>
    void AVLTree<Comparable>::rotateWithRightChild(AVLNode*& k2) 
    {
        /*if (k2 == nullptr || k2->right == nullptr)
        {
            return;
        }*/
        AVLNode* k1 = k2->right; //setting k1 to the right child of k2
        k2->right = k1->left; //k1's left to k2's right
        k1->left = k2; //rotating k2

        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
        
    }

    // private doubleWithLeftChild: for case 2, see textbook, Figure 4.46 (code) and Figure 4.45 (visualization)
    template<typename Comparable>
    void AVLTree<Comparable>::doubleWithLeftChild(AVLNode*& k3)
    {
        rotateWithRightChild(k3->left); //right rotation on left child
        rotateWithLeftChild(k3); //left rotation on K3
    }
        

    // private doubleWithRightChild: for case 3 (the mirrored case of case 2)
    template<typename Comparable>
    void AVLTree<Comparable>::doubleWithRightChild(AVLNode*& k3) {
        rotateWithLeftChild(k3->right); //left rotation on right child
        rotateWithRightChild(k3); //right rotation on k3
    }

    // public isBalanced
    template <class Comparable>
    bool AVLTree<Comparable>::isBalanced() const {
        return isBalanced(root);
    }

    // public isBST
    template <class Comparable>
    bool AVLTree<Comparable>::isBST() const {
        return isBST(root);
    }

    // public treeSize
    template <typename Comparable>
    int AVLTree<Comparable>::treeSize() const {
        /*cout << "TODO: treeSize function" << endl;*/
        return treeSize(root);
    }

    // public computeHeight. See Figure 4.61 in Textbook
    template <typename Comparable>
    int AVLTree<Comparable>::computeHeight() const {
       // AVLNode* t = root;
        return computeHeight(root);
       // return 0;

    }

    // public readRootHeight
    template <typename Comparable>
    int AVLTree<Comparable>::readRootHeight() const {
        /*cout << "TODO: readRootHeight function" << endl;*/
        return computeHeight(root);
    }

    // public averageDepth
    template <typename Comparable>
    double AVLTree<Comparable>::averageDepth() const {
        /*cout << "TODO: averageDepth function" << endl;*/
        int nodeNum = 0;
        double total = averageDepth(root,0, nodeNum);

        if (nodeNum == 0)
        {
            return 0.0;
        }
        else
        {
            return total/nodeNum;
        }
      
    }

    // public removeByRank
    template <typename Comparable>
    void AVLTree<Comparable>::removeByRank(int rank) 
    {
        removeByRank(rank, root);

       // cout << "TODO: removeByRank function" << endl;
    }


