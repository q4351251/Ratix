#ifndef BASICLOGGER_H
#define BASICLOGGER_H

//namespace SGDP
//{
	typedef struct 
	{
		UINT32 m_dwSec;     ///< seconds
		UINT32 m_dwUsec;    ///< microsecond(1/1,000,000 second)
	}STimeVal;

	typedef struct
	{
		INT32 m_nSec;       ///< Seconds.     [0-60]
		INT32 m_nMin;       ///< Minutes.     [0-59]
		INT32 m_nHour;      ///< Hours.       [0-23]
		INT32 m_nMday;      ///< Day.         [1-31]
		INT32 m_nMon;       ///< Month.       [0-11]
		INT32 m_nYear;      ///< Year
		INT32 m_nWday;      ///< Day of week. [0-6]
	}SLocalTime;
//}

class CBasicLogger
{
public:
	CBasicLogger();
	~CBasicLogger();

	bool Init(INT32 nMode, UINT32 dwID,	const char* pszDir,const char* pszName);
	bool Log(const char* pData, INT32 nLen);
	bool LogWithoutTimeMark(const char* pData, INT32 nLen);
    bool LogEx(const char* pData, INT32 nLen);
	void Flush();

protected:
	void _UpdateFilePointer();  //根据时间和日志文件的纪录方式决定是否要开启一个新的日志文件

protected:
	INT32		m_nLogMode;
	UINT32		m_dwID;
	char		m_szDir[MAX_PATH];
	char		m_szName[128];
	FILE*		m_fpFile;
	bool		m_bDirty;
	INT32		m_nSubFile;
	SLocalTime	m_stTime;
};

#endif
