#ifndef __TIMER_LISTENER_H_
#define __TIMER_LISTENER_H_



namespace Sexy
{

//定时器的监听类
class TimerListener
{
public:
	virtual	void OnTimer(int id){};
};

}





#endif