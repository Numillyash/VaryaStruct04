#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
#define MAX 256

// counter[0] = malloc, counter[1] = calloc, counter[2] = realloc, counter[3] = free
int counter[4] = { 0 };

struct b_tree {
	char opera;
	int unar;
	int num;
	char var;
	struct b_tree* right;
	struct b_tree* left;
};

int fact(int num) {
	int res = 1;
	if (num == 0) return res;
	for (int i = 1; i <= num; i++) {
		res *= i;
	}
	return res;
}

int nod(int a, int b) {
	while (a != 0 && b != 0) {
		if (a > b) a = a % b;
		else b = b % a;
	}
	return a + b;
}

int isoperator(char c) {
	if (c == '#' || c == '*' || c == '/' || c == '%' || c == '+' || c == '-' || c == '!') return 1;
	else return 0;
}

int isunary_m(char* c, int i) {
	if (i == 0 && c[i] == '-') return 1;
	else {
		if (c[i - 1] == '(' && c[i] == '-') return 1;
		else return 0;
	}
}

int priority(char c, int f) {
	if (c == '-' && f == 1) return 3;
	switch (c) {
	case '+':
	case '-':
		return 0;
		break;
	case '*':
	case '/':
	case '%':
		return 1;
		break;
	case '#':
		return 2;
		break;
	case '!':
		return 4;
		break;
	}

	return -1;
}

char* nulify(char* s, int k) {
	for (int i = 0; i < k; i++) {
		s[i] = '\0';
	}
	return s;
}

char* swap(char* s, int k) {
	if (k == 1) return s;
	else {
		int n = k - 1;
		char tmp;
		for (int i = 0; i < k / 2; i++) {
			tmp = s[i];
			s[i] = s[n];
			s[n--] = tmp;
		}
		return s;
	}
}

void del(struct b_tree* root) {
	if (root != NULL)
	{
		del(root->left);
		del(root->right);
		free(root);
		counter[3]++;
	}
	else return;
}

struct b_tree* skobka(char* s, int* i) {
	struct b_tree* node{}, * child{}, * prevnode{}, * root{}, * skoba{};
	char prevopera = '\0', tmp[255] = { '\0' };
	int n = 0, j = 0;
	(*i)++;
	while (1) {
		if (s[(*i)] == '(') {
			skoba = skobka(s, i);
			if (prevnode != NULL) {
				if (prevnode->left != NULL) {
					prevnode->right = skoba;
					skoba = NULL;
				}
			}
			else {
				child = skoba;
			}
		}
		else if (isdigit(s[(*i)]) || islower(s[(*i)])) {
			child = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			if (islower(s[(*i)])) {
				child->var = s[(*i)];
			}
			else {
				j = 0;
				while (isdigit(s[(*i)])) {
					tmp[j++] = s[(*i)++];
				}
				(*i)--;
				n = atoi(tmp);
				child->num = n;
			}
			if (prevnode != NULL && prevnode->unar) {
				prevnode->right = child;
			}
			else if (prevnode != NULL) {
				if (prevnode->left != NULL) {
					prevnode->right = child;
				}
			}
		}
		else if (isoperator(s[(*i)])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			node->opera = s[(*i)];
			if (isunary_m(s, *i)) {
				node->unar = 1;
				prevopera = node->opera;
				root = node;
				prevnode = node;
				(*i)++;
				continue;
			}
			if (prevopera == '\0') {
				node->left = child;
				root = node;
				prevnode = node;
				prevopera = node->opera;
			}
			else if (priority(prevopera, prevnode->unar) >= priority(s[(*i)], node->unar)) {
				node->left = prevnode;
				root = node;
				prevnode = node;
				prevopera = node->opera;
			}
			else if (priority(prevopera, prevnode->unar) < priority(s[(*i)], node->unar)) {
				node->left = root->right;
				root->right = node;
				prevnode = node;
			}
		}
		else if (s[(*i)] = ')') {
			break;
		}
		(*i)++;
	}
	return root;
}

struct b_tree* parse(char* s) {
	struct b_tree* node{}, * child{}, * prevnode{}, * root{}, * skoba{};
	char prevopera = '\0', tmp[255] = { '\0' };
	int n = 0, j = 0;
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] == '(') {
			skoba = skobka(s, &i);
			if (prevnode != NULL) {
				if (prevnode->left != NULL) {
					prevnode->right = skoba;
					skoba = NULL;
				}
			}
			else {
				child = skoba;
			}
		}
		else if (isdigit(s[i]) || islower(s[i])) {
			child = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			if (islower(s[i])) {
				child->var = s[i];
			}
			else {
				j = 0;
				while (isdigit(s[i])) {
					tmp[j++] = s[i++];
				}
				i--;
				n = atoi(tmp);
				child->num = n;
			}
			if (prevnode != NULL && prevnode->unar) {
				prevnode->right = child;
			}
			else if (prevnode != NULL) {
				if (prevnode->left != NULL) {
					prevnode->right = child;
				}
			}
		}
		else if (isoperator(s[i])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			node->opera = s[i];
			if (isunary_m(s, i)) {
				node->unar = 1;
				prevopera = node->opera;
				root = node;
				prevnode = node;
				continue;
			}
			if (prevopera == '\0') {
				node->left = child;
				root = node;
				prevnode = node;
				prevopera = node->opera;

			}
			else if (priority(prevopera, prevnode->unar) >= priority(s[i], node->unar)) {
				node->left = root;
				root = node;
				prevnode = node;
				prevopera = node->opera;
			}
			else if (priority(prevopera, prevnode->unar) < priority(s[i], node->unar)) {
				node->left = root->right;
				root->right = node;
				prevnode = node;
			}
		}
	}
	if (root == NULL && skoba != NULL) {
		return skoba;
	}
	else {
		return root;
	}
}

struct b_tree* load_prf(char* s) {
	struct b_tree* node{};
	struct b_tree* stk[10]{};
	int t = 0;
	char tmp[255]{}, h{};
	int j = 0, n{};
	for (int i = strlen(s) - 1; i >= 0; i--) {
		if (islower(s[i]) || isdigit(s[i])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			if (islower(s[i])) {
				node->var = s[i];
			}
			else if (isdigit(s[i])) {
				j = 0;
				while (isdigit(s[i])) {
					tmp[j++] = s[i--];
				}
				i++;
				n = atoi(swap(tmp, j));
				nulify(tmp, j);
				node->num = n;
			}
			stk[t] = node;
			t++;
		}
		else if (isoperator(s[i])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			node->opera = s[i];
			if (s[i] != '!') {
				if (t < 1) return NULL;
				t--;
				node->left = stk[t];
				node->right = stk[t - 1];
				stk[t - 1] = node;
				stk[t] = NULL;
			}
			else {
				if (t > -1) {
					t--;
					node->left = stk[t];
					stk[t] = node;
					t++;
				}
				else return NULL;
			}
		}
	}
	return stk[0];
}

struct b_tree* load_pst(char* s) {
	struct b_tree* node{};
	struct b_tree* stk[255]{};
	int t = 0;
	char tmp[255]{};
	int j = 0, n{};
	for (int i = 0; i < strlen(s); i++) {
		if (islower(s[i]) || isdigit(s[i])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			if (islower(s[i])) {
				node->var = s[i];
			}
			else if (isdigit(s[i])) {
				j = 0;
				while (isdigit(s[i])) {
					tmp[j++] = s[i++];
				}
				i--;
				n = atoi(tmp);
				nulify(tmp, j);
				node->num = n;
			}
			stk[t] = node;
			t++;
		}
		else if (isoperator(s[i])) {
			node = (struct b_tree*)calloc(1, sizeof(struct b_tree));
			counter[1]++;
			node->opera = s[i];
			if (s[i] != '!') {
				if (t < 1) return NULL;
				t--;
				node->right = stk[t];
				node->left = stk[t - 1];
				stk[t - 1] = node;
				stk[t] = NULL;
			}
			else {
				if (t > -1) {
					t--;
					node->left = stk[t];
					stk[t] = node;
					t++;
				}
				else return NULL;
			}
		}
	}
	return stk[0];
}

void save_prf(struct b_tree* tree) {
	if (tree == NULL) return;
	if (tree->opera != '\0') {
		printf("%c", tree->opera);
	}
	else if (tree->var != '\0') {
		printf("%c", tree->var);
	}
	else {
		printf("%d ", tree->num);
	}
	save_prf(tree->left);
	save_prf(tree->right);
}

void save_pst(struct b_tree* tree) {
	if (tree == NULL) return;
	save_pst(tree->left);
	save_pst(tree->right);
	if (tree->opera != '\0') {
		printf("%c", tree->opera);
	}
	else if (tree->var != '\0') {
		printf("%c", tree->var);
	}
	else {
		printf("%d ", tree->num);
	}
}

void replace(struct b_tree* tree, char* vars, int* vals, int i) {
	if (tree == NULL) return;
	if (tree->var == vars[i]) {
		tree->var = '\0';
		tree->num = vals[i];
	}
	replace(tree->left, vars, vals, i);
	replace(tree->right, vars, vals, i);
}

int eval(struct b_tree* tree) {
	if (tree != NULL) {
		if (tree->right == NULL && tree->left == NULL) {
			return tree->num;
		}
		else {
			int res = 0, l = eval(tree->left), r = eval(tree->right);
			if (tree->opera == '!') {
				res = fact(l);
				return res;
			}
			else if (tree->unar == 1 && tree->opera == '-') {
				res = 0 - r;
				return res;
			}
			else {
				switch (tree->opera) {
				case ('+'):
					res = l + r;
					return res;
				case ('-'):
					res = l - r;
					return res;
				case ('/'):
					res = l / r;
					return res;
				case ('*'):
					res = l * r;
					return res;
				case ('%'):
					res = l % r;
					return res;
				case ('#'):
					res = nod(l, r);
					return res;
				}
			}
		}
	}
	else return 0;
}

int check(char* s) {
	int f_o = 0, f_c = 0;
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] == '(') f_o++;
		else if (s[i] == ')') f_c++;
	}
	if (f_o != f_c) return 0;
	return 1;
}

int main() {
	FILE* inp, * mem;
	inp = fopen("input.txt", "r");
	mem = fopen("memstat.txt", "w+");
	struct b_tree* tmp{};

	char* cmd = (char*)calloc(256, sizeof(char));
	char* expr;
	counter[1]++;
	while (1) {
		fgets(cmd, 255, inp);
		if (strncmp(cmd, "parse ", 6) == 0) {
			expr = (char*)calloc(strlen(cmd) + 1 - 6, sizeof(char));
			strcpy(expr, cmd + 6);
			expr[strlen(expr) - 1] = '\0';
			counter[1]++;
			if (tmp != NULL) {
				del(tmp);
				tmp = NULL;
			}
			if (check(expr)) {
				tmp = parse(expr);
				if (tmp != NULL) {
					printf("success\n");
				}
				else printf("incorrect\n");
			}
			else printf("incorrect\n");
			memset(cmd, '\0', 256);
			free(expr);
			counter[3]++;
		}
		else if (strncmp(cmd, "load_prf ", 9) == 0) {
			expr = (char*)calloc(strlen(cmd) + 1 - 9, sizeof(char));
			strcpy(expr, cmd + 9);
			expr[strlen(expr) - 1] = '\0';
			counter[1]++;
			if (tmp != NULL) {
				del(tmp);
				tmp = NULL;
			}
			tmp = load_prf(expr);
			if (tmp != NULL) {
				printf("success\n");
			}
			else printf("incorrect\n");

			memset(cmd, '\0', 256);
			free(expr);
			counter[3]++;
		}
		else if (strncmp(cmd, "load_pst ", 9) == 0) {
			expr = (char*)calloc(strlen(cmd) + 1 - 9, sizeof(char));
			strcpy(expr, cmd + 9);
			expr[strlen(expr) - 1] = '\0';
			counter[1]++;

			if (tmp != NULL) {
				del(tmp);
				tmp = NULL;
			}
			tmp = load_pst(expr);
			if (tmp != NULL) {
				printf("success\n");
			}
			else printf("incorrect\n");

			memset(cmd, '\0', 256);
			free(expr);
			counter[3]++;
		}
		else if (strncmp(cmd, "save_prf", 8) == 0) {
			if (tmp != NULL) {
				save_prf(tmp);
				printf("\n");
			}
			else printf("not_loaded\n");
			memset(cmd, '\0', 256);
		}
		else if (strncmp(cmd, "save_pst", 8) == 0) {
			if (tmp != NULL) {
				save_pst(tmp);
				printf("\n");
			}
			else printf("not_loaded\n");
			memset(cmd, '\0', 256);
		}
		else if (strncmp(cmd, "eval ", 5) == 0) {
			expr = (char*)calloc(strlen(cmd) + 1 - 5, sizeof(char));
			strcpy(expr, cmd + 5);
			expr[strlen(expr) - 1] = '\0';
			counter[1]++;
			char* temp, * tok, vars[255]{}, d[20]{};
			int vals[255]{}, j = 0, n = 0;
			temp = expr;
			while ((tok = strtok(temp, ",")) != NULL) {
				temp = NULL;
				n = 0;
				if (islower(tok[0]) && tok[1] == '=' && isdigit(tok[2])) vars[j] = tok[0];
				else printf("incorrect");
				for (int i = 2; i < strlen(tok); i++) {
					if (isdigit(tok[i])) d[n++] = tok[i];
				}
				vals[j++] = atoi(d);
				nulify(d, n);
			}
			int res = 0;
			if (tmp != NULL) {
				for (int i = 0; i < j; i++) {
					replace(tmp, vars, vals, i);
				}
				res = eval(tmp);
				printf("%d\n", res);
				del(tmp);
				tmp = NULL;
			}
			memset(cmd, '\0', 256);
			memset(vars, '\0', 255);
			memset(vals, 0, 255);
			free(expr);
			counter[3]++;
		}
		else if (strncmp(cmd, "exit", 4) == 0) {
			if (tmp != NULL) del(tmp);
			free(cmd);
			counter[3]++;
			fprintf(mem, "malloc=%d\ncalloc=%d\nrealloc=%d\nfree=%d\n", counter[0], counter[1], counter[2], counter[3]);
			break;
		}
	}

	return 0;
}
