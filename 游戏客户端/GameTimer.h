#pragma once
#include <map>

namespace Sexy
{
	typedef struct GameTimerInfo
	{
		int nTimerId;					//定时器ID
		unsigned long dwElapse;			//时间间隔
		unsigned long dwTickCount;		
		bool bAvalid;					//是否可用
		unsigned short wChairID;		//玩家位置
		unsigned int nElapse;			//定时器次数
	} GameTimerInfoDef;

	class GameTimerListener;
	class GameTimer
	{
	public:
		GameTimer(GameTimerListener *pGameTimerListener);
		~GameTimer(void);
	private:
		std::map<int, GameTimerInfoDef> m_GameTimeMap;
		GameTimerListener			*m_pGameTimerListener;
	public:
		//定时器ID 时间间隔 玩家位置 定时器次数
		void SetTimer(int nTimerId, unsigned long dwElapse, WORD wChairID, UINT nElapse);
		void KillTimer(int nTimerId);
		void Update();
		
	};
}
