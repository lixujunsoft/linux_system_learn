#include <stdio.h>

int main(int argc, char *argv[])
{
	char *pstrArray[] = {"hello", "world", "how", "are", "you", NULL};
	char **ppStrArray = pstrArray;
	printf("%p\n", pstrArray);
	printf("%p\n", pstrArray[0]);
	for (; *ppStrArray != NULL; ppStrArray++) {
		printf("%s\n", *ppStrArray);
	}
}
