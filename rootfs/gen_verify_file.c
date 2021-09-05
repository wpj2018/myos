#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp = fopen("verify.bin", "wb+");
	for (size_t i = 0; i < 8 * 1024 * 1024; i++) {
		fwrite(&i, sizeof(int), 1, fp);
	}
	fclose(fp);
	return 0;
}
