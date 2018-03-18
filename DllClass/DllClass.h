#ifndef _DLL_CLASS_H_
#define _DLL_CLASS_H_

#ifdef _DLLCLASS_DLL_
#define DLLCLASS_EXT __declspec(dllexport)
#else
#define DLLCLASS_EXT __declspec(dllimport)
#endif

class DLLCLASS_EXT CMath
{
public:
	int Add(int n1, int n2);
	int Sub(int n1, int n2);
};

#endif