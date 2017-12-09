#include <stdio.h>
#include <Windows.h>

void showCursor(int visibility);
void MoveCursor(int x, int y);
COORD getCursor();
int** init_Table(int size);
void showGtable(int **arr, int size);