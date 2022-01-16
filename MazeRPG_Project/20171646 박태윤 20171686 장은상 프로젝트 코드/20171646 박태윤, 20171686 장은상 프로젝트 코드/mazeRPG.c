#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "player.h"
#include "monster.h"


void main() {
	FILE* mfp = fopen("MAZE.txt","r");
	FILE* ifp = fopen("item.txt", "r");
	FILE* sfp = fopen("skill.txt", "r");
	int sizeOfMaze;
	int **field = NULL;
	int can[4];

	fscanf(mfp, "%d", &sizeOfMaze);

	field = makeMaze(mfp, sizeOfMaze);
	item* itemList = (item*)malloc(sizeof(item));
	skill* skillList = NULL;
	monster* monsterList = (monster*)malloc(sizeof(monster));
	player *p = (player*)malloc(sizeof(player));
	itemInput(ifp, itemList);
	skillList = skillInput(sfp);
	int currentRow = 1, currentCol = 1;
	int res=0;
	playerInit(p, skillList);

	//game play
	while (1) {

		if (p->s.CHP <= 0) {
			printf("\n------------------------------------\n");
			printf("            Game Over!!!!!!         \n");
			printf("------------------------------------\n");
			break;
		}
		
		//move
		if (field[currentRow][currentCol] == 0) {
			move(field, &currentRow, &currentCol, can, p);
		}
		//monster
		else if (field[currentRow][currentCol] == 2) {
			//randomly monster
			pushMonster(p->s.level, monsterList);
			res = battle(p, monsterList);
			field[currentRow][currentCol] = 0;
			if (res == 1) {
				//assign skill
				getSkill(p, skillList);
			}
		}
		//item
		else if (field[currentRow][currentCol] == 4) {
			//randomly item
			getItem(p,itemList);
			field[currentRow][currentCol] = 0;
		}
		//boss
		else if (field[currentRow][currentCol] == 3) {
			pushMonster(p->s.level, monsterList);
			res = battle(p, monsterList);

			printPlayer(p);
			printMonster(monsterList);
			printField(field, sizeOfMaze);

			if (res == 1) {
				/*엔딩*/
				printf("\n------------------------------------\n");
				printf("            clear!!!!!!             \n");
				printf("------------------------------------\n");
				break;
			}

			else if(res == -1){
				/*배드엔딩*/
				printf("\n------------------------------------\n");
				printf("            SOOO Close                \n");
				printf("            Try Again!!!!            \n");
				printf("------------------------------------\n");
				break;
			}
		}
		
	}

	fclose(mfp); fclose(ifp); fclose(sfp);

	for (int i = 0; i < sizeOfMaze; i++) {
		free(field[i]);
	}
	free(field);

	item *x,*y;
	y = itemList;

	while (y != NULL) {
		x = y;
		y = y->link;
		free(x);
	}

	free(p->pSkill);
	free(skillList);

	y = p->pItem;

	while (y != NULL) {
		x = y;
		y = y->link;
		free(x);
	}


	free(p);
}

