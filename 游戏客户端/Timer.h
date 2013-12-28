#ifndef __TIMER_H_
#define __TIMER_H_

#include <map>
#include "TimerListener.h"

namespace Sexy
{

typedef struct TimerInfo
{
	int nTimerId;
	unsigned long dwElapse;
	unsigned long dwTickCount;
	bool bAvalid;
} TimerInfoDef;

class  Timer
{

public:
	void SetTimer(int nTimerId, unsigned long dwElapse);
	void KillTimer(int nTimerId);
	void Update();


public:
	std::map<int, TimerInfoDef> m_TimeMap;

	Timer(TimerListener *pTimerListener);
	~Timer();

private:
	TimerListener			*m_pTimerListener;
}; //class Timer

}
#endif //__TIMER_H_