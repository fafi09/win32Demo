#include "stdio.h"
#include "conio.h"
#pragma comment(lib, "../lib/wincpplib.lib")
#pragma comment(lib, "../lib/winclib.lib")
int CPP_Add(int n1, int n2);
int CPP_Sub(int n1, int n2);
extern "C"
{
int C_Add(int n1, int n2);
int C_Sub(int n1, int n2);
}

int main()
{
	int nAdd = 0;
	int nSub = 0;

	nAdd = CPP_Add(100,50);
	nSub = CPP_Sub(100,50);

	printf("nAdd=%d, nSub=%d\n",nAdd,nSub);

	nAdd = C_Add(100,50);
	nSub = C_Sub(100,50);

	printf("nAdd=%d, nSub=%d",nAdd,nSub);

	getch();
}