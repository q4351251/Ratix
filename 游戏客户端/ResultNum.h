#pragma once
#include "Stdafx.h"
#include "widget.h"
#include "WidgetManager.h"

namespace Sexy
{
	class Graphics;
}

class CGameClientApp;
class CGameClientDlg;
class CGameView;

class CResultNum:public Sexy::Widget
{
public:

	void				        Draw(Sexy::Graphics* g);										//绘制函数	
	
	
	//设置分数					
	void						SetResultNum(LONGLONG lResultNum[GAME_PLAYER], BYTE cbPlayStatus[GAME_PLAYER]);		
	//设置状态
	void						SetNowShow(bool bShow);
	//动画时钟
	void						AniTimer();	
	//动画帧函数
	void NumAddFrame();

	CResultNum(void);
	~CResultNum(void);




public:
	//测试
 	int							m_ix, m_iy;
 	void						SetPosition(int x , int y);

 	bool						m_bShow;
 	BYTE						m_cbViewStatus[GAME_PLAYER];		//玩家状态
 	LONGLONG					m_lResultNum[GAME_PLAYER];			//结算分数
	int							m_iTrans;							//透明度		
	
	CGameClientApp              *m_pGameApp;
	CGameClientDlg              *m_pGameDlg;                                         
	CGameView                   *m_pGameView;
	
};
