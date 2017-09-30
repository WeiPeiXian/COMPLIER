#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
	FILE* a = fopen("test.scala","r+");
	int key;
		while(key != EOF)
		{
			key = getc(a);
			printf("%c",key );
		}

	return 0;
}