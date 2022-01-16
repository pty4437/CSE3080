#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef enum {treenode, leafnode} tagfield;

typedef struct _treenode {
	int Ldata;
	int Rdata;
	int** thisArr;
	int arrRow;
	struct _treenode* Llink;
	struct _treenode* Rlink;
	tagfield tag;
	union {
		int index;
		int tagNum;
	}u;
} TreeNode;

int** arr;
int numOfData, numOfFeature;


double logB(double);
void makeFirstNode(TreeNode* tree);
void makeTree(TreeNode*, TreeNode*, TreeNode*);
int countT(TreeNode*, int, int);
int countF(TreeNode*, int, int);
int thereIsFeauture(TreeNode*);
double calEntropy(TreeNode* , int, int);
double calEntropy2(int, int);
void makeNodeArr(TreeNode*,TreeNode*, int);
void makeLeafNode(TreeNode*);
void makeOutput(TreeNode*);
void freeTree(TreeNode*);
void PostOrder(TreeNode*);

int main() {
	FILE* dfp = fopen("data.txt", "r");
	TreeNode* tree = (TreeNode*)malloc(sizeof(TreeNode));

	fscanf(dfp, "%d %d", &numOfData, &numOfFeature);

	arr = (int**)malloc(sizeof(int*) * numOfData);
	for (int i = 0; i < numOfData; i++)
		arr[i] = (int*)malloc(sizeof(int) * (numOfFeature + 1));

	for (int i = 0; i < numOfData; i++) {
		for (int j = 0; j < numOfFeature + 1; j++)
			fscanf(dfp, "%d", &arr[i][j]);
	}

	makeFirstNode(tree);
	makeTree(tree, tree->Llink, tree->Rlink);
	makeOutput(tree);
	PostOrder(tree);

	for (int i = 0; i < numOfData; i++)
		free(arr[i]);

	free(arr);
	fclose(dfp);

	return 0;
}

void PostOrder(TreeNode* tree) {
	if (tree->Llink) PostOrder(tree->Llink);
	if (tree->Rlink) PostOrder(tree->Rlink);
	freeTree(tree);
}

void freeTree(TreeNode* tree) {
	for (int i = 0; i < tree->arrRow; i++)
		free(tree->thisArr[i]);
	free(tree->thisArr);
	free(tree);
}

void makeOutput(TreeNode* tree) {
	int numOfTest;
	TreeNode* tmp;
	int* ary;
	FILE* tfp = fopen("test.txt", "r");
	FILE* ofp = fopen("output.txt", "w");

	fscanf(tfp, "%d", &numOfTest);
	ary = (int*)malloc(sizeof(int) * numOfFeature);

	for (int i = 0; i < numOfTest; i++) {
		tmp = tree;
		for (int j = 0; j < numOfFeature; j++)
			fscanf(tfp, "%d", &ary[j]);
		
		while (1) {
			if (tmp->tag == leafnode) {
				fprintf(ofp, "%d\n", tmp->u.tagNum);
				break;
			}
			else if (ary[tmp->u.index] == TRUE) {
				tmp = tmp->Rlink;
			}
			else {
				tmp = tmp->Llink;
			}
		}
	}
	free(ary);
	fclose(tfp); fclose(ofp);
}

void makeFirstNode(TreeNode* tree) {
	tree->Llink = NULL;
	tree->Rlink = NULL;
	tree->tag = treenode;
	double minEntropy;
	int index = 0;
	double TD, FD, tmp;

	tree->thisArr = (int**)malloc(sizeof(int*) * numOfData);
	for (int i = 0; i < numOfData; i++)
		tree->thisArr[i] = (int*)malloc(sizeof(int) * (numOfFeature + 1));

	for (int i = 0; i < numOfData; i++) {
		for (int j = 0; j < numOfFeature + 1; j++) {
			tree->thisArr[i][j] = arr[i][j];
		}
	}

	TD = (double)countT(tree, 0, -1);
	FD = ((double)numOfData - TD);

	tree->arrRow = numOfData;

	minEntropy = -1;

	for (int i = 0; i < numOfFeature; i++) {
		TD = (double)countT(tree, i, -1);
		FD = ((double)numOfData - TD);
		tmp = calEntropy(tree, i, numOfData);

		if (minEntropy > tmp || minEntropy < 0) {
			index = i;
			minEntropy = tmp;
		}
	}

	tree->u.index = index;
	tree->Ldata = FALSE;
	tree->Rdata = TRUE;

}

void makeTree(TreeNode* tree, TreeNode* Lnode, TreeNode* Rnode) {

	double TD, FD, minEntropy, tmp;
	int index = 0;

	//¿ÞÂÊ
	Lnode = (TreeNode*)malloc(sizeof(TreeNode));
	tree->Llink = Lnode;
	Lnode->Llink = NULL; Lnode->Rlink = NULL;
	makeNodeArr(Lnode, tree, tree->Ldata);

	if (!thereIsFeature(Lnode) || Lnode->arrRow == countT(Lnode, numOfFeature, -1) || Lnode->arrRow == countF(Lnode, numOfFeature, -1) || Lnode->arrRow == 0) {
		makeLeafNode(Lnode);
	}
	else
		Lnode->tag = treenode;

	if (Lnode->tag != leafnode) {
		TD = (double)countT(Lnode, 0, -1);
		FD = ((double)(Lnode->arrRow) - TD);

		minEntropy = -1;

		for (int i = 0; i < numOfFeature; i++) {
			if (Lnode->thisArr[0][i] != -1) {
				TD = (double)countT(Lnode, i, -1);
				FD = ((double)numOfData - TD);
				tmp = calEntropy(Lnode, i, Lnode->arrRow);
				if (minEntropy > tmp || minEntropy<0) {
					index = i;
					minEntropy = tmp;
				}
			}
		}

		Lnode->u.index = index;
		Lnode->Ldata = FALSE;
		Lnode->Rdata = TRUE;
	}


	Rnode = (TreeNode*)malloc(sizeof(TreeNode));
	tree->Rlink = Rnode;
	Rnode->Llink = NULL; Rnode->Rlink = NULL;
	makeNodeArr(Rnode, tree, tree->Rdata);

	if (!thereIsFeature(Rnode) || Rnode->arrRow == countT(Rnode, numOfFeature, -1) || Rnode->arrRow == countF(Rnode, numOfFeature, -1) || Rnode->arrRow == 0) {
		makeLeafNode(Rnode);
	}
	else
		Rnode->tag = treenode;

	if (Rnode->tag != leafnode) {
		TD = (double)countT(Rnode, 0, -1);
		FD = ((double)(Rnode->arrRow) - TD);

		minEntropy = -1;

		for (int i = 0; i < numOfFeature; i++) {
			if (Rnode->thisArr[0][i] != -1) {
				TD = (double)countT(Rnode, i, -1);
				FD = ((double)numOfData - TD);
				tmp = calEntropy(Rnode, i, Rnode->arrRow);

				if (minEntropy > tmp || minEntropy < 0) {
					index = i;
					minEntropy = tmp;
				}
			}
		}

		Rnode->u.index = index;
		Rnode->Ldata = FALSE;
		Rnode->Rdata = TRUE;
	}
	
	if(Lnode->tag == treenode)
		makeTree(Lnode, Lnode->Llink, Lnode->Rlink);

	if (Rnode->tag == treenode)
		makeTree(Rnode, Rnode->Llink, Rnode->Rlink);


}



void makeNodeArr(TreeNode* child, TreeNode* parent, int tf) {
	int count = 0;
	int index = 0;

	for (int i = 0; i < parent->arrRow; i++) {
		if (parent->thisArr[i][parent->u.index] == tf)
			count++;
	}

	child->thisArr = (int**)malloc(sizeof(int*) * count);
	for (int i = 0; i < count; i++)
		child->thisArr[i] = (int*)malloc(sizeof(int) * (numOfFeature+1));

	child->arrRow = count;


	for (int i = 0; i < parent->arrRow; i++) {
		if (parent->thisArr[i][parent->u.index] == tf) {
			for (int j = 0; j < numOfFeature + 1; j++) {
				if (j == parent->u.index) {
					child->thisArr[index][j] = -1;
				}
				else {
					child->thisArr[index][j] = parent->thisArr[i][j];
				}
			}
			index++;
		}
	}
}

void makeLeafNode(TreeNode* tree) {
	int T=0, F=0;
	tree->tag = leafnode;

	for (int i = 0; i < tree->arrRow; i++) {
		if (tree->thisArr[i][numOfFeature] == TRUE)
			T++;
		else
			F++;
	}

	if (T > F) {
		tree->u.tagNum = TRUE;
	}
	else {
		tree->u.tagNum = FALSE;
	}
}

double calEntropy(TreeNode* node, int index, int size) {
	int TD = countT(node, index, -1);
	int FD = countF(node, index, -1);

	return ((double)TD / (double)size) * calEntropy2(countT(node, index, 1),TD) + ((double)FD / (double)size)*calEntropy2(countF(node,index,1),FD);
}

double calEntropy2(int a, int b) {
	return ((double)a / (double)b) * logB((double)b / (double)a) + (1 - (double)a / (double)b) * logB(1/(1 - (double)a / (double)b));
}

double logB(double num) {
	return log(num) / log(2.0);
}

int thereIsFeature(TreeNode* tree) {
	for (int i = 0; i < tree->arrRow; i++) {
		for (int j = 0; j < numOfFeature; j++) {
			if (tree->thisArr[i][j] != -1)
				return TRUE;
		}
	}
	return FALSE;
}

int countT(TreeNode* node, int index, int which) {
	int numOfTrue = 0;

	if (which == -1) {
		for (int i = 0; i < node->arrRow; i++) {
			if (node->thisArr[i][index] == TRUE)
				numOfTrue++;
		}
	}

	else{
		for (int i = 0; i < node->arrRow; i++) {
			if (node->thisArr[i][index] == TRUE) {
				if (arr[i][numOfFeature] == TRUE)
					numOfTrue++;
			}		
		}
	}

	return numOfTrue;
}

int countF(TreeNode* node, int index, int which) {
	int numOfFalse = 0;

	if (which == -1) {
		for (int i = 0; i < node->arrRow; i++) {
			if (node->thisArr[i][index] == FALSE)
				numOfFalse++;
		}
	}

	else {
		for (int i = 0; i < node->arrRow; i++) {
			if (node->thisArr[i][index] == FALSE) {
				if (arr[i][numOfFeature] == TRUE)
					numOfFalse++;
			}
		}
	}

	return numOfFalse;
}


