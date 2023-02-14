#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	FILE* out = fopen("dur.txt", "w");
	int d, c;
	scanf("%d %d", &d, &c);
	fprintf(out, "%d %d\n", d, c);
	for (int i = 0; i < d; i++)
		for (int j = 0; j < c; j++)
			fprintf(out, "%d\n", rand() % 15 + 1);
}
