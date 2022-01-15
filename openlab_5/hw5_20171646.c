#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50

typedef enum { head, entry } tagfield;

typedef struct entry_node {
	int row;
	int col;
	int value;
} entryNode;

typedef struct matrix_node {
	struct matrix_node* down;
	struct matrix_node* right;
	tagfield tag;
	union {
		struct matrix_node* next;
		entryNode entry;
	}u;
} Node;

Node* mread(FILE*);
void mwrite(Node*);
void merase(Node*);

Node* hdnode[MAX_SIZE];

void main() {
	Node* a;
	Node* b;
	FILE* fp1 = fopen("A.txt", "r");
	FILE* fp2 = fopen("B.txt", "r");

	a = mread(fp1);
	b = mread(fp2);

	mwrite(a);
	mwrite(b);

	merase(&a);
	merase(&b);

	mwrite(a);
	mwrite(b);

	fclose(fp1);
	fclose(fp2);
}

Node* mread(FILE* fp) {
	int num_rows, num_cols, num_heads, i;
	int value, current_row;
	int row = 0, col = 0;
	int count = 0;
	Node* temp;
	Node* node;
	Node* last;

	fscanf(fp, "%d %d", &num_rows, &num_cols);

	num_heads = (num_cols > num_rows) ? num_cols : num_rows;

	node = (Node*)malloc(sizeof(Node));
	node->tag = entry;
	node->u.entry.row = num_rows;
	node->u.entry.col = num_cols;


	if (!num_heads) node->right = node;
	else {
		for (i = 0; i < num_heads; i++) {
			temp = (Node*)malloc(sizeof(Node));
			hdnode[i] = temp; hdnode[i]->tag = head;
			hdnode[i]->right = temp; hdnode[i]->u.next = temp;
		}
		current_row = 0; last = hdnode[0];
		while (row < num_rows) {

			fscanf(fp, "%d", &value);

			if (col == num_cols) {
				last->right = hdnode[current_row];
				current_row++;
				last = hdnode[current_row];
				row++; col = 0;
			}

			if (value != 0) {
				temp = (Node*)malloc(sizeof(Node)); temp->tag = entry;
				temp->u.entry.row = row; temp->u.entry.col = col;
				temp->u.entry.value = value; last->right = temp; last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
				count++;
			}
			col++;

		}

		// close all column lists
		for (i = 0; i < num_cols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		// link all head nodes together
		for (i = 0; i < num_heads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];

		hdnode[num_heads - 1]->u.next = node;
		node->right = hdnode[0];
		node->u.entry.value = count;
	}
	return node;
}

void mwrite(Node* node)
{
	if (node == NULL) {
		printf("Empty\n\n");
		return;
	}

	int i;
	Node* temp;
	Node* head = node->right;

	
	for (i = 0; i < node -> u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}

	printf("\n\n");
}

void merase(Node** node)
{
	int i, num_heads;
	Node* x, *y, *head = (*node)->right;
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y; y = y->right; free(x);
		}
		x = head; head = head->u.next; free(x);
	}
	// free remaining head nodes
	y = head;
	while (y != *node) {
		x = y; y = y->u.next; free(x);
	}
	free(*node); *node = NULL;
}
