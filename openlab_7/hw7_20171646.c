#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 30
#define MIN2(x,y) ((x) < (y) ? (x) : (y))

typedef struct _node {
	int vertex;
	struct _node* link;
} node;

typedef struct _stack {
	int x;
	int y;
	struct _stack* link;
} Stack;

node* graph[MAX_VERTICES];
Stack* stack;

short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];

int num;
int numOfV;

void init();
void bicon(int, int);
void makeGraph(node*, int);
void add(int, int);
void delete(int*, int*);

void main()
{
	node* delnode;
	node* tmp;

	init();

	bicon(3, -1);

	for (int i = 0; i < numOfV; i++) {
		tmp = graph[i]->link;
		while (tmp != NULL) {
			delnode = tmp;
			tmp = tmp->link;
			free(delnode);
		}
	}
}

void init() {
	int i,j;
	char arr[1000];
	FILE* fp = fopen("input.txt", "r");
	int N = 0, count = 0;

	stack = (Stack*)malloc(sizeof(Stack));
	stack->link = NULL;

	fscanf(fp, "%d", &numOfV);

	for (i = 0; i < numOfV; i++) {
		graph[i] = (node*)malloc(sizeof(node));
		graph[i]->link = NULL;
		graph[i]->vertex = i;
	}

	fgets(arr, 1000, fp);

	for (i = 0; i < numOfV; i++) {
		fgets(arr, 1000, fp);
		count = 0;
		N = 0;
		for (j = 0;; j++) {
			if (arr[j] >= '0' && arr[j] <= '9') {
				N = (N * 10) + (arr[j] - '0');
			}

			else if (arr[j] == ' ' || arr[j] == '\n' || arr[j] == '\0') {
				if (count == 0) {
					count = 1;
					N = 0;
				}
				else {
					makeGraph(graph[i], N);
					N = 0;
				}
			}
			
			if (arr[j] == '\0' || arr[j] == '\n')
				break;

		}
	}

	fclose(fp);

	for (i = 0; i < numOfV; i++) {
		dfn[i] = -1;
		low[i] = -1;
	}
	num = 0;
}

void makeGraph(node* graph, int num) {
	node* newNode = (node*)malloc(sizeof(node));
	node* tmp = graph;
	newNode->vertex = num;
	newNode->link = NULL;

	if (tmp->link == NULL) {
		graph->link = newNode;
	}

	else {
		while (tmp->link != NULL)
			tmp = tmp->link;
		tmp->link = newNode;
	}
}

void bicon(int u, int v)
{
	node* ptr;
	int w, x, y;
	int top = 0;
	dfn[u] = low[u] = num++;
	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		if (v != w && dfn[w] < dfn[u])
			add(u, w);
		if (dfn[w] < 0) {
			bicon(w, u);
			low[u] = MIN2(low[u], low[w]);
			if (low[w] >= dfn[u]) {
				printf("New biconnected component:");
				do {
					delete(&x, &y);
					printf("<%d, %d>", x, y);
				} while (x != u && y != w);
				
				printf("\n");
			}
		}
		else if (w != v) {
			low[u] = MIN2(low[u], dfn[w]);
		}
	}

}

void add(int u, int w) {
	Stack* tmp = (Stack*)malloc(sizeof(Stack));
	tmp->x = u;
	tmp->y = w;

	if (stack->link == NULL) {
		stack->link = tmp;
		tmp->link = NULL;
	}
	else {
		tmp->link = stack->link;
		stack->link = tmp;	
	}
}

void delete(int* x, int* y) {
	*x = stack->link->x;
	*y = stack->link->y;
	node* del;

	del = stack->link;
	stack->link = stack->link->link;

	free(del);
}

