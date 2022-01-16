#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdio.h>

//status
typedef struct _status {
	double atk;
	double speed;
	double HP;
	double CHP;
	int level;
} stat;

typedef struct _item {
	stat s;
	char* name;
	struct _item* link;
} item;

typedef struct _skill {
	stat s;
	char* name;
} skill;

typedef struct _player {
	stat s;
	//itemlist
	item* pItem;
	//skill list
	skill* pSkill;
	int skillNumber;
} player;

//text file to itemList.
void itemInput(FILE* ifp, item* itemList);
//text file to skillList.
skill* skillInput(FILE* sfp);
//assign one item to player.
void getItem(player* p, item* itemList);
//assign one skill to player, level up.
void getSkill(player* p, skill* skillList);


//show skill list
void showSkill(player* p);
//show item list
void showItem(player* p);
//initiate player stat
void playerInit(player*, skill*);
//print player info.
void printPlayer(player* p);


#endif
