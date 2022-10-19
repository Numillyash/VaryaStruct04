#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define size 500
#define True 1
#define False 0
#define Empty INT_MIN

int Malloc = 0;
int Free = 0;

int alf(char c) {
	char A[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\0' };
	for (int i = 0; A[i] != '\0'; i++) {
		if (c == A[i])return True;
	}
	return False;
}

// ! # - НОД

struct data {
	char letter;
	float value;
}data;

typedef struct Node {
	char symbol;
	float n;
	struct Node* left;
	struct Node* right;
}tree;

typedef struct Node2 {
	tree* T;
	struct Node2* next;
}stack;

stack* Stack;
tree* myTree;

tree* New(char a) {
	tree* new = (tree*)malloc(sizeof(tree)); Malloc++;
	new->symbol = a;
	new->left = new->right = NULL;
	return new;
}

int priority(char c) {
	switch (c) {
	case '+':
	case '-':return 1;
	case '*':
	case '/':return 2;
	case '^':return 3;
	case '~':
	case '#':return 4;
	case '!':return 5;
	}
	return 10;
}

/// <summary>
/// Получает подстроку
/// </summary>
/// <param name="expr"></param>
/// <param name="substring"></param>
/// <param name="start"></param>
/// <param name="end"></param>
/// <returns></returns>
char* getSubstring(char expr[50], char substring[50], int start, int end) {
	for (int i = start; i <= end; i++)
	{
		substring[i - start] = expr[i];
		substring[i - start + 1] = '\0';
	}
	if (end < start)
		substring[0] = '\0';
	return substring;
}

char* deleteBackets(char expr[50], int first, int second) {
	for (int i = first; i < size; i++) {
		expr[i] = expr[i + 1];
		if (i == second) {
			expr[i - 1] = '\0';
			break;
		}
	}
	return expr;
}
tree* parse(char expr[50], int start, int end, tree* p) {

	char c; int cnt = 0;
	int minPriority = 10;
	int currentPriority, k = 0;

	char substringLeft[50];
	char substringRight[50];

	p = (tree*)malloc(sizeof(tree)); Malloc++;
	p->left = p->right = NULL;

	for (int i = 0; i >= 0; i++) {
		if (expr[i] != '\0')end = i;
		else break;
	}

	if (expr[0] == '\0')
		return NULL;

	if (start == end) {
		p->symbol = expr[start];
		p->left = p->right = NULL;
		return p;
	}

	for (int i = start; i <= end; i++) {
		c = expr[i];
		//int newEnd = 0;

		if (c == '(') { cnt++; continue; }
		if (c == ')') { cnt--; continue; }
		if (cnt > 0)continue;

		currentPriority = priority(c);
		if (currentPriority <= minPriority) {
			minPriority = currentPriority;
			k = i;
		}
	}
	if (minPriority == 10 && expr[start] == '(' && expr[end] == ')') {
		deleteBackets(expr, start, end);
		return parse(expr, start, end, p);
	}


	p->symbol = expr[k];
	if (k - 1 >= 0) {
		getSubstring(expr, substringLeft, 0, k - 1);
		p->left = parse(substringLeft, 0, k - 1, p);
	}
	getSubstring(expr, substringRight, k + 1, end);
	p->right = parse(substringRight, 0, end, p);

	return p;
}


void Push(tree* a) {
	stack* new;
	new = (stack*)malloc(sizeof(stack)); Malloc++;
	new->T = a;
	new->next = Stack;
	Stack = new;
}

void Pop(stack* newStack) {
	stack* s = newStack;
	newStack = newStack->next;
	free(s); Free++;
	Stack = newStack;
}

void Print(tree* v) {
	static int n = 0;
	if (v == NULL) return;
	n++;
	Print(v->right);
	printf("(Level %d) %c \n", n, v->symbol);
	Print(v->left);
	n--;
}


char* prepareArray(char c[50], char A[size], int n) {
	for (int i = n; i > 0; i++) {
		if (A[i] == '\0') { c[i - n] = '\0'; break; }
		else c[i - n] = A[i];
	}
	return c;
}

tree* loadPST(char expr[50]) {
	for (int i = 0; expr[i] != '\0'; i++) {
		if (alf(expr[i]) == True || (expr[i] >= '0' && expr[i] <= '9')) {
			tree* newNode = New(expr[i]);
			Push(newNode);
		}
		else if (priority(expr[i]) != 10) {
			tree* newNode = New(expr[i]);

			if (Stack && Stack->T) {
				if (expr[i + 1] == '\0') {
					if (expr[i] == '~') {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
						return Stack->T;
					}
					if (expr[i] == '-' && !Stack->next) {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
						return Stack->T;
					}
					newNode->right = Stack->T;
					newNode->left = Stack->next->T;
					Pop(Stack);
					Pop(Stack);
					Push(newNode);
					return Stack->T;
				}
				if (newNode->symbol == '-') {
					if (alf(expr[i - 1]) == True) {
						newNode->left = Stack->next->T;
						newNode->right = Stack->T;
						Pop(Stack);
						Pop(Stack);
						Push(newNode);
					}
					else if (Stack->next && priority(Stack->next->T->symbol) != 10) {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
					}
					/*else if(Stack->next->T->left && Stack->next->T->right){
						newNode->right = Stack->T;
						newNode->left = Stack->next->T;
						Pop(Stack);
						Pop(Stack);
						Push(newNode,'0');
					}
					else if(!Stack->next){
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode,'0');
					}*/
				}
				if (newNode->symbol == '~') {
					//if(Stack->next){newNode->left = Stack->next->T;}
					newNode->right = Stack->T;
					//Pop(Stack);
					Pop(Stack);
					Push(newNode);
				}
				else if (newNode->symbol != '-') {
					newNode->left = Stack->next->T;
					newNode->right = Stack->T;
					Pop(Stack);
					Pop(Stack);
					Push(newNode);
				}
			}
		}
	}
	return Stack->T;
}
tree* loadPRF(char expr[50], unsigned long n) {
	n--;
	for (unsigned long i = n;; i--) {
		if (alf(expr[i]) == True || (expr[i] >= '0' && expr[i] <= '9')) {
			tree* newNode = New(expr[i]);
			Push(newNode);
		}
		else if (priority(expr[i]) != 10) {
			tree* newNode = New(expr[i]);

			if (Stack && Stack->T) {
				if (i == 0) {
					if (expr[i] == '~') {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
						return Stack->T;
					}
					if (expr[i] == '-' && !Stack->next) {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
						Push(newNode);
						return Stack->T;
					}
					newNode->left = Stack->T;
					newNode->right = Stack->next->T;
					Pop(Stack);
					Pop(Stack);
					Push(newNode);
					return Stack->T;
				}
				if (newNode->symbol == '-') {
					if (alf(expr[i - 1]) == False && Stack->next) {
						newNode->right = Stack->next->T;
						newNode->left = Stack->T;
						Pop(Stack);
						Pop(Stack);
						Push(newNode);
					}
					else if (Stack && priority(Stack->T->symbol) != 10) {
						newNode->right = Stack->T;
						Pop(Stack);
						Push(newNode);
					}
					/*else if(!Stack->next){
						newNode->left = Stack->T;
						Pop(Stack);
						Push(newNode,'0');
					}*/
				}
				if (newNode->symbol == '~') {
					//if(Stack){newNode->right = Stack->T;Pop(Stack);}
					newNode->right = Stack->T;

					Pop(Stack);
					Push(newNode);
				}
				else if (newNode->symbol != '-') {
					newNode->right = Stack->next->T;
					newNode->left = Stack->T;
					Pop(Stack);
					Pop(Stack);
					Push(newNode);
				}
			}
		}
	}
	return Stack->T;
}
void savePRF(tree* t, FILE* output) {
	if (t) {
		printf("%c", t->symbol);
		fprintf(output, "%c", t->symbol);
		savePRF(t->left, output);
		savePRF(t->right, output);
	}
}
void savePST(tree* t, FILE* output) {
	if (t) {
		savePST(t->left, output);
		savePST(t->right, output);
		printf("%c", t->symbol);
		fprintf(output, "%c", t->symbol);
	}
}
void clearTreeNum(tree* t) {
	if (t) {
		t->n = Empty;
		clearTreeNum(t->left);
		clearTreeNum(t->right);
	}
}
void findInTree(tree* t) {
	if (t) {
		if (t->symbol == data.letter) {
			t->n = data.value;
			return;
		}
		if (t->symbol >= '0' && t->symbol <= '9')t->n = atoll(&t->symbol);
		findInTree(t->left);
		findInTree(t->right);
	}
}
long long power(float k, float  num) {
	if (k == 1)return num;
	return num * power(k - 1, num);
}

long long factorial(float k)
{
	float res = 1;
	for (int i = 1; i <= k; i++)
		res *= i;
	return res;
}

void eval(tree* t, tree* u) {
	if (t && t->left && t->right) {

		char c = t->symbol;
		float result = 0;
		if ((t->left->n != Empty && t->right->n != Empty) && priority(t->symbol) != 10) {

			float nLeft = (float)t->left->n;
			float nRight = (float)t->right->n;

			switch (c) {
			case '+': result = nLeft + nRight; break;
			case '-': result = nLeft - nRight; break;
			case '*': result = nLeft * nRight; break;
			case '/': result = nLeft / nRight; break;
			case '^': result = power(nRight, nLeft); break;
			case '~': result = fabs(nRight);
			case '!': result = factorial(nLeft);
			}
			t->n = result;
			//printf("result = %lld\n",result);
			//res = result;
			t->left = NULL;
			t->right = NULL;
		}
		eval(t->left, u);
		eval(t->right, u);
	}
	if (t) {
		if (t->symbol == '-' && !t->left && t->right) {
			if (t->right->n == Empty)eval(t->right, u);
			if (t->right) {
				t->n = 0 - t->right->n;
				t->right = NULL;
				eval(u, u);
			}
		}
		if (t->symbol == '~' && !t->left && t->right) {
			if (t->right->n == Empty)eval(t->right, u);
			if (t->right && t->right->n != Empty) {
				t->n = fabs(t->right->n);
				t->right = NULL;
			}
			eval(u, u);
		}
		if (t->symbol == '!' && t->left && !t->right) {
			if (t->left->n == Empty)eval(t->left, u);
			if (t->left && t->left->n != Empty) {
				t->n = factorial(t->left->n);
				t->left = NULL;
			}
			eval(u, u);
		}
	}
	else {
		free(t); Free++;
		eval(u, u);
	}
}
int checkArray(char expr[50]) {
	for (int i = 0; expr[i] != '\0'; i++) {
		if (alf(expr[i]) == False && priority(expr[i]) == 10 && !(expr[i] >= '0') && !(expr[i] <= '9')) {
			return False;
		}
	}
	return True;
}

int main() {
	myTree = NULL;
	Stack = NULL;

	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "w");
	FILE* memstat = fopen("memstat.txt", "w");
	if (input == NULL) { printf("File input error\n"); exit(0); }
	if (output == NULL) { printf("File output\n"); exit(0); }
	if (memstat == NULL) { printf("File memstat error\n"); exit(0); }

	while (!feof(input)) {

		char A[size];
		char expr[50];
		int N = 0, i = 0;

		for (int i = 0; i < size && !feof(input); i++) {
			A[i] = fgetc(input);
			if (A[i] == '\n' || A[i] == EOF) { A[i] = '\0'; break; }
		}
		if (!checkArray(expr)) { fprintf(output, "incorrect\n"); exit(0); }

		if (A[0] == 'p' && A[1] == 'a' && A[2] == 'r' && A[3] == 's' && A[4] == 'e') {
			N = i - 6;
			prepareArray(expr, A, 6);
			myTree = parse(expr, 0, N - 1, myTree);
			printf("\n");//Print(myTree);
			fprintf(output, "Success\n");

		}

		else if (A[0] == 'l' && A[1] == 'o' && A[2] == 'a' && A[3] == 'd' && A[4] == '_' && A[5] == 'p' && A[6] == 'r' && A[7] == 'f') {
			N = i - 7;

			prepareArray(expr, A, 9); unsigned long n = strlen(expr);
			myTree = loadPRF(expr, n);
			fprintf(output, "Success\n");
			Pop(Stack);
		}
		else if (A[0] == 'l' && A[1] == 'o' && A[2] == 'a' && A[3] == 'd' && A[4] == '_' && A[5] == 'p' && A[6] == 's' && A[7] == 't') {
			N = i - 6;
			prepareArray(expr, A, 9);
			//printf("%s\n",expr);
			myTree = loadPST(expr);
			fprintf(output, "Success\n");
			Pop(Stack);

		}
		else if (A[0] == 's' && A[1] == 'a' && A[2] == 'v' && A[3] == 'e' && A[4] == '_' && A[5] == 'p' && A[6] == 'r' && A[7] == 'f') {
			if (myTree) {
				printf("-> Pref: ");
				fprintf(output, "-> Pref: ");
				savePRF(myTree, output);
				printf("\n");
				fprintf(output, "\n");
			}
			else { fprintf(output, "not_loaded\n"); exit(0); }
		}
		else if (A[0] == 's' && A[1] == 'a' && A[2] == 'v' && A[3] == 'e' && A[4] == '_' && A[5] == 'p' && A[6] == 's' && A[7] == 't') {
			if (myTree) {
				printf("-> Post: ");
				fprintf(output, "-> Post: ");
				savePST(myTree, output);
				printf("\n");
				fprintf(output, "\n");
			}
			else { fprintf(output, "not_loaded\n"); exit(0); }
		}
		else if (A[0] == 'e' && A[1] == 'v' && A[2] == 'a' && A[3] == 'l') {
			prepareArray(expr, A, 5);
			//printf("%s\n",expr);

			clearTreeNum(myTree);
			int c = 0;
			for (int i = 0; expr[i] != '\0'; i++) {
				if (alf(expr[i]) == True) {
					data.letter = expr[i];
					data.value = atoi(&expr[i + 2]);
					findInTree(myTree);
					c++;
				}
			}
			eval(myTree, myTree);

			printf("Result is %f\n\n", myTree->n);
			fprintf(output, "Result is %f\n\n", myTree->n);
			myTree = NULL; free(myTree);

		}
	}
	fprintf(memstat, "Malloc = %d\n", Malloc);
	fprintf(memstat, "Free = %d\n", Free);
}