#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#define max_string_size 100
#define max_pattern_size 100

int pmatch();
void fail();

char str[max_string_size];
char pat[max_pattern_size];
int failure[max_pattern_size];

int main() {
	FILE* fp = fopen("kmp.txt","r");

	fgets(str,sizeof(str),fp);
	fgets(pat, sizeof(pat), fp);

	fail(pat);
	printf("%d\n", pmatch(str, pat));
	fclose(fp);
	return 0;
}

void fail(char* pat) {
	int i,j, n = strlen(pat);
	failure[0] = -1;

	for (j = 1; j < n; j++) {
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1]) && (i >= 0))
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else failure[j] = -1;
	}
}

int pmatch(char* str, char* pat) {
	int i = 0, j = 0;
	int lens = strlen(str);
	int lenp = strlen(pat);

	while (i < lens && j < lenp) {
		if (str[i] == pat[j]) {
			i++;
			j++;
		}
		else if (j == 0) i++;
		else j = failure[j - 1] + 1;

	}

	return ((j == lenp) ? (i - lenp) : -1);
}
