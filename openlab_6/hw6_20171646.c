#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_TERMS 100
typedef struct treeNode* treePtr;

typedef struct treeNode {
	treePtr lchild;
	int data;
	treePtr rchild;
}Node;

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int);

void main()
{
	int i, n, A[MAX_TERMS];
	treePtr tree = NULL;
	FILE* fp = fopen("input.txt","r");
	fscanf(fp, "%d", &n);
	for (i = 0; i < n; i++)
		fscanf(fp, "%d", &A[i]);
	// 순서대로 binary search tree에 삽입한다 //
	for (i = 0; i < n; i++)
		insert(&tree, A[i]);

	inorder(tree);

	printf("\n\n\n");

	preorder(tree);

	fclose(fp);
	
}

void insert(treePtr* tree, int num) {
	treePtr ptr, temp = modifiedSearch(*tree, num);
	if (temp != NULL || !(*tree)) {
		/* num is not in the tree */
		ptr = (treePtr)malloc(sizeof(Node));
		ptr->data = num;
		ptr->lchild = ptr->rchild = NULL;
		if (*tree) /* insert as child of temp */
			if (num < temp->data) {
				temp->lchild = ptr;
			}
			else {
				temp->rchild = ptr;
			}
		else *tree = ptr;
	}

}

treePtr modifiedSearch(treePtr tree, int num) {
	if (tree == NULL)
		return NULL;

	else {
		treePtr temp = tree;

		if (temp->data > num) {
			if (tree->lchild == NULL)
				return temp;
			else
				modifiedSearch(tree->lchild, num);
		}
		else if (temp->data < num) {
			if (tree->rchild == NULL)
				return temp;
			else
				modifiedSearch(tree->rchild, num);
		}
		else
			return NULL;
	}

}

void inorder(treePtr tree) {
	if (tree) {
		inorder(tree->lchild);
		printf("%d\n", tree->data);
		inorder(tree->rchild);
	}
	
}

void preorder(treePtr tree) {
	if (tree) {
		printf("%d\n", tree->data);
		preorder(tree->lchild);
		preorder(tree->rchild);
	}
}