#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"

void itemInput(FILE* ifp, item* itemList) {
	/*item.txt to itemList*/
	int i,j;
	double a, s, h, c;
	item* temp;
	itemList->link = NULL;
	char* iname;
	char tempname='a';
	char trash[20];
	for (i = 0; i < 20; i++) {
		temp = itemList;
		while (temp->link != NULL) {
			temp = temp->link;
		}
		iname = (char*)malloc(sizeof(char) * 20);
		j = 0;
		fgets(trash, sizeof(trash), ifp);
		while (tempname != '\n') {
			fscanf(ifp, "%c", &tempname);
			if (tempname != '\n') {
				iname[j] = tempname;
				j++;
			}
		}
		iname[j] = '\0';
		tempname = 'a';
		fscanf(ifp, "%lf %lf %lf %lf", &a, &s, &h, &c);
		item* newitem = (item*)malloc(sizeof(item));
		newitem->name = iname;
		newitem->s.atk = a;
		newitem->s.speed = s;
		newitem->s.HP = h;
		newitem->s.CHP = c;
		newitem->link = NULL;
		temp->link = newitem;
	}

	free(iname);

}

skill* skillInput(FILE* sfp) {
	/*make skillList as a tree.*/
	int skillsize;
	char* sname = NULL;
	char trash[20];
	char temp = 'a';
	double a, s, h, c;
	int j;
	fscanf(sfp, "%d", &skillsize);
	skill* skillList = (skill*)malloc(sizeof(skill) * skillsize);
	for (int i = 0; i < skillsize; i++) {
		sname = (char*)malloc(sizeof(char) * 20);
		j = 0;
		fgets(trash, sizeof(trash), sfp);
		while (temp != '\n') {
			fscanf(sfp, "%c", &temp);
			if (temp != '\n') {
				sname[j] = temp;
				j++;
			}
		}
		sname[j] = '\0';
		temp = 'a';
		fscanf(sfp, "%lf %lf %lf %lf", &a, &s, &h, &c);
		skillList[i].name = sname;
		skillList[i].s.atk = a;
		skillList[i].s.speed = s;
		skillList[i].s.HP = h;
		skillList[i].s.CHP = c;
	}

	free(sname);
		

	return skillList;
}


void getItem(player *p, item* itemList) {
	/*Give random item to player*/
	int assign, a;
	item* i = (item*)malloc(sizeof(item));
	item* temp = itemList;
	srand(time(0));
	assign = rand() % 19;

	for (a = 0; a <= assign; a++) {
		temp = temp->link;
	}
	
	printf("==============================================================================\n");
	printf("You get the %s!!\n\tatk : %.2lf\tspd : %.2lf\tHP : %.2lf\n", temp->name, temp->s.atk, temp->s.speed, temp->s.HP);
	printf("==============================================================================\n");
	p->s.HP += temp->s.HP;
	p->s.atk += temp->s.atk;
	p->s.speed += temp->s.speed;
	p->s.CHP += temp->s.CHP;
	 
	i->s.atk = temp->s.atk;
	i->s.HP = temp->s.HP;
	i->s.CHP = temp->s.CHP;
	i->s.speed = temp->s.speed;
	i->name = temp->name;

	if (p->pItem == NULL) {
		p->pItem = i;
		p->pItem->link = NULL;
		return;
	}
	else {
		temp = p->pItem;
		while (temp->link != NULL) {
			temp = temp->link;
		}
		temp->link = i;
		i->link = NULL;
	}

}

void getSkill(player *p, skill* skillList) {
	/*Assign skill to player, and level up proccess*/
	p->s.level++;
	int node = p->skillNumber;
	int choose = 3;
	int leftchild = node * 2 +1;
	int rightchild = node * 2 + 2;

	if (p->s.level < 5) {
		printf("Choose Skill\n");
		printf("1 : %s\n", skillList[leftchild].name);
		printf("2 : %s\n", skillList[rightchild].name);
		while (!(choose == 1 || choose == 2)) {
			scanf("%d", &choose);
			if (!(choose != 1 || choose != 2))
				printf("Please choose 1 or 2\n");
		}
		if (choose == 1) {
			p->pSkill[p->s.level - 1] = skillList[leftchild];
			p->skillNumber = leftchild;
			printf("You learn %s\n", skillList[leftchild].name);
		}
		else if (choose == 2) {
			p->pSkill[p->s.level - 1] = skillList[rightchild];
			p->skillNumber = rightchild;
			printf("\tYou learn %s\n", skillList[rightchild].name);
		}
	}

	else
		printf("\n\nskill master.\n\n\n");
	p->s.atk = p->s.atk * 1.5;
	p->s.HP = p->s.HP * 1.5;
	p->s.speed = p->s.speed * 1.5;
	p->s.CHP = p->s.HP;

}

void showSkill(player* p) {
	//Show skill list
	int i;
	int k;
	if (p->s.level < 5) {
		k = p->s.level;
	}
	else
		k = 4;
		printf("\n\n<<<Learned Skill>>>\n");
		for (i = 0; i < k; i++) {
			printf("%d : \t%s\n", i, p->pSkill[i].name);
			printf("damage : %.1f\n", p->pSkill[i].s.atk);
			printf("cost : %.1f\n\n", p->pSkill[i].s.speed);
			
		}
}
void showItem(player* p) {
	//show item list
	int i;
	item* temp;
	temp = p->pItem;
	if (temp->link == NULL)
		printf("No item\n");
	else {
		printf("\n\n<<<Item List>>>\n");
		temp = temp->link;
		while (temp != NULL) {
			printf("%s\n", temp->name);
			temp = temp->link;
		}
	}
}

void playerInit(player* p, skill* s) {
	/*initiate player stat.*/
	p->s.atk = 10;
	p->s.HP = 100;
	p->s.CHP = 100;
	p->s.level = 1;
	p->s.speed = 10;

	p->pItem = (item*)malloc(sizeof(item));
	p->pItem->link = NULL;
	p->pSkill = (skill*)malloc(sizeof(skill) * 5);
	p->pSkill[0] = s[0];
	p->skillNumber = 0;

}
void printPlayer(player* p) {
	/*print player info*/
	printf("==============player's status==============\n");
	printf("level : %d\n", p->s.level);
	printf("attack : %.1f\n", p->s.atk);
	printf("HP : %.1f\n", p->s.HP);
	printf("speed : %.1f\n", p->s.speed);
	printf("============================================\n\n");
}

