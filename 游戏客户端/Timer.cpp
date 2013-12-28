#include "Stdafx.h"
#include "Timer.h"

using namespace Sexy;

//构造函数
Timer::Timer(TimerListener *pTimerListener)
{
	m_pTimerListener = pTimerListener;
	m_TimeMap.clear();
}

//析构函数
Timer::~Timer()
{
	m_TimeMap.clear();
}

//设置定时器
void Timer::SetTimer(int nTimerId, unsigned long dwElapse)
{
	unsigned long dwTickCount = GetTickCount();
	std::map<int, TimerInfoDef>::iterator pos;
	pos = m_TimeMap.find(nTimerId);
	if (pos != m_TimeMap.end())
	{
		pos->second.nTimerId = nTimerId;
		pos->second.dwElapse = dwElapse;
		pos->second.dwTickCount = dwTickCount;
		pos->second.bAvalid = true;
	}
	else
	{
		TimerInfoDef timeInfo;
		timeInfo.nTimerId = nTimerId;
		timeInfo.dwElapse = dwElapse;
		timeInfo.dwTickCount = dwTickCount;
		timeInfo.bAvalid = true;
		m_TimeMap.insert(std::make_pair<int, TimerInfoDef>(nTimerId, timeInfo)) ;
	}
}

//杀掉定时器
void Timer::KillTimer(int nTimerId)
{
	std::map<int, TimerInfoDef>::iterator pos;
	pos = m_TimeMap.find(nTimerId);
	if (pos != m_TimeMap.end())
	{
		pos->second.bAvalid = false;
	}
}

void Timer::Update()
{
	unsigned long dwTickCount = GetTickCount();
	std::map<int, TimerInfoDef>::iterator posTime;
	for (posTime=m_TimeMap.begin(); posTime!=m_TimeMap.end(); ++posTime)
	{
		if (posTime->second.bAvalid)
		{
			if (posTime->second.dwElapse <= dwTickCount - posTime->second.dwTickCount)
			{
				posTime->second.dwTickCount = posTime->second.dwElapse + posTime->second.dwTickCount;
				m_pTimerListener->OnTimer(posTime->second.nTimerId);
			}
		}
	}
}
