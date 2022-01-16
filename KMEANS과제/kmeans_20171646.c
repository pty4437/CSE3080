#define _CRT_SECURE_NO_WARNINGS
#define MAX_OF_FEATURES 100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _node {
	struct _node* link;
	double data[MAX_OF_FEATURES];
} Node;

//�������� ����, feature�� ����, Centroid�� ����
int numOfData, numOfFeature, K;

void changeList(Node**);
double calDistance(Node*, Node*);
double calAverage(Node*, int);
int calCentroid(Node**);
void printList(Node**);
void makeListFirst(Node**, double[]);
void freeList(Node** list);

//��忡 ����Ǿ��ִ� �����͸� ���� �� �� ������ �Ÿ��� ���Ѵ�.
double calDistance(Node* list, Node* tNode) {
	double min = 0;

	for (int m = 0; m < numOfFeature; m++) {
		min += pow(list->data[m] - tNode->data[m], 2.0);
	}

	return sqrt(min);
}

//Centroid�� ������ ���� �� ������ ����Ǿ��ִ� Centroid���� �Ÿ��� ����� Centroid�� ����� �ش� Centroid�� ��带 �������ش�.
void changeList(Node** list) {
	Node* tmpNode;
	Node* prevNode;
	double min = 0;
	double temp = 0;
	int index;

	for (int i = 0; i < K; i++) {
		tmpNode = list[i]->link;
		prevNode = list[i];
		index = i;

		while (tmpNode != NULL) {
			min = -1;

			//���� ����� Centroid�� index���ϱ�
			for (int j = 0; j < K; j++) {
				temp = calDistance(list[j], tmpNode);
				if (min > temp || min < 0) {
					index = j;
					min = temp;
				}

			}

			//����� �̵��� �ʿ��� ���
			if (index != i) {
				prevNode->link = tmpNode->link;
				tmpNode->link = list[index]->link;
				list[index]->link = tmpNode;
				tmpNode = prevNode->link;
			}

			//�׷��� ���� ���
			else {
				prevNode = tmpNode;
				tmpNode = tmpNode->link;
			}
		}
	}
}

//����Ǿ��ִ� ����� ��� ��ǥ���� ������ش�.
double calAverage(Node* list, int index) {
	Node* tmp = list->link;
	double count = 0;
	double sum = 0;
	double res = 0;

	while (tmp != NULL) {
		sum += tmp->data[index];
		tmp = tmp->link;
		count++;
	}

	res = sum / count;
	
	return res;
}

//Centroid�� �����͸� ����Ǿ��ִ� ����� ��� ��ǥ�� �ٲ��ش�.
int calCentroid(Node** list) {
	/*judge�� Centroid��ǥ�� �̵��� �Ͼ������ �Ǻ����ִ� �����̴�. 
	�״�� 0�� ��ȯ�� ��� �̵��� �Ͼ�� ���� ����̹Ƿ� main�Լ����� ȣ�� �� �̸� ����Ͽ� Iteration�� �����Ų��.*/
	int judge = 0;
	double num = 0;

	for (int i = 0; i < K; i++) {
		for (int j = 0; j < numOfFeature; j++) {
			if (list[i]->link != NULL) {
				num = calAverage(list[i], j);

				//���� Centroid��ǥ�� ����Ǿ��ִ� ����� ��� ��ǥ�� �ٸ� ��� �̵�
				if (num != list[i]->data[j]) {
					judge = 1;
					list[i]->data[j] = num;
				}
			}
		}

	}

	return judge;
}

//output.txt���Ͽ� ����Ѵ�.
void printList(Node** list) {
	FILE* out = fopen("output.txt","w");
	Node* temp;

	for (int i = 0; i < K; i++) {
		fprintf(out,"%d\n", i);
		temp = list[i]->link;

		while (temp != NULL) {
			for (int j = 0; j < numOfFeature; j++) {
				fprintf(out,"%.10f ", temp->data[j]);
			}
			fprintf(out,"\n");
			temp = temp->link;
		}
	}
	fclose(out);
}

//ó���� �����Ͱ� ����Ǿ��ִ� arr�迭�� ������ list�� ��带 �����Ͽ� �־��ش�.
void makeListFirst(Node** list, double arr[]) {
	Node* tempNode = (Node*)malloc(sizeof(Node));
	double min = 0, tmp = 0;
	int index = 0;

	for (int i = 0; i < numOfFeature; i++) {
		tempNode->data[i] = arr[i];
	}

	min = calDistance(list[0], tempNode);

	for (int j = 1; j < K; j++) {
		tmp = calDistance(list[j], tempNode);

		if (tmp < min) {
			index = j;
			min = tmp;
		}
	}

	if (list[index]->link == NULL) {
		list[index]->link = tempNode;
		tempNode->link = NULL;
	}

	else {
		tempNode->link = list[index]->link;
		list[index]->link = tempNode;
	}

}

//����Ʈ �޸� ������ ���ش�.
void freeList(Node** list) {
	Node* curNode;
	Node* delNode;

	for (int i = 0; i < K; i++) {
		curNode = list[i]->link;

		while (curNode != NULL) {
			delNode = curNode;
			curNode = curNode->link;
			free(delNode);
		}

		free(list[i]);
	}
}


int main() {
	srand(1000);
	Node** list;
	double *arr;
	int count = 1;
	FILE* fp = fopen("input.txt", "r");

	fscanf(fp, "%d %d %d", &numOfData, &numOfFeature, &K);

	list = (Node**)malloc(sizeof(Node*) * K);

	//centroid���Ƿ� ����
	for (int i = 0; i < K; i++) {
		list[i] = (Node*)malloc(sizeof(Node));

		for (int j = 0; j < numOfFeature; j++) {
			list[i]->data[j] = (double)(rand()) / RAND_MAX;
		}

		list[i]->link = NULL;
	}

	arr = (double*)malloc(sizeof(double) * numOfFeature);

	//arr�迭�� input.txt���Ͽ� �ִ� ������ ������ ��� ����־��ش�.
	for (int i = 0; i < numOfData; i++) {
		for (int j = 0; j < numOfFeature; j++) {
			fscanf(fp, "%lf", &arr[j]);
		}

		makeListFirst(list, arr);
	}


	while (count <= 50) {
		if (calCentroid(list) == 0)
			break;
		changeList(list);
		count++;
	}

	printList(list);


	fclose(fp);
	freeList(list);
	free(arr);

	return 0;
}

