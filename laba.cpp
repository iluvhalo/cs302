// Matt Matto
// Lab A
// implements the method in laba.h
// preorder print, postorder print, depth, height, and rotate


#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "laba.h"
using namespace std;

// recursively prints the BST in preorder format
void BSTree::recursive_preorder(int level, BSTNode *n) {
	
	// prints the correct amount of spcaes
	for (int i = 0; i < level; i++) {
		printf("  ");
	}
	
	// prints the name
	printf("%s\n", n->key.c_str());
	
	// recursively goes down the tree to print
	if (n->left != sentinel) recursive_preorder(level + 1, n->left);
	if (n->right != sentinel) recursive_preorder(level + 1, n->right);
	
	// returns
	return;
}

// recursively prints the BST in postorder format
void BSTree::recursive_postorder(int level, BSTNode *n) {
	
	//recursively goes down the tree to print
	if (n->left != sentinel) recursive_postorder(level + 1, n->left);
	if (n->right != sentinel) recursive_postorder(level + 1, n->right);
	
	// prints the correct number of spaces
	for (int i = 0; i < level; i++) {
		printf("  ");
	}
	
	// prints the name
	printf("%s\n", n->key.c_str());
	
	return;
}

int BSTree::recursive_height(BSTNode *n) {
	// initializes the right and left height ints
	int l = 0, r = 0;

	// checks for if you have reached a leaf
	if (n == sentinel) return -1;
	// goes down the right and left branch
	l = recursive_height(n->left);
	r = recursive_height(n->right);

	// returns the bigger of the two height ints
	if (l > r) return (l + 1);
	else return (r + 1);
}

// calls the recursive postorder function
void BSTree::Postorder() {
	recursive_postorder(0, sentinel->right);
}

// calls the recursive preorder function
void BSTree::Preorder() {
	recursive_preorder(0, sentinel->right);
}

// returns the depth of a given key
int BSTree::Depth(string key) {

	// initializes the depth counter and BSTNode iterator
	int count = 0;
	BSTNode *n = sentinel->right;

	// searches until it reaches a sentinel, signaling the key was not found
	while (n != sentinel) {
		// goes left down the tree
		if (key < n->key) {
			count++;
			n = n->left;
		// goes right down the tree
		} else if (key > n->key) {
			count++;
			n = n->right;
		// if the key was found, returns the depth of the key
		} else if (key == n->key) {
			return count;
		}
	}

	// if it cant find the key, returns -1
	return -1;
}

// checks to see if the tree if empty, then calls the recursive height function
int BSTree::Height() {
	if (Empty()) return -1;
	return recursive_height(sentinel->right);
}

// checks if a BST is also and AVL tree
int BSTree::IsAVL() {
	
	// if it is empty, returns -1
	if (Empty()) return -1;

	//calls the recursive height and avl check function
	int height;
	height = recursive_height_and_avl_check(sentinel->right);
	
	// if the function returned -2, then the tree is not an AVL tree
	if (height == -2) return 0;
	else return 1;
}

// checks if a tree is an AVL tree
int BSTree::recursive_height_and_avl_check(BSTNode *n) {
	// initializes the right and left heights
	int l = 0, r = 0;
	if (n == sentinel) return 0;
	// calls the function for the left and right branches and goes down the tree
	l = recursive_height_and_avl_check(n->left);
	r = recursive_height_and_avl_check(n->right);
	// checks that the left anf right heights differ by no more than 1
	// also checks if a -2 is being passed up the function
	if (((l - r) < -1) or ((l - r) > 1) or (l == -2) or (r == -2)) return -2;
	// returns the bigger of te two heights
	if (l > r) return (l + 1);
	else return (r + 1);
}

// rotates a branch around a particular node
int BSTree::Rotate(string key) {
	BSTNode *n = sentinel->right;
	BSTNode *p;
	
	// searches for a key, and if it is found rotates the tree
	while (n != sentinel) {
		// the key was found
		if (n->key == key) {
			// checks to make sure the tree is not empty
			if (n == sentinel->right) return 0;
			
			// checks if n is to the left of its parent
			if (n == n->parent->left) {
				// sets the parent pointer(p) and n's parent pointer is p's parent
				p = n->parent;
				n->parent = p->parent;
				
				// checks and adjusts the grandparent's pointer to n
				if (n->parent->right == p) p->parent->right = n;
				else if (p->parent->left == p) p->parent->left = n;
				
				// keep the middle node in the middle
				p->left = n->right;
				
				// moves the last few around
				if (n->right != sentinel) n->right->parent = p;
				n->right = p;
			// if n is to the right of the parent
			} else {
				//sets the parent pointer(p) nd n's parent pointer to p's parent
				p = n->parent;
				n->parent = p->parent;

				// adjusts the grandparent pointer to n
				if (p->parent->right == p) p->parent->right = n;
				else if (p->parent->left == p) p->parent->left = n;
				
				// keeps the middle pointer in the middle
				p->right = n->left;
				
				// makes sure n->left is filled and if it is pointer it back to p
				if (n->left != sentinel) n->left->parent = p;
				
				// n becoms the parent to p
				n->left = p;
			}
			
			// p points back to n
			p->parent = n;

			// success
			return 1;
		}

		// travels down through the tree trying to find the key
		else if (key < n->key) n = n->left;
		else n = n->right;
	}

	// it failed
	return 0;
}

// ------------------------------------------------------------------------
// eveything after this was already in laba.cpp
// ------------------------------------------------------------------------

BSTree::BSTree()
{
  sentinel = new BSTNode;
  sentinel->parent = NULL;
  sentinel->left = NULL;
  sentinel->right = sentinel;
  size = 0;
}

BSTree::~BSTree()
{
  recursive_destroy(sentinel->right);
  delete sentinel;
}

int BSTree::Insert(string s, void *val)
{
  BSTNode *parent;
  BSTNode *n;

  parent = sentinel;
  n = sentinel->right;

  while (n != sentinel) {
    if (n->key == s) return 0;
    parent = n;
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }

  n = new BSTNode;
  n->key = s;
  n->val = val;
  n->parent = parent;
  n->left = sentinel;
  n->right = sentinel;
  if (parent == sentinel) {
    sentinel->right = n;
  } else if (s < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }
  size++;
  return 1;
}

void *BSTree::Find(string s)
{
  BSTNode *n;

  n = sentinel->right;
  while (1) {
    if (n == sentinel) return NULL;
    if (s == n->key) return n->val;
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }
}

int BSTree::Delete(string s)
{
  BSTNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  n = sentinel->right;
  while (n != sentinel && s != n->key) {
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }
  if (n == sentinel) return 0;

  parent = n->parent;
  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;
  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;
  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
  }
  return 1;
}

vector <void *>BSTree::Sorted_Vector()
{
  array.clear();
  recursive_make_vector(sentinel->right);
  return array;
}

void BSTree::Print()
{
  recursive_inorder_print(0, sentinel->right);
}

int BSTree::Size()
{
  return size;
}

int BSTree::Empty()
{
  return (size == 0);
}

void BSTree::recursive_inorder_print(int level, BSTNode *n)
{
  if (n == sentinel) return;
  recursive_inorder_print(level+2, n->right);
  printf("%*s%s\n", level, "", n->key.c_str());
  recursive_inorder_print(level+2, n->left);
}

void BSTree::recursive_make_vector(BSTNode *n)
{
  if (n == sentinel) return;
  recursive_make_vector(n->left);
  array.push_back(n->val);
  recursive_make_vector(n->right);
}

void BSTree::recursive_destroy(BSTNode *n)
{
  if (n == sentinel) return;
  recursive_destroy(n->left);
  recursive_destroy(n->right);
  delete n;
}
