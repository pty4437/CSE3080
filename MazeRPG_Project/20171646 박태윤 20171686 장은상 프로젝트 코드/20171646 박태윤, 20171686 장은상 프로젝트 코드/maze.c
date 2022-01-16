#define _CRT_SECURE_NO_WARNINGS

#include "maze.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>


#define LEFT 97
#define RIGHT 100
#define UP 119
#define DOWN 115

int getch(void){
	char ch;
	int error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0,&Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;
#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif
	if(0 == (error = tcsetattr(0, FLAG, &Ntty))){
		error = read(0, &ch, 1);
		error += tcsetattr(0,FLAG, &Otty);
	}
	return (error == 1 ? (int)ch : -1);
}



int** makeMaze(FILE* mfp, int sizeOfMaze) {
	/*maze.txt to field */

	int** field = (int**)malloc(sizeof(int*) * sizeOfMaze);
 
    for (int j = 0; j < sizeOfMaze; j++)
        field[j] = (int*)malloc(sizeof(int) * sizeOfMaze);

	for (int i = 0; i < sizeOfMaze; i++) {
		for (int j = 0; j < sizeOfMaze; j++) {
			fscanf(mfp, "%d", &field[i][j]);
		}
	}
    return field;
}

void move(int** field, int* currentRow, int* currentCol, int can[], player* p) {
	/*Get command and move to up down left right 'i'show itemlist, 'k' show skilllist.*/
	/*when get i and k do not shut down function*/

    int c;

    can[0] = field[*currentRow - 1][*currentCol];
    can[1] = field[*currentRow + 1][*currentCol];
    can[2] = field[*currentRow][*currentCol-1];
    can[3] = field[*currentRow][*currentCol+1];           
                c = getch();
                switch (c) {
                case LEFT:
                    printf("move left\n");
                    if (can[2] != 1) {
                        (*currentCol)--;
                    }
                    else {
                        printf("can't move.\n");
                        move(field, currentRow, currentCol, can, p);
                    }
                    break;
                case RIGHT:
                    printf("move right\n");
                    if (can[3] != 1) {
                        (*currentCol)++;
                    }
                    else {
                        printf("can't move.\n");
                        move(field, currentRow, currentCol, can, p);
                    }
                    break;
                case UP:
                    printf("move up\n");
                    if (can[0] != 1) {
                        (*currentRow)--;
                    }
                    else {
                        printf("can't move.\n");
                        move(field, currentRow, currentCol, can, p);
                    }
                    break;
                case DOWN:
                    printf("move down\n");
                    if (can[1] != 1) {
                        (*currentRow)++;
                    }
                    else {
                        printf("can't move.\n");
                        move(field, currentRow, currentCol, can, p);
                    }
                    break;
                case 'i':
                    showItem(p);
                    move(field, currentRow, currentCol, can, p);
                    break;
                case 'k':
                    showSkill(p);
                    move(field, currentRow, currentCol, can, p);
                    break;
                case 'm':
                    showMap(field, currentRow, currentCol);
                    move(field, currentRow, currentCol, can, p);
                    break;
                }
               
}
void showMap(int** field,int* cr, int* cc) {

    for (int i = 0; i <= *cr; i++) {
        for (int j = 0; j <= *cc; j++) {
            printf("%d ", field[i][j]);
        }
        printf("\n");
    }
}
int battle(player* p, monster* monsterList) {
	/*Do battle. head of monsterlist have facing monster information.*/
	/*win return 1 lose return -1*/
    monster* M = monsterList->link;
    skill use;
    double pcs = p->s.speed, mcs = M->speed;

    printf("========battle!!!========\n\n");
    printPlayer(p);

    while (1) {
		int c;
        //case of lose
        if (p->s.CHP <= 0)
            return -1;
        //case of win
        else if (M->CHP <= 0) {
            printf("win!!!!!!!!!!!!!!\n\n\n");
            printf("============================================\n");
            return 1;
        }
        //battle
        else {
            if (pcs >= mcs) {
                printf("============================================\n");
                printf("==============player's turn!!!==============\n");
                printf("============================================\n");
                //player attack chance
                showSkill(p);
                printf("Which skill do you want to use??\n");
                scanf("%d", &c);
                if (c >= p->s.level || (c<0 && c>9)) {
                    printf("\nWrong command.\n\n");
                    continue;
                }
                printf("\n\n");

                use = p->pSkill[c];

                if (use.s.CHP == -1) {
                    M->CHP = M->CHP - (use.s.atk + p->s.atk);
                    mcs = mcs + use.s.speed + M->speed;

                    printf("damage : %.1f\n", use.s.atk + p->s.atk);
                    printf("player's HP    monster's HP\n");
                    printf("     %.1f         %.1f    \n\n\n", p->s.CHP, M->CHP);
                }
                else {
                    if (use.s.HP == 1) {
                        if (p->s.CHP + p->s.HP / 5 > p->s.HP) {
                            p->s.CHP = p->s.HP;
                        }
                        else
                        p->s.CHP += p->s.HP / 5;

                        mcs = mcs + use.s.speed + M->speed;

                        printf("player's HP    monster's HP\n");
                        printf("     %.1f         %.1f    \n\n\n", p->s.CHP, M->CHP);
                    }
                    else if (use.s.HP == 2) {
                        if (p->s.CHP + p->s.HP / 3 > p->s.HP) {
                            p->s.CHP = p->s.HP;
                        }
                        else
                        p->s.CHP += p->s.HP / 3;

                        mcs = mcs + use.s.speed + M->speed;

                        printf("player's HP    monster's HP\n");
                        printf("     %.1f         %.1f    \n\n\n", p->s.CHP, M->CHP);
                    }
                    else {
                        
                        if (p->s.CHP + p->s.HP / 2 > p->s.HP) {
                            p->s.CHP = p->s.HP;
                        }
                        else
                        p->s.CHP += p->s.HP / 2;

                        mcs = mcs + use.s.speed + M->speed;

                        printf("player's HP    monster's HP\n\n");
                        printf("     %.1f         %.1f    \n\n", p->s.CHP, M->CHP);
                    }
                }
            }
            else {
                //Monster attack chance
                printf("\n============================================\n");
                printf("==============monster's turn!!!=============\n");
                printf("============================================\n");

                p->s.CHP -= M->atk;
                pcs += p->s.speed;

                printf("damage : %f\n", M->atk);
                printf("player's HP    monster's HP\n\n");
                printf("    %.1f         %.1f    \n\n", p->s.CHP, M->CHP);
            }
        }
    }
}

void printField(int** field, int sizeOfMaze) {
	/*print field.*/
    for (int i = 0; i < sizeOfMaze; i++) {
        for (int j = 0; j < sizeOfMaze; j++)
            printf("%d ", field[i][j]);

        printf("\n");
    }
}
