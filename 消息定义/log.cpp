#include "Stdafx.h"
#include "log.h"
#include <string>
#include "GameLogic.h"

//CLog* CLog::m_pInstance = NULL;

CLog			g_Log;				//全局日志

bool CLog::Init(const char* szSubDir,const char* szSubDir2,const char* szLogFileName)
{

	char szDir[1024] = {0};
	sprintf(szDir,"%s\\%s\\%s",GetRootPath(),szSubDir,szSubDir2);
	if (!m_oBaseLogger.Init(LOG_MODE_DAY_DIR_DIVIDE,0,szDir,szLogFileName))
	{
		return false;
	}
	
	//bool Init(INT32 nMode, UINT32 dwID,	const char* pszDir, const char* pszName);
	//if (NULL == m_pLog)
	//{
	//	m_pLog = SDErrorGetModule(&SDERROR_VERSION);
	//	if (NULL == m_pLog)
	//	{
	//		return false;
	//	}

	//	if (!m_pLog->InitErrorModule())
	//	{
	//		return false;
	//	}
	//	if (!m_pLog->SetOutputMode(SDERROR_TYPE_LOG,SDERROR_OUTPUT_LOGFILE))
	//	{
	//		return false;
	//	}
	//	m_pLog->SetLogFormat(SDERROR_LOGFORMAT_DIVIDEBYTIME | SDERROR_LOGFORMAT_DIVIDEBYMODULE,szLogFileName);
	//	

	//	//m_pFileLogger = SDCreateFileLogger(&SDFILELOGGER_VERSION);
	//	return true;
	//}

	return true;
}

bool CLog::InitEx(const char* szSubDir,const char* szLogFileName)
{
	char szDir[1024] = {0};
	sprintf(szDir,"%s\\%s",GetRootPath(),szSubDir);
	if (!m_oBaseLogger.Init(LOG_MODE_DAY_DIVIDE,0,szDir,szLogFileName))
	{
		return false;
	}

	return true;
}
void CLog::UnInit()
{
	//if (NULL != m_pLog)
	//{
	//	Log("日志结束");
	//	m_pLog->Release();
	//	m_pLog = NULL;
	//}

}

void CLog::Log(const char* pszFormat, ...) 
{ 
	char pszDest[1024] = {0};
	va_list args; 
	va_start(args, pszFormat); 
	_vsnprintf(pszDest, 1024, pszFormat, args); 
	va_end(args);
	//if (NULL != m_pLog)
	//{
	//	m_pLog->Log(pszDest,"3dAnimal");
	//}
	m_oBaseLogger.Log(pszDest,sizeof(pszDest));
	m_oBaseLogger.Flush();

}

void CLog::LogWithoutTimeMark(const char* pszFormat, ...) 
{ 
	char pszDest[1024] = {0};
	va_list args; 
	va_start(args, pszFormat); 
	_vsnprintf(pszDest, 1024, pszFormat, args); 
	va_end(args);
	//if (NULL != m_pLog)
	//{
	//	m_pLog->Log(pszDest,"3dAnimal");
	//}
	m_oBaseLogger.LogWithoutTimeMark(pszDest,sizeof(pszDest));
	m_oBaseLogger.Flush();

}

const char* vn(BYTE nType)
{	
	switch(nType&0x0F)
	{
	case 1:
		return "A";
	case 2:
		return "2";
	case 3:
		return "3";
	case 4:
		return "4";
	case 5:
		return "5";
	case 6:
		return "6";
	case 7:
		return "7";
	case 8:
		return "8";
	case 9:
		return "9";
	case 10:
		return "10";
	case 11:
		return "J";
	case 12:
		return "Q";
	case 13:
		return "K";
	case 14:
		return "小王";
	case 15:
		return "大王";
	}
	return "未知牌";
}

const char* cn(BYTE nType)
{
	switch(nType&0xF0)
	{
	case 0x00:
		return "方块";
	case 0x10:
		return "梅花";
	case 0x20:
		return "红桃";
	case 0x30:
		return "黑桃";
	case 0x40:
		return "";
	}
	return "未知颜色";
}

const char* GetBowlName(BYTE cbCardData)
{
	switch(cbCardData)
	{
	case BIG_BUFFER:
		return "大聚宝盆";
	case SMALL_BUFFER:
		return "小聚宝盆";
	case 2:
		return "喜金";
	}
	return "未知聚宝盆";
}

const char* CardTypeName( BYTE cbType )
{

	switch(cbType)
	{
	case 0:
		return "无牛";
	case 1:
		return "牛一";
	case 2:
		return "牛二";
	case 3:
		return "牛三";
	case 4:
		return "牛四";
	case 5:
		return "牛五";
	case 6:
		return "牛六";
	case 7:
		return "牛七";
	case 8:
		return "牛八";
	case 9:
		return "牛九";
	case 10:
		return "牛牛";
	case OX_GOLD:
		return "金牛";
	case OX_DOUBLE_JOKE:
		return "金牛王";
	}
	return "未知牌型";
}