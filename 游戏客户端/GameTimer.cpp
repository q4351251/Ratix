#include "Stdafx.h"
#include "GameTimer.h"
#include "GameTimerListener.h"

using namespace Sexy;

GameTimer::GameTimer(GameTimerListener *pGameTimerListener)
{
	m_pGameTimerListener = pGameTimerListener;
}

GameTimer::~GameTimer(void)
{
}


void GameTimer::SetTimer(int nTimerId, unsigned long dwElapse, WORD wChairID, UINT nElapse)
{
	unsigned long dwTickCount = GetTickCount();
	std::map<int, GameTimerInfoDef>::iterator pos;
	pos = m_GameTimeMap.find(nTimerId);
	if (pos != m_GameTimeMap.end())
	{
		pos->second.nTimerId = nTimerId;
		pos->second.dwElapse = dwElapse;
		pos->second.dwTickCount = dwTickCount;
		pos->second.bAvalid = true;
		pos->second.wChairID = wChairID;
		pos->second.nElapse = nElapse;
	}
	else
	{
		GameTimerInfoDef timeInfo;
		timeInfo.nTimerId = nTimerId;
		timeInfo.dwElapse = dwElapse;
		timeInfo.dwTickCount = dwTickCount;
		timeInfo.bAvalid = true;
		timeInfo.wChairID = wChairID;
		timeInfo.nElapse = nElapse;
		m_GameTimeMap.insert(std::make_pair<int, GameTimerInfoDef>(nTimerId, timeInfo)) ;
	}
}

void GameTimer::KillTimer( int nTimerId )
{
	std::map<int, GameTimerInfoDef>::iterator pos;
	pos = m_GameTimeMap.find(nTimerId);
	if (pos != m_GameTimeMap.end())
	{
		pos->second.bAvalid = false;
	}
}

void GameTimer::Update()
{	
	std::map<int, GameTimerInfoDef>::iterator posTime;
	for (posTime=m_GameTimeMap.begin(); posTime!=m_GameTimeMap.end(); ++posTime)
	{
        unsigned long dwTickCount = GetTickCount();
		if (posTime->second.bAvalid)
		{
			if (posTime->second.dwElapse <= dwTickCount - posTime->second.dwTickCount)
			{
				posTime->second.dwTickCount = posTime->second.dwElapse + posTime->second.dwTickCount;

				if (posTime->second.nElapse != -1)
				{
					posTime->second.nElapse--;
				}
				m_pGameTimerListener->OnTimer(posTime->second.nTimerId, posTime->second.wChairID, posTime->second.nElapse);
				if (posTime->second.nElapse == 0)
				{
					KillTimer(posTime->second.nTimerId);
				}
			}
		}
	}
}
