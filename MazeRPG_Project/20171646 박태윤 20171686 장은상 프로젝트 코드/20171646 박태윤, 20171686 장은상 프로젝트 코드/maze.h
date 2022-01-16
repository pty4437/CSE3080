#ifndef __MAZE_H__
#define __MAZE_H__
#include <stdio.h>
#include "player.h"
#include "monster.h"

typedef struct entry_node {
	int row;
	int col;
}entry_node;

typedef struct _track {
	struct _track* right;
	struct _track* down;
	union {
		struct _track* next;
		entry_node entry;
	}u;
} Track;

//텍스트파일에 있는 미로를  field배열에 넣어준다.
int** makeMaze(FILE* mfp, int sizeOfMaze);
//이동시킨다.
void move(int** , int* , int* , int[], player*);
//battle한다. 플레이어가 이기면 0반환, 지면 -1을 반환한다.
int battle(player* p, monster* monsterList);
//field를 출력해준다.
void printField(int** field, int);
void showMap(int**, int*, int*);

#endif
