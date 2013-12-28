#pragma once
#include "Widget.h"
#include "ButtonListener.h"
#include "ButtonWidget.h"
#include "AnimLayer.h"
#include "DXCard.h"
#include "GameClient.h"
#include "AnimLayerListener.h"

namespace Sexy
{
	class Graphics;
	class Color;
	class ButtonWidget;
	class WidgetManager;
	class Timer;
	class GameTimer;
	class SysFont;
	class AnimLayer;
	class ListWidget;
}

class CGameClientApp;
class CGameClientDlg;
class CGameView;

class CMyBasicInfo:public Sexy::Widget, public Sexy::ButtonListener, public Sexy::AnimLayerListener
{
public:

	void				        Draw(Sexy::Graphics* g);										//绘制函数	
	
	void						DrawBaseInfo( Sexy::Graphics* g );
	//设置座位				
	void						SetMyChair(WORD wMeChair);		
	//设置状态
	void						SetNotShow(bool bShow);
	//获得等级索引
	WORD						GetUserOrder( LONG lExperience );	
	//获得称号
	WORD						GetDescription(LONGLONG lBuffer);
	//设置view
	void						SetGameView(CGameView *pGameView);
	CMyBasicInfo(void);
	~CMyBasicInfo(void);
	
	Sexy::ButtonWidget			*m_pBtnAddChip;


public:
	//测试
 	int							m_ix, m_iy;
 	void						SetPosition(int x , int y);

	virtual void				ButtonDepress( int theId );

	CGameClientApp              *m_pGameApp;
	CGameClientDlg              *m_pGameDlg;                                         
	CGameView                   *m_pGameView;
	
	WORD						m_wMeChairID;
	Sexy::SysFont				*m_pFont10WeiRuanYaHei;						//10号微软雅黑
	Sexy::SysFont				*m_pFont9WeiRuanYaHei;						//9号微软雅黑
	
};
