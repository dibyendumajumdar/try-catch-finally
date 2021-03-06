/***
*    YASE (Yet Another Search Engine) 
*    Copyright (C) 1995-2003  Dibyendu Majumdar.
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*    Author : Dibyendu Majumdar
*    Email  : dibyendu@mazumdar.demon.co.uk
*    Website: www.mazumdar.demon.co.uk/yase_index.html
*/
/*
 * --- avl3a.c (version 3.0) 
 * --- started oct 21 1995 
 * --- revised oct 24 1995
 * --- revised jul 6 1996 
 * --- revised jan 27 1997
 * --- revised jan 17 1997
 * --- author : dibyendu majumdar 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "avl3.h"
#include "avl3int.h"
#include "alloc.h"

extern void
AVLTree_ForwardApply(AVLNode * root, void (*funcptr)(void *));

/* ys_allocate a new node */
AVLNode        *
AVL_new(AVLTree * tree, void * key)
{
	AVL_vtbl       *vtbl = tree->vptr;
	AVLNode        *n = (AVLNode *) ys_allocate(tree->a, 0); 
	/* AVLNode        *n = (AVLNode *) calloc(1, tree->a->size); */

	if (n == NULL) {
		fprintf(stderr, "Error [AVL_new]: Out of memory\n");
		exit(-1);
	}
	n->left = n->right = n->parent = NULL;
	n->balance = Equal;

	CONSTRUCT_NODE(n, key);

	assert(n->left == NULL && n->right == NULL && n->parent == NULL);
	assert(n->balance == Equal);

	return n;
}

/* insert a key into an AVL tree and return new object inserted */
void           *
AVLTree_Insert(AVLTree * tree, void * k)
{
#ifndef __CXC__
	AVLNode        *root = (AVLNode *) tree->root;
	AVLNode        *found = NULL;
	AVL_vtbl       *vtbl = tree->vptr;
#else
	AVLNode        *root;
	AVLNode        *found;
	AVL_vtbl       *vtbl;

	root = (AVLNode *) tree->root;
	found = NULL;
	vtbl = tree->vptr;
#endif

	/* is the tree empty ? */
	if (root == NULL) {
		found = root = AVL_new(tree, k);
	} else {
		/* find insertion point */
#ifndef __CXC__
		int             height_changed = 0;
		AVLNode        *p = NULL, *n = NULL;
#else
		int             height_changed;
		AVLNode        *p, *n;

		height_changed = 0;
		p = NULL, n = NULL;
#endif

		n = root;
		do {
#ifndef __CXC__
			int             compresult = 0;
#else
			int             compresult;
			compresult = 0;
#endif

			p = n;
			compresult = COMP(k, n);
			if (compresult < 0)
				n = p->left;
			else if (compresult > 0)
				n = p->right;
			else
				return (void *) (n + 1);
		} while (n != 0);
		found = n = AVL_new(tree, k);

		/* insert */
		if (COMP(k, p) < 0)
			AVL_SetLeft(p, n);
		else
			AVL_SetRight(p, n);

		/* rebalance */
		height_changed = 1;
		while (p != 0 && height_changed) {
			if (n == p->left)
				p = AVL_RebalanceHeavierLeft(p, 
							&height_changed);
			else
				p = AVL_RebalanceHeavierRight(p, 
							&height_changed);
			n = p;
			p = p->parent;
		}
		if (p == 0)
			root = n;
	}
	tree->root = root;
	return (void *) (found + 1);
}

/* delete a key from an AVL tree */
int 
AVLTree_Delete(AVLTree * tree, void * k)
{
#ifndef __CXC__
	AVLNode        *root = (AVLNode *) tree->root;
	AVLNode        *n = NULL;
	AVL_vtbl       *vtbl = tree->vptr;
#else
	AVLNode        *root;
	AVLNode        *n;
	AVL_vtbl       *vtbl;

	root = (AVLNode *) tree->root;
	n = NULL;
	vtbl = tree->vptr;
#endif

	if (root == NULL)
		return -1;

	/* search */
	n = root;
	while (n != NULL) {
#ifndef __CXC__
		int             compresult = COMP(k, n);
#else
		int             compresult;

		compresult = COMP(k, n);
#endif
		if (compresult < 0)
			n = n->left;
		else if (compresult > 0)
			n = n->right;
		else
			break;
	}
	if (n == NULL)
		return -1;
	AVLTree_DeleteObject(tree, n+1);
	return 0;
}

/* delete an object from an AVL tree */
int 
AVLTree_DeleteObject(AVLTree * tree, void *obj)
{
#ifndef __CXC__
	AVLNode        *root = (AVLNode *) tree->root;
	AVLNode        *discard = NULL, *p = NULL, *n = (AVLNode *)obj;
	int             height_changed = 0;
	AVL_vtbl       *vtbl = tree->vptr;
	int             flag = 0;
#else
	AVLNode        *root;
	AVLNode        *discard, *p, *n;
	int             height_changed;
	AVL_vtbl       *vtbl;
	int             flag;

	root = (AVLNode *) tree->root;
	discard = NULL, p = NULL, n = (AVLNode *)obj;
	height_changed = 0;
	vtbl = tree->vptr;
	flag = 0;
#endif

	if (n == NULL) 
		return -1;
	--n;
	
	if (n->left != NULL) {
		/* find predecessor */
		p = n;
		for (n = n->left; n->right != NULL; n = n->right);

		COPY_NODE(p, n, tree->size);

		p = n->parent;
		if (n == p->left) {
			flag = 1;
			AVL_SetLeft(p, n->left);
		} else
			AVL_SetRight(p, n->left);
		discard = n;
	} else {
		/* no predecessor, left subtree is guaranteed to be NULL */
		p = n->parent;
		if (p != NULL) {
			if (n == p->left) {
				AVL_SetLeft(p, n->right);
				flag = 1;
			} else
				AVL_SetRight(p, n->right);
			DESTRUCT_NODE(n);
			discard = n;
		} else {
			/* root is being deleted */
			p = n->right;
			if (p != NULL)
				p->parent = NULL;
			DESTRUCT_NODE(n);
			ys_deallocate(tree->a, n); 
			/* free(n); */
			tree->root = p;
			return 0;
		}
	}

	/* rebalance */
	height_changed = 1;
	while (p != NULL && height_changed) {
		if (flag || n == p->left) {
			flag = 0;
			p = AVL_RebalanceShorterLeft(p, &height_changed);
		} else
			p = AVL_RebalanceShorterRight(p, &height_changed);
		n = p;
		p = p->parent;
	}
	if (p == NULL)
		tree->root = n;
	ys_deallocate(tree->a, discard); 
	/* free(discard); */
	return 0;
}

/* search for an object */
void           *
AVLTree_Search(AVLTree * tree, void * k)
{
#ifndef __CXC__
	AVLNode        *root = (AVLNode *) tree->root;
	AVL_vtbl       *vtbl = tree->vptr;
#else
	AVLNode        *root;
	AVL_vtbl       *vtbl;

	root = (AVLNode *) tree->root;
	vtbl = tree->vptr;
#endif

	if (root) {
#ifndef __CXC__
		register AVLNode *current = root;
#else
		register AVLNode *current;
		current = root;
#endif

		while (current) {
#ifndef __CXC__
			int             compresult = COMP(k, current);
#else
			int             compresult;
			compresult = COMP(k, current);
#endif
			if (compresult == 0)
				return (void *) (current + 1);
			else if (compresult < 0)
				current = current->left;
			else
				current = current->right;
		}
	}
	return NULL;
}

/* create a new tree */
AVLTree        *
AVLTree_New(AVL_vtbl * vtbl, size_t objsize, size_t growby)
{
	AVLTree        *tree = (AVLTree *) malloc(sizeof(AVLTree));
	if (tree != NULL) {
		tree->root = NULL;
		tree->vptr = vtbl;
		tree->size = objsize;
		tree->n = 0;
		tree->a = ys_new_allocator(objsize + sizeof(AVLNode), growby);
	}
	return tree;
}

void 
AVLTree_Destroy(AVLTree * tree)
{
	if (tree->vptr->destroyobject != NULL) {
		AVLTree_ForwardApply((struct AVLNode_st *)tree->root, tree->vptr->destroyobject);
	}
	ys_destroy_allocator(tree->a);
	free(tree);             /* jul 6 1996 */
}
