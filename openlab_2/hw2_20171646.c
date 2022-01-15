#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define MAX 100
typedef struct {
	float coef;
	int expon;
} polynomial;

polynomial terms[MAX];
int avail = 0;

int compare(int a, int b) {
	if (a > b)
		return 1;
	else if (a < b)
		return -1;
	else
		return 0;
}

void attach(float coefficient, int exponent) {
	if (avail >= MAX) {
		printf("Too many terms in the polynomail\n");
		return;
	}
	terms[avail].coef = coefficient;
	terms[avail].expon = exponent;
	avail++;
}

void padd(int starta, int finisha, int startb, int finishb, int* startd, int* finishd) {
	float coefficient;
	*startd = avail;
	while (starta <= finisha && startb <= finishb) {
		// a와b의 지수를 비교
		switch (compare(terms[starta].expon, terms[startb].expon)) {
			//b의 지수가 클 때
		case -1:
			attach(terms[startb].coef, terms[startb].expon);
			startb++;
			break;
			//지수가 같을 때
		case 0:
			coefficient = terms[starta].coef + terms[startb].coef;
			if (coefficient)
				attach(coefficient, terms[starta].expon);
			starta++; startb++;
			break;
			//a의 지수가 클 때
		case 1:
			attach(terms[starta].coef, terms[starta].expon);
			starta++;
			break;
		}
	}

	//남아있는 수 처리
	for (; starta <= finisha; starta++)
		attach(terms[starta].coef, terms[starta].expon);
	for (; startb <= finishb; startb++)
		attach(terms[startb].coef, terms[startb].expon);
	*finishd = avail - 1;
}

int main() {
	FILE* fpa, * fpb;
	int i = 0;
	int starta = 0, finisha;
	int startb, finishb;
	int startd, finishd;

	fpa = fopen("A.txt", "r");
	fpb = fopen("B.txt", "r");

	// read to "A.txt"
	while(1) {
		if (feof(fpa) != 0)
			break;

		fscanf(fpa, "%f %d", &terms[i].coef, &terms[i].expon);

		i++;
	}

	finisha = i - 1;
	startb = i;

	// read to "B.txt"
	while(1) {

		if (feof(fpb) != 0)
			break;

		fscanf(fpb, "%f %d", &terms[i].coef, &terms[i].expon);

		i++;

	}

	finishb = i - 1;
	avail = i;

	padd(starta, finisha, startb, finishb, &startd, &finishd);

	//출력
	for (i = startd; i <= finishd-1; i++) {
		printf("(%.f,%d), ", terms[i].coef, terms[i].expon);
	}
	printf("(%.f,%d)\n", terms[finishd].coef, terms[finishd].expon);
	
	fclose(fpa);
	fclose(fpb);

	return 0;

}