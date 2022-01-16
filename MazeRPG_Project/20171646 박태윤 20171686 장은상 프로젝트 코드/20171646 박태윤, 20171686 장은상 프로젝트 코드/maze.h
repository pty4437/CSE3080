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

//�ؽ�Ʈ���Ͽ� �ִ� �̷θ�  field�迭�� �־��ش�.
int** makeMaze(FILE* mfp, int sizeOfMaze);
//�̵���Ų��.
void move(int** , int* , int* , int[], player*);
//battle�Ѵ�. �÷��̾ �̱�� 0��ȯ, ���� -1�� ��ȯ�Ѵ�.
int battle(player* p, monster* monsterList);
//field�� ������ش�.
void printField(int** field, int);
void showMap(int**, int*, int*);

#endif
