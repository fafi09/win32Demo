#pragma comment(lib, "../lib/dllclass.lib")

#include "../DllClass/DllClass.h"
#include "stdio.h"
#include "conio.h"

int main()
{
	CMath math;
	int nAdd = math.Add(100,50);
	int nSub = math.Sub(100,50);

	printf("nAdd=%d\n",nAdd);
	printf("nSub=%d\n",nSub);
	getch();
}