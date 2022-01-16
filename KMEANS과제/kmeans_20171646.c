#define _CRT_SECURE_NO_WARNINGS
#define MAX_OF_FEATURES 100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _node {
	struct _node* link;
	double data[MAX_OF_FEATURES];
} Node;

//데이터의 개수, feature의 개수, Centroid의 개수
int numOfData, numOfFeature, K;

void changeList(Node**);
double calDistance(Node*, Node*);
double calAverage(Node*, int);
int calCentroid(Node**);
void printList(Node**);
void makeListFirst(Node**, double[]);
void freeList(Node** list);

//노드에 저장되어있는 데이터를 통해 두 점 사이의 거리를 구한다.
double calDistance(Node* list, Node* tNode) {
	double min = 0;

	for (int m = 0; m < numOfFeature; m++) {
		min += pow(list->data[m] - tNode->data[m], 2.0);
	}

	return sqrt(min);
}

//Centroid의 데이터 변경 후 기존에 연결되어있던 Centroid보다 거리가 가까운 Centroid가 생기면 해당 Centroid로 노드를 연결해준다.
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

			//가장 가까운 Centroid의 index구하기
			for (int j = 0; j < K; j++) {
				temp = calDistance(list[j], tmpNode);
				if (min > temp || min < 0) {
					index = j;
					min = temp;
				}

			}

			//노드의 이동이 필요할 경우
			if (index != i) {
				prevNode->link = tmpNode->link;
				tmpNode->link = list[index]->link;
				list[index]->link = tmpNode;
				tmpNode = prevNode->link;
			}

			//그렇지 않을 경우
			else {
				prevNode = tmpNode;
				tmpNode = tmpNode->link;
			}
		}
	}
}

//연결되어있는 노드의 평균 좌표값을 계산해준다.
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

//Centroid의 데이터를 연결되어있는 노드의 평균 좌표로 바꿔준다.
int calCentroid(Node** list) {
	/*judge는 Centroid좌표의 이동이 일어났는지를 판별해주는 변수이다. 
	그대로 0을 반환할 경우 이동이 일어나지 않은 경우이므로 main함수에서 호출 시 이를 고려하여 Iteration을 종료시킨다.*/
	int judge = 0;
	double num = 0;

	for (int i = 0; i < K; i++) {
		for (int j = 0; j < numOfFeature; j++) {
			if (list[i]->link != NULL) {
				num = calAverage(list[i], j);

				//현재 Centroid좌표랑 연결되어있는 노드의 평균 좌표랑 다를 경우 이동
				if (num != list[i]->data[j]) {
					judge = 1;
					list[i]->data[j] = num;
				}
			}
		}

	}

	return judge;
}

//output.txt파일에 출력한다.
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

//처음에 데이터가 저장되어있는 arr배열을 가지고 list에 노드를 생성하여 넣어준다.
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

//리스트 메모리 해제를 해준다.
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

	//centroid임의로 설정
	for (int i = 0; i < K; i++) {
		list[i] = (Node*)malloc(sizeof(Node));

		for (int j = 0; j < numOfFeature; j++) {
			list[i]->data[j] = (double)(rand()) / RAND_MAX;
		}

		list[i]->link = NULL;
	}

	arr = (double*)malloc(sizeof(double) * numOfFeature);

	//arr배열에 input.txt파일에 있는 데이터 값들을 모두 집어넣어준다.
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

