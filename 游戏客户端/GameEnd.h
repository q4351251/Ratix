#pragma once
#include "Widget.h"
#include "ButtonListener.h"
#include "AnimLayer.h"
#include "DXCard.h"
#include "GameClient.h"
#include "AnimLayerListener.h"

namespace Sexy
{
    class Graphics;
    class ButtonWidget;
    class WidgetManager;
	class SysFont;
	class AnimLayer;
	class AnimLayerListener;    
}

class CGameClientApp;
class CGameClientDlg;
class CGameView;
class CCard;

enum
{
	BT_CHANGE_DESK=0,
	BT_CONTINUE,
	BT_EXIT
};

class GameEnd:public Sexy::Widget,public Sexy::ButtonListener,public Sexy::AnimLayerListener
{
public:
    GameEnd(void);
    ~GameEnd(void);

    void                Draw(Sexy::Graphics* g);
    void                Update();
    virtual void        MouseDown(int x, int y, int theClickCount);                        //鼠标按下
    virtual void        MouseDrag(int x, int y);                                           //鼠标拖动 
	virtual void		MouseMove(int x, int y);
    virtual void        MouseUp(int x, int y, int theClickCount);                          //鼠标松开
    virtual void        KeyDown(Sexy::KeyCode theKey);                                     //键盘按下
    virtual void        ButtonDepress(int theId);                                          //按钮按下
    virtual void        AddedToManager(Sexy::WidgetManager* theWidgetManager);
    virtual void        Resize(int theX, int theY, int theWidth, int theHeight);           //设置矩形


	//积分信息
protected:
	std::vector<CCard>	m_vecResultCard[GAME_PLAYER];							//剩余的牌
	BYTE				m_cbOX[GAME_PLAYER];									//牛牛信息



	WORD				m_RemainCardCount[GAME_PLAYER];							//剩余扑克数量
	LONGLONG			m_EndScore[GAME_PLAYER];								//结算分数
	LONGLONG			m_EndScoreTemp[GAME_PLAYER];
	WORD				m_wBanker;												//地主    
	int					m_wBeiShu;												//倍数
	WORD				m_wTimerNum;											//时钟数字
    int                 m_iScore;                                               //鸡蛋等级积分



	//功能函数
public:

    static int m_x,m_y;
    static bool m_bCtrlDown;

    void    SetGameView(CGameView *pGameView);
	void	InitWidget();
	//设置结算牌
	void	SetGameEndResultCard(std::vector<CCard> result[]);
	//设置牛牛信息
	void	SetOxData(BYTE cbOx[GAME_PLAYER]);

	//设置倒计时钟数字
	void	SetTimerNum(WORD wNum);
    bool	SetEndScore(LONG lScore[]);
	void	SetShowScore(bool bShowScore);
    void    ResetPlace();
	//画数字的方法 theJustification 对齐方式 -1为左对齐 0为居中对齐(x就为中心坐标了),1为右对齐(x就为最右边的坐标)
	void	DrawNum2( Sexy::Graphics* g, Sexy::Image *img, LONGLONG iNum, int x, int y, int theJustification /*= 0*/, int iSign /*= 0*/);
	//时钟动画
	void	InitTimerAni();
	void	InitResultAni(bool bWin);

private:
    CGameClientApp              *m_pGameApp;
    CGameClientDlg              *m_pGameDlg;
    CGameView                   *m_pGameView;
	bool						m_bLeftDown;
	bool						m_bShowEndScore;

	Sexy::ButtonWidget			*m_pBtnExit;													//退出按钮
	Sexy::ButtonWidget			*m_pBtnChangeDesk;												//换桌按钮
	Sexy::ButtonWidget			*m_pBtnContinue;												//继续按钮

	//字体
	Sexy::SysFont				*m_pFont10WeiRuanYaHei;											//10号微软雅黑
	Sexy::AnimLayer				*m_pTimerAni;													//时间动画

};
