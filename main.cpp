#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "tree.h"

#define Key_LEFT 75
#define Key_RIGHT 77
#define Key_UP 72
#define Key_DOWN 80
#define Key_SPACE 32
#define mokLength 3
#define SIZE 3

#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int userTurn(int max_x, int max_y, int **gTable, int type);
void play_tic(int size);
void move(int x, int y, int max_x, int max_y);
int place5mok(int x, int y, int **gTable, int size, int type, int vis);
int dumb_comp(int **gTable, int size, int type);
int** init_Table(int size);
treeNode* createTree(int size);
int rtrZeros(int **gTable, int size);
int chkTic(int x, int y, int **gTable, int size, int type);
void play_with_comp(treeNode *root, int size);
treeNode* fnd_lowest(treeNode *root, int type);
void play_first(treeNode *root, int size);

int main() {
	treeNode *root;
	root = createTree(SIZE);
	play_with_comp(root, SIZE);
	//play_first(root, SIZE);
	//play_tic(SIZE);
	_getch();
	return 0;
}
treeNode* fnd_correspond(treeNode *root, int **gTable, int size) {
	int i, j, k;
	int sameness;
	for (i = 0; i < root->num_child; i++) {
		sameness = 0;
		if (root->child[i] == NULL)
			continue;
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				if (gTable[j][k] == root->child[i]->gTable[j][k])
					sameness++;
			}
		}
		if (sameness == size * size)
			return root->child[i];
	}

	return NULL; //no correspond has found.
}

COORD fnd_difference(treeNode *root, treeNode *child, int size) { 
	COORD rtr;
	int i, j;
	rtr.X = 0;
	rtr.Y = 0;
	if (child->num_child - root->num_child > 1) //return value of 0 when the given tree is not its child
		return rtr;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (root->gTable[i][j] != child->gTable[i][j]) {
				rtr.X = j;
				rtr.Y = i;
				return rtr;
			}
		}
	}

	return rtr;
}
treeNode* fnd_highestscore(treeNode *root, int type) {
	int i, max_idx, max, temp;
	COORD rtr;
	max = 0;
	max_idx = -1;


	int min = INT_MAX;
	int min_idx;

	for (i = 0; i < root->num_child; i++) {
		if (root->child[i] == NULL)
			continue;

		/*If an omok is detected. << highest priority*/
		rtr = fnd_difference(root, root->child[i], SIZE);
		if (chkTic(rtr.X * 2, rtr.Y, root->child[i]->gTable, SIZE, type)) {
			return root->child[i];
		}
		/*if an omok for oppose is detected*/
		if (type == 1)
			temp = 2;
		else
			temp = 1;
		if (chkTic(rtr.X * 2, rtr.Y, root->child[i]->gTable, SIZE, temp)) {
			return root->child[i];
		}
		/*find the highest winning chance*/
		if (type == 1) {
			if (root->child[i]->score >= max) { //b_score
				//max = root->child[i]->b_score;
				max = root->child[i]->score;
				max_idx = i;
			}
		}
		else {
			if (root->child[i]->score <= min) { //when white find the lowest
				min = root->child[i]->score;
				min_idx = i;
			}
		}
	}
	if (type == 1) {
		if (max == 0) { //When score for max is zero, the next best choice can be found by the lowest w_Score.
			return fnd_lowest(root, 1);
		}
		if (max_idx != -1)
			return root->child[max_idx];
	}
	else {
		if (min_idx != -1)
			return root->child[min_idx];
	}
	return root->child[0];
}

treeNode* fnd_lowest(treeNode *root, int type) { //find opposite lowest
	int i, min_idx, min;
	min = INT_MAX;
	min_idx = -1;
	for (i = 0; i < root->num_child; i++) {
		if (root->child[i] == NULL)
			continue;
		if (type == 1) {
			if (root->child[i]->w_score < min) {
				min = root->child[i]->w_score;
				min_idx = i;
			}
		}
		else {
			if (root->child[i]->b_score < min) {
				min = root->child[i]->b_score;
				min_idx = i;
			}
		}
	}
	if (min_idx == -1)
		return NULL;
	return root->child[min_idx];
}
void play_first(treeNode *root, int size) {
	int isFin = 0, i, j;
	int **userTable, userResult;
	COORD cord;
	treeNode *updated;
	cord = getCursor();
	userTable = init_Table(size);
	updated = root;
	while (!isFin) {
		
		
		/*user turn*/
		userResult = -1;
		while (userResult == -1) {
			userResult = userTurn(size, size, userTable, 1); //change to 1
			if (userResult == 1) {
				printf("user won.\n");
				isFin = 1;
				return;
			}
		}
		MoveCursor(cord.X, cord.Y);
		/*computer turn*/
		
		updated = fnd_correspond(updated, userTable, size);
		cord = getCursor();
		updated = fnd_highestscore(updated, 2);
		system("cls");
		showGtable(updated->gTable, size);
		cord = fnd_difference(updated->parent, updated, size);
		if (chkTic(cord.X * 2, cord.Y, updated->gTable, size, 2)) {
			printf("computer has won.\n");
			return;
		}
		else if (rtrZeros(updated->gTable, size) == 0) {
			printf("tie.\n");
			return;
		}
		/*copy the gameboard*/
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++)
				userTable[i][j] = updated->gTable[i][j];
		}
	}
	
}
void play_with_comp(treeNode *root, int size) {
	int isFin = 0, i, j;
	int **userTable, userResult;
	
	COORD cord;
	treeNode *updated;

	userTable = init_Table(size);
	updated = root;
	while (!isFin) {
		cord = getCursor();
		/*computer turn*/
		updated = fnd_highestscore(updated, 1);
		//updated = fnd_lowest(updated, 1);
		system("cls");
		showGtable(updated->gTable, size);
		cord = fnd_difference(updated->parent, updated, size);
		if (chkTic(cord.X * 2, cord.Y, updated->gTable, size, 1)) {
			printf("computer has won.\n");
			return;
		}
		else if (rtrZeros(updated->gTable, size) == 0) {
			printf("tie.\n");
			return;
		}
		/*end of computer turn*/

		/*copy the gameboard*/
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++)
				userTable[i][j] = updated->gTable[i][j];
		}

		MoveCursor(cord.X, cord.Y);
		/*user turn*/
		userResult = -1;
		while (userResult == -1) {
			userResult = userTurn(size, size, userTable, 2);
			if (userResult == 1) {
				printf("user won.\n");
				isFin = 1;
			}
		}

		//showGtable(userTable, size);
		/*update find the corresponding treenode*/
		updated = fnd_correspond(updated, userTable, size);
		
	}
	_getch();
}
int rtrZeros(int **gTable, int size) {
	int i, j, cnt;
	cnt = 0;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (gTable[i][j] == 0)
				cnt++;
		}
	}
	return cnt;
}
void showGtable(int **arr, int size) {
	int i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (arr[i][j] == 1)
				printf("¡Ü"); //black
			else if (arr[i][j] == 2)
				printf("¡Û");
			else
				printf("  ");
		}
		printf("\n");
	}
}
treeNode* createTree(int size) {
	DQueue que;
	treeNode * root,  *tNode, *created;
	int **gTable, i, j, k, numZeros, type, cnt_child;
	
	int num = 0;
	/*Initialize table*/
	gTable = init_Table(size);
	InitQueue(&que);
	
	type = 0;
	numZeros = rtrZeros(gTable, size);
	root = CreateNode(gTable, numZeros, size);
	EnQueue(&que, root);
	while (!IsEmpty(&que)) {
		tNode = Peek(&que);
		DeQueue(&que);
		gTable = tNode->gTable;
		cnt_child = 0;
		//type = (type) % 2 + 1;
		for (j = 0; j < size; j++) {
			for (k = 0; k < size; k++) {
				//printf("%d\r", num++);
				if (gTable[j][k] == 0) {
					type = rtrZeros(gTable, size);
					if (type % 2 == 0)
						type = 2;
					else
						type = 1;
					gTable[j][k] = type;

					if (chkTic(k * 2, j, gTable, size, type)) { // if an omok is detected. give the coordinate
						treeNode * temp;
						if (type == 1) {
							tNode->b_score++;
							tNode->score++;
						}
						else {
							tNode->w_score++;
							tNode->score--;
						}
						temp = tNode;
						while (temp->parent != root) {
							if (type == 1) {
								temp->parent->b_score++;
								temp->parent->score++;
								//temp->parent->w_score--;
							}
							else {
								temp->parent->w_score++;
								temp->parent->score--;
								//temp->parent->b_score--;
							}
							temp = temp->parent;
						}
						created = CreateNode(gTable, 0, size); //no need to have zeros since the omok has been found.
						created->child = NULL; //does not have child.
						createSubtree(tNode, created, cnt_child); //enqueue not inserted
					}
					else{
						created = CreateNode(gTable, rtrZeros(gTable, size), size);
						createSubtree(tNode, created, cnt_child);
						EnQueue(&que, created);
					}
					cnt_child++;
					gTable[j][k] = 0;
				}
			}
		}
	}
	
	/*
	InitQueue(&que);
	EnQueue(&que, root);
	while (!IsEmpty(&que)) {
		int sum;
		tNode = Peek(&que);
		DeQueue(&que);
		sum = 0;
		for (i = 0; i < tNode->num_child; i++) {
			
			if (tNode->child[i] != NULL) {
				printf("b:%d w:%d  ", tNode->child[i]->b_score, tNode->child[i]->w_score, tNode->child[i]->score);
				//sum += tNode->child[i]->score;
				EnQueue(&que, tNode->child[i]);
			}
		}
		printf("\n", sum);
	}
	printf("\n%d", num);
	int key = _getch();
	*/
	return root;
}

void play_tic(int size) {
	int isFInished, **gTable;

	int comp_1, comp_2;
	isFInished = 0;


	gTable = init_Table(size);
	srand(time(NULL));
	comp_1 = comp_2 = 0;
	//printf("comp1: %d\ncomp2: %d", comp_1, comp_2);
	clock_t t;
	t = clock();
	while (!isFInished) {
		
		if (userTurn(size, size, gTable, 1)) {
			printf("user won.\n");
			isFInished = 1;
		}
		
		
		/*
		if (dumb_comp(gTable, size, 2)) {
			init_Table(gTable, size);
			MoveCursor(0, 0);
			//printf("comp1: %d\ncomp2: %d", comp_1, comp_2);
			comp_1++;
		}
		if (dumb_comp(gTable, size, 1)) {
			init_Table(gTable, size);
			MoveCursor(0, 0);
			//printf("comp1: %d\ncomp2: %d", comp_1, comp_2);
			comp_2++;
		}
		if (comp_1 + comp_2 >= 10000) {
			MoveCursor(0, 0);
			printf("comp1: %d\ncomp2: %d", comp_1, comp_2);
			isFInished = 1;
			t = clock() - t;
			double taken = ((double)t) / CLOCKS_PER_SEC;
			printf("\nFinished %f seconds.", taken);
			int key = _getch();
		}*/
	}
	
	//printf("comp1: %d comp2: %d\n", comp_1, comp_2);

}
int** init_Table(int size) {

	int **gTable;
	gTable = (int**)malloc(sizeof(int*) * (size + 1));
	for (int i = 0; i < size + 1; i++)
		gTable[i] = (int*)malloc(sizeof(int) * (size + 1));
	for (int i = 0; i < size + 1; i++) {
		for (int j = 0; j < size + 1; j++)
			gTable[i][j] = 0;
	}
	return gTable;
}
int dumb_comp(int **gTable, int size, int type) {
	int cnt = 0;
	int rnd_x, rnd_y, rnd;
	rnd_x = rand() % (size);
	rnd_y = rand() % size;
	while (gTable[rnd_y][rnd_x] != 0) {
		rnd = rand() % 2;
		if (rnd)
			rnd_y = (rnd_y + 1) % size;
		else
			rnd_x = (rnd_x + 1) % (size);
		cnt++;
		if (cnt > size*size)
			return 1;//tie
	}
	return place5mok(rnd_x * 2, rnd_y, gTable, size, type, 0);
}

int chkTic(int x, int y, int **gTable, int size, int type) { //check based on the last position.
	int arr_x, arr_y, cnt;
	int i, j, dir;
	
	arr_x = x / 2;
	arr_y = y;
	///horizontal check
	cnt = 0;
	for (i = 0; i < size; i++) {
		if (gTable[arr_y][i] == type)
			cnt++;
		else
			cnt = 0;
		if (cnt == mokLength) {
			if (i == size - 1)
				return 1;
			else if (gTable[arr_y][i + 1] == 0)
				return 1;
			else
				cnt = 0;
		}
	}
	//vertical check
	cnt = 0;
	for (i = 0; i < size; i++) {
		if (gTable[i][arr_x] == type)
			cnt++;
		else	
			cnt = 0;
		if (cnt == mokLength) {
			if (i == size - 1)
				return 1;
			else if (gTable[i + 1][arr_x] == 0)
				return 1;
			else
				cnt = 0;
		}
	}
	
	//left to right diag check
	cnt = 0;
	//if (arr_y >= mokLength - 1 && arr_x >= mokLength - 1) {
	while (arr_y > 0) {
		if (arr_x >= size - 1)
			break;
		arr_y--;
		arr_x++;
	}
	j = 0;

	for (i = arr_x; i >= 0; i--) {
		if (arr_y + j < size) {
			if (gTable[arr_y + j][arr_x - j] == type) {
				cnt++;
			}
			else
				cnt = 0;
			if (cnt == mokLength) {
				if (i == 0)
					return 1;
				else if (gTable[arr_y + j + 1][arr_x - j - 1] == 0)
					return 1;
				else
					cnt = 0;
			}
		}
		j++;
	}
	
	//right to left diag check
	arr_y = y;
	arr_x = x / 2;
	while (arr_y > 0) {
		if (arr_x <= 0)
			break;
		arr_y--;
		arr_x--;
	}
	j = 0;
	cnt = 0;
	for (i = arr_y; i < size; i++) {
		if (gTable[arr_y + j][arr_x + j] == type)
			cnt++;
		else
			cnt = 0;
		if (cnt == mokLength) {
			if (arr_y + j >= size - 1 || arr_x + j >= size - 1)
				return 1;
			else if (gTable[arr_y + j + 1][arr_x + j + 1] == 0)
				return 1;
			else
				cnt = 0;	
		}
		j++;
	}


	return 0;
}
int place5mok(int x, int y, int **gTable, int size, int type, int vis) { //given as a coordinate
	if (gTable[y][x / 2] == 0) {
		gTable[y][x / 2] = type;
		if (vis == 1) {
			MoveCursor(x, y);
			if (type == 1)
				printf("¡Ü");
			else
				printf("¡Û");
			MoveCursor(x, y);
		}
		return chkTic(x, y, gTable, size, type);
	}
	return 0;
}
int userTurn(int max_x, int max_y, int **gTable, int type) { //return -1 when the input does not change the gameboard

	int key, res;
	res = 0;
	key = _getch();
	if (key == 224) {
		key = _getch();
		switch (key)
		{
		case Key_DOWN:
			move(0, 1, max_x, max_y);
			break;
		case Key_LEFT:
			move(-2, 0, max_x, max_y);
			break;
		case Key_RIGHT:
			move(+2, 0, max_x, max_y);
			break;
		case Key_UP:
			move(0, -1, max_x, max_y);
		default:
			break;
		}
		return -1;
	}
	else {
		if (key = Key_SPACE) {
			COORD temp;
			temp = getCursor();
			res = place5mok(temp.X, temp.Y, gTable, max_x, type, 1);
		}
		else
			return -1; //when the input does not change the output
	}

	return res;
}
void move(int x, int y, int max_x, int max_y) {
	COORD cursor;
	cursor = getCursor();
	if ((cursor.X + x) / 2 < max_x && cursor.Y + y < max_y)
		MoveCursor(cursor.X + x, cursor.Y + y);
}