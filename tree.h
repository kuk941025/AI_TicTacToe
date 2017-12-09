#ifndef _TREE_H_
#define _TREE_H_

typedef int** tData;


typedef struct _treeNode {
	tData gTable;
	int num_child;
	int w_score, b_score, score;
	struct _treeNode **child;
	struct _treeNode *parent;
}treeNode;


treeNode * CreateNode(tData item, int num_child, int size);
void DestroyNode(treeNode * node);

void createSubtree(treeNode * root, treeNode *child, int n_child);

#endif _TREE_H_


#ifndef _DYNAMIC_QUEUE_H
#define _DYNAMIC_QUEUE_H

typedef treeNode* Data;

typedef struct _Node
{
	Data item;
	struct _Node* next;
} Node;

typedef struct
{
	Node* front;
	Node* rear;
} DQueue;


// Make a queue empty.
void InitQueue(DQueue *pqueue);
// Check whether a queue is empty.
bool IsEmpty(DQueue *pqueue);

// Read the item at the front.
Data Peek(DQueue *pqueue);
// Insert an item at the rear.
void EnQueue(DQueue *pqueue, Data item);
// Delete an item at the front.
void DeQueue(DQueue *pqueue);

#endif _DYNAMIC_QUEUE_H