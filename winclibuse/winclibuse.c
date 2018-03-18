#include "stdio.h"
#include "stdlib.h"
#include "conio.h"
#pragma comment(lib, "../lib/winclib.lib")

int main()
{
	int nAdd = 0;
	int nSub = 0;

	nAdd = C_Add(100, 50);
	nSub = C_Sub(100, 50);

	printf("nAdd=%d\n",nAdd);
	printf("nSub=%d\n",nSub);

	getch();
}