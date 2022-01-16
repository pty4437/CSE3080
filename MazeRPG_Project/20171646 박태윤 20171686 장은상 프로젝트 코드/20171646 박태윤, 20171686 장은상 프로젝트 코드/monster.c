#include <stdio.h>
#include <stdlib.h>
#include "monster.h"


void pushMonster(int level, monster* monsterList) {
	/*add monster to stack accoring to player level*/
	monster* pMonster = (monster*)malloc(sizeof(monster));
	pMonster->atk = level * 9;
	pMonster->HP = -20 + level * 60;
	pMonster->CHP = -20 + level * 60;
	pMonster->speed = level * 9;

	if (level == 1) {
		monsterList->link = pMonster;
		pMonster->link = NULL;
	}
	else {
		pMonster->link = monsterList->link;
		monsterList->link = pMonster;
	}
}

int popMonster(monster* monsterList) {
	/*monsterList stack Pop and return monster*/
	monster* dMonster;
	int mLevel;

	if (monsterList->link != NULL) {
		dMonster = monsterList->link;
		monsterList->link = dMonster->link;
		mLevel = (int)dMonster->atk / 9;
		free(dMonster);
		return mLevel;
	}
	else
		return -1;
}

void printMonster(monster* monsterList) {
	/*print monster stack*/
	/*get return of popMonster to print*/
	int res;

	while (1) {
		res = popMonster(monsterList);

		if (res == -1)
			break;
		else {
			printf("The number : %d\n", res);
			printf("Attack : %d\n", res * 3);
			printf("HP : %d\n", res * 9);
			printf("Speed : %d\n\n\n", res * 9);
		}
	}
}
