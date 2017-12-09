#include "tree.h"
#include "main.h"



treeNode * CreateNode(tData item, int num_child, int size) {
	int i, j;
	treeNode * tNode = (treeNode*)malloc(sizeof(treeNode));
	tNode->gTable = init_Table(size);
	/*
	tNode->gTable = (int**)malloc(sizeof(int*) * (size + 4));
	for (i = 0; i < size; i++)
		tNode->gTable[i] = (int*)malloc(sizeof(int) * (size + 4));
		*/
	for (i = 0; i < size + 1; i++)
		for (j = 0; j < size + 1; j++)
			tNode->gTable[i][j] = item[i][j];
	
	tNode->num_child = num_child;
	tNode->child = (treeNode**)malloc(sizeof(treeNode*) * num_child);
	for (i = 0; i < num_child; i++)
		tNode->child[i] = NULL;

	tNode->parent = (treeNode*)malloc(sizeof(treeNode));
	tNode->parent = NULL;

	tNode->w_score = tNode->b_score = tNode->score = 0;

	return tNode;
}

void DestroyNode(treeNode * node) {

}

void createSubtree(treeNode * root, treeNode *child, int n_child) {
	if (root->child[n_child] != NULL)
		printf("Already have a child. Failed\n");
	if (child != NULL)
		child->parent = root;
	root->child[n_child] = child;
	
}




void InitQueue(DQueue *pqueue)
{
	pqueue->front = pqueue->rear = NULL;
}
// Check whether a queue is empty.
bool IsEmpty(DQueue *pqueue)
{
	return pqueue->front == NULL;
}

// Read the item at the front.
Data Peek(DQueue *pqueue)
{
	if (IsEmpty(pqueue)) exit(1);

	return pqueue->front->item;
}

// Insert an item at the rear.
void EnQueue(DQueue *pqueue, Data item)
{
	Node* newNode = (Node *)malloc(sizeof(Node));
	newNode->item = item;
	newNode->next = NULL;
	if (IsEmpty(pqueue)) {
		pqueue->front = pqueue->rear = newNode;
	}
	else {
		pqueue->rear->next = newNode;
		pqueue->rear = newNode;
	}
}

// Delete an item at the front.
void DeQueue(DQueue *pqueue)
{
	Node* temp;
	if (IsEmpty(pqueue)) exit(1);

	temp = pqueue->front;
	if (temp->next == NULL)
		pqueue->front = pqueue->rear = NULL;
	else
		pqueue->front = temp->next;

	free(temp);
}




