// stdafx.cpp : 只包括标准包含文件的源文件
// GameServer.pch 将成为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

int RandInt(int nMin,int nMax)
{
	int nDelta = nMax - nMin;
	if (nDelta <=0)
	{
		return nMax;
	}

	int nRand1 = ::rand();
	int nRand2 = ::rand();
	return ((nRand1<<15) + nRand2)%(nDelta+1) + nMin;
}

const char* GetRootPath(void)
{
#ifdef WIN32
	static char szPath[MAX_PATH];
	static bool bFirstTime = true;

	if(bFirstTime)
	{
		bFirstTime = false;
		GetModuleFileName(NULL, (char*)szPath, sizeof(szPath));
		char *p = strrchr(szPath, '\\');
		*p = '\0';
	}

	return szPath;
#else
	return "./";
#endif
}