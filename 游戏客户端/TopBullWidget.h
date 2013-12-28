#pragma once
#include "TimerListener.h"
#include "Res.h"
#include "widget.h"
#include "ButtonListener.h"

class CGameClientApp;

namespace Sexy
{
	class Timer;
	class ButtonWidget;
	class Graphics;
};

//顶部系统公告
class CTopBullWidget :
	public Sexy::Widget, public Sexy::TimerListener, public Sexy::ButtonListener
{
public:
	//添加公告
	void AddString(LPCTSTR pszString);


public:
	CTopBullWidget(void);
	~CTopBullWidget(void);

	CGameClientApp *m_pGameApp;

	virtual void Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void Update();									//更新
	virtual void Draw(Sexy::Graphics *g);					//绘图

	Sexy::Timer		*m_pTimer;
	enum { TIM_BULL_SHOW_ID = 1,  TIM_BULL_HIDE_ID};
	virtual void OnTimer(int nTimerId);						//定时器方法

	enum { BTN_HIDE_ID = 1};
	virtual void ButtonDepress(int theId);
	void OnBtnHide();										//隐藏按钮响应

	Sexy::ButtonWidget *m_pBtnHide;							//隐藏按钮
	Sexy::SysFont *m_pFontHei;								//宋体

	std::vector<std::string> m_strBull;						//消息容器

	wchar_t m_szBullContent[128];							//公告



};
