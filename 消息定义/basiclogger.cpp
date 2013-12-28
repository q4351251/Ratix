#include "stdafx.h"
#include "basiclogger.h"
#include "strhelper.h"
#include "log.h"

bool  SGSYS_CreateDirectory(const char* pszDir)
{
	char szPath[MAX_PATH];
	lstrcpyn(szPath, pszDir, sizeof(szPath));

	char* p1 = szPath;

	while(*p1)
	{
		if('/' == *p1) *p1 = '\\';
		p1++;
	}

	if( ((szPath[0] & 0x80) == 0) && (szPath[1] == ':') )
	{
		p1 = szPath+2;

		//d:
		if('\0' == *p1)
			return true;

		if('\\' == *p1)
		{
			p1++;
			if('\0' == *p1)
				return true;
		}
	}
	else
	{
		p1 = szPath;
	}

	do
	{
		p1 = strchr(p1, '\\');
		if(p1 != NULL)
			*p1 = '\0';

		if(FALSE == CreateDirectory(szPath, NULL))
		{
			if(GetLastError() != ERROR_ALREADY_EXISTS)
			{
				return false;
			}
		}
		if(p1 != NULL)
			*p1++ = '\\';
	}while(p1 != NULL);

	return true;
}

void  SGSYS_GetLocalTime(UINT32 dwTime,SLocalTime* pstLocalTime)
{
	if (dwTime == 0)
	{
		SYSTEMTIME  stTime;
		GetLocalTime(&stTime);

		pstLocalTime->m_nYear   = stTime.wYear;
		pstLocalTime->m_nMon    = stTime.wMonth;
		pstLocalTime->m_nMday   = stTime.wDay;
		pstLocalTime->m_nHour   = stTime.wHour;
		pstLocalTime->m_nMin    = stTime.wMinute;
		pstLocalTime->m_nSec    = stTime.wSecond;
		pstLocalTime->m_nWday   = stTime.wDayOfWeek;
	}
	else
	{
		struct tm* pstTime = localtime((time_t*)&dwTime);

		pstLocalTime->m_nSec	= pstTime->tm_sec;
		pstLocalTime->m_nMin	= pstTime->tm_min;
		pstLocalTime->m_nHour	= pstTime->tm_hour;
		pstLocalTime->m_nMday	= pstTime->tm_mday;
		pstLocalTime->m_nMon	= pstTime->tm_mon + 1;
		pstLocalTime->m_nYear	= pstTime->tm_year + 1900;
		pstLocalTime->m_nWday	= pstTime->tm_wday; 
	}
}

using namespace SGDP;

#define MAX_LOGFILE_SIZE	(1024*1024*1024)

CBasicLogger::CBasicLogger()
{
	m_nLogMode	= LOG_MODE_SINGLE_FILE;;
	m_dwID		= 0;
	ZeroString(m_szDir);
	ZeroString(m_szName);
	m_fpFile	= NULL;
	m_bDirty	= false;
	m_nSubFile	= 0;

	memset(&m_stTime, '\0', sizeof(m_stTime));
}

CBasicLogger::~CBasicLogger()
{
	if(m_fpFile != NULL)
		fclose(m_fpFile);
}

bool CBasicLogger::Init(INT32 nMode, UINT32 dwID, const char* pszDir, const char* pszName)
{
	m_nLogMode	= nMode;
	m_dwID		= dwID;
	StrSafeCopy(m_szDir, pszDir);
	StrSafeCopy(m_szName, pszName);

	char* p = m_szDir;
	while(*p)
	{
#ifdef WIN32
		if('/' == *p) *p = '\\';
#else
		if('\\' == *p) *p = '/';
#endif
		p++;
	}

	size_t nLastPos = StrNLen(m_szDir, sizeof(m_szDir))-1;
	if( ('/' == m_szDir[nLastPos]) || ('\\' == m_szDir[nLastPos]) )
	{
		m_szDir[nLastPos] = '\0';
	}
	//SGSYS_CreateDirectory(m_szDir);

	//SGSYS_GetLocalTime(0, &m_stTime);
	//char szFile[MAX_PATH];
	//ZeroString(szFile);
	//_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d.log", 
	//	m_szDir, m_szName,
	//	m_stTime.m_nYear, m_stTime.m_nMon, m_stTime.m_nMday);
 //   
	//m_fpFile = fopen(szFile, "a");
	
	m_nSubFile = 0;

	_UpdateFilePointer();
	return true;
}

bool CBasicLogger::Log(const char* pData, INT32 nLen)
{
	_UpdateFilePointer();

	if(NULL == m_fpFile)
		return false;

	fprintf(m_fpFile, "%04d-%02d-%02d %02d:%02d:%02d %s\n", 
		m_stTime.m_nYear, m_stTime.m_nMon, m_stTime.m_nMday,
		m_stTime.m_nHour, m_stTime.m_nMin, m_stTime.m_nSec, pData);

	m_bDirty = true;

	return true;
}

bool CBasicLogger::LogWithoutTimeMark(const char* pData, INT32 nLen)
{
	_UpdateFilePointer();

	if(NULL == m_fpFile)
		return false;

	fprintf(m_fpFile, "%s\n", pData);

	m_bDirty = true;

	return true;
}

void CBasicLogger::_UpdateFilePointer()
{
	char szFile[MAX_PATH];
	SLocalTime stCurTime;
	SGSYS_GetLocalTime(0, &stCurTime);

	//
	// 如果文件超过1G，需要分割文件，否则可能会导致程序崩溃
	//
	if(m_fpFile != NULL)
	{
		if(ftell(m_fpFile) > MAX_LOGFILE_SIZE)
		{
			m_nSubFile++;

			ZeroString(szFile);
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d_%d.log", 
				m_szDir, m_szName,
				stCurTime.m_nYear, stCurTime.m_nMon, stCurTime.m_nMday, m_nSubFile);

			fclose(m_fpFile);
			m_fpFile = fopen(szFile, "a");
		}
	}

	if(LOG_MODE_DAY_DIVIDE == m_nLogMode)
	{
		if(stCurTime.m_nYear != m_stTime.m_nYear 
			|| stCurTime.m_nMon != m_stTime.m_nMon 
			|| stCurTime.m_nMday != m_stTime.m_nMday)
		{			
			ZeroString(szFile);
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d.log", 
				m_szDir, m_szName,
				stCurTime.m_nYear, stCurTime.m_nMon, stCurTime.m_nMday);

			if(m_fpFile != NULL)fclose(m_fpFile);
			m_fpFile = fopen(szFile, "a");
			m_nSubFile = 0;
		}
	}
	else if(LOG_MODE_MONTH_DIVIDE == m_nLogMode)
	{
		if(stCurTime.m_nYear != m_stTime.m_nYear 
			|| stCurTime.m_nMon != m_stTime.m_nMon)
		{			
			ZeroString(szFile);
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s_%04d_%02d_%02d.log", 
				m_szDir, m_szName,
				stCurTime.m_nYear, stCurTime.m_nMon, stCurTime.m_nMday);

			if(m_fpFile != NULL)fclose(m_fpFile);
			m_fpFile = fopen(szFile, "a");
			m_nSubFile = 0;
		}
	}
	else if (LOG_MODE_DAY_DIR_DIVIDE == m_nLogMode)
	{
		if(stCurTime.m_nYear != m_stTime.m_nYear 
			|| stCurTime.m_nMon != m_stTime.m_nMon 
			|| stCurTime.m_nMday != m_stTime.m_nMday)
		{	
			char szNewDir[128] = {0};
			_snprintf(szNewDir,127,"%s/%d-%02d-%02d",m_szDir,stCurTime.m_nYear,stCurTime.m_nMon,stCurTime.m_nMday);
			SGSYS_CreateDirectory(szNewDir);

			ZeroString(szFile);
			_snprintf(szFile, sizeof(szFile)-1, "%s/%s.log", 
				szNewDir, m_szName);

			if(m_fpFile != NULL)fclose(m_fpFile);
			m_fpFile = fopen(szFile, "a");
			m_nSubFile = 0;
		}
	}

	m_stTime = stCurTime;
}

void CBasicLogger::Flush()
{
	if(m_bDirty)
	{
		fflush(m_fpFile);
		m_bDirty = false;
	}
}

bool CBasicLogger::LogEx( const char* pData, INT32 nLen )
{
    _UpdateFilePointer();

    if(NULL == m_fpFile)
        return false;

    fprintf(m_fpFile, "%04d-%02d-%02d %02d:%02d:%02d ", 
        m_stTime.m_nYear, m_stTime.m_nMon, m_stTime.m_nMday,
        m_stTime.m_nHour, m_stTime.m_nMin, m_stTime.m_nSec);//, pData);
    for (int i = 0; i < nLen; i++)
    {
        fprintf(m_fpFile, "%x ", pData[i]);
    }
    //fwrite(pData, sizeof(char), nLen, m_fpFile);
    fprintf(m_fpFile, "\n");
    m_bDirty = true;

    return true;
}