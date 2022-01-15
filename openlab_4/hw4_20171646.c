#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_EXPR_SIZE 100
typedef enum { lparen, rparen, plus, minus, times, divide, mod, eos, operand }
precedence;

typedef struct _list {
	struct _node* head;
} list;

typedef struct _node {
	struct _node* next;
	char data;
} node;

char expr[MAX_EXPR_SIZE];
char pf[MAX_EXPR_SIZE];
int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 }; 
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };

precedence token;

precedence getToken(char* symbol, int* n)
{
	*symbol = expr[(*n)++];
	switch (*symbol) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	case '%': return mod;
		case '\0' : return eos;
		default: return operand;
	}
}

void Push(list* stack, char data) {
	node* newnode = (node*)malloc(sizeof(node));

	newnode->data = data;
	newnode->next = stack->head;

	stack->head = newnode;
}

char Pop(list* stack) {
	node* rnode;
	char rdata;

	if (IsEmpty(stack)) {
		printf("Error!!\n");
		exit(-1);
	}

	rdata = stack->head->data;
	rnode = stack->head;

	stack->head = stack->head->next;
	free(rnode);
	return rdata;
}

int IsEmpty(list* stack) {
	if (stack->head == NULL)
		return 1;
	else
		return 0;
}

void stackInit(list* stack) {
	stack->head = NULL;
}

char getData(list* stack) {
	return stack->head->data;
}

precedence stackToken(list* stack) {
	char c = getData(stack);

	switch (c) {
	case '(': return lparen;
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	case '%': return mod;
	}
}

void postfix() {
	list stack;
	char symbol;
	int n = 0;
	int index = 0;
	stackInit(&stack);
	
	for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
		if (token == operand) {
			pf[index++] = symbol;
		}

		else if(token == rparen) {
			while (getData(&stack) != '(') {
				pf[index++] = Pop(&stack);
			}
			Pop(&stack);
		}

		else {
			if (!IsEmpty(&stack)) {
				while (isp[stackToken(&stack)] >= icp[token]) {
					pf[index++] = Pop(&stack);

					if (IsEmpty(&stack))
						break;
				}
			}
			Push(&stack, symbol);
		}
	}

	while (!IsEmpty(&stack)) {
		if (getData(&stack) != '(')
			pf[index++] = Pop(&stack);
		else
			Pop(&stack);
	}

	printf("%s\n", pf);
}

void main()
{
	FILE* fp = fopen("expr.txt", "r");
	char c;
	int i = 0;
	
	while (fscanf(fp, "%c", &c) != EOF) {
		if (c != ' ') {
			expr[i] = c;
			i++;
		}
	}

	postfix();

	fclose(fp);
}