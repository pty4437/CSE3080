#ifndef __MONSTER_H__
#define __MONSTER_H__

typedef struct _monster {
	double atk;
	double speed;
	double HP;
	double CHP;
	struct _monster* link;
} monster;

//push monster list stack.
void pushMonster(int level, monster* monsterList);
//pop monster list stack.
int popMonster(monster* monsterList);
//print monster info.
void printMonster(monster* monsterList);

#endif