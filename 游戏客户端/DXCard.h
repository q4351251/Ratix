#pragma once
#include "Stdafx.h"
#include "widget.h"
#include "WidgetManager.h"



#pragma   warning(disable:4305)

#ifndef PI
#define PI 3.1415926535
#endif
//中牌
#define		CARD_V_WIDTH		36
#define		CARD_V_HEIGHT		50
//#define		MAX_FRAME			6
#define		CENTER_FRAME		1
#define		GIVEUPFRAME			8

namespace Sexy
{
	class Graphics;
}

class CGameClientApp;
class CGameClientDlg;
class CGameView;

//回调接口类
class CardCallback
{
public:
	//virtual void OnMoveOneStart(int id) = NULL;		//发牌开始(主要用来播放发牌声音)
	virtual void OnMoveFinish(int id) = NULL;			//发牌完成
	//virtual void OnHideFinish(int id) = NULL;			//牌隐藏完成
	//virtual void OnFlyFinish(int id) = NULL;			//放弃的牌已经飞到荷官面前
};


class CCard
{
public:
	CCard(void);
	CCard(char cCard);
	~CCard(void);
public:
	int                         GetType();
	int                         GetVaule();
	void                        SetCard(char cCard);
	void                        SetSelect(bool bSelect);
	bool                        IsSelect();
	void                        SetMouseSelect(bool bSelect);
	bool                        IsMouseSelect();
	void                        SetCardPos(int x,int y);
	BYTE                        GetCard(){return m_cCard;}
	void                        SetMark(int iMark);
	int                         GetMark(){return m_iMark;}    

public:
	BYTE                        m_cCard;           //牌
	bool                        m_bSelect;         //是否选中
	bool                        m_bMouseSelect;    //鼠标选中
	int                         m_iMark;           //默认0,进贡1,回贡2
	int                         m_CardX;		   //牌的位置
	int                         m_CardY;   
	int							m_iNowFrame;		//当前帧数
	int							m_iTotalFrame;		//总帧数
};

class CGameCard:public Sexy::Widget
{
public:
	CGameCard(void);
	~CGameCard();

	void				        Draw(Sexy::Graphics* g);										//绘制函数
	void                        DrawMeCard(Sexy::Graphics* g);                                  //画自己的牌
	void                        DrawOtherCard(Sexy::Graphics* g);                               //画其他人的牌
	virtual void				MouseMove(int x, int y);										//鼠标移动
	virtual void				MouseDrag(int x, int y);										//鼠标拖拽
	virtual void				MouseDown(int x, int y, int theClickCount);		                //鼠标按下
	virtual void	            MouseUp(int x, int y, int theClickCount);	                    //鼠标抬起
	void                        SetGameView(CGameView *pGameView);

	DWORD                       SetCardData(std::vector<CCard> &Card,const BYTE bCardData[], DWORD dwCardCount);    //设置牌
	//初始化牌
	DWORD						InitCardData(WORD wViewChair,const BYTE bCardData[], DWORD dwCardCount); 
	//重置数据
	void						Reset();
	//动画时钟
	void						AniTimer();	
	//动画帧函数
	bool						CardAddFrame(int id);
	//设置回调接口
	void						setCallBack(CardCallback * pCallBack);
	//设置发牌动画
	void						InitSendCardAni(int id);
	//是否全部牌都发完
	bool                        IsAllFinish();
	//设置摊牌
	void						SetOutCard(int iIndex, BYTE cbOutCard[]);
	//已出自己的牌
	void						RemoveMyAllCard();
	//设置结算扑克
	void						SetResultCardData();
	//牌消失动画帧运算
	void						CardDisAppearFrame();			
	//设置牌消失动画播放
	void						SetCardDisAppear(bool bDis);					

	void                        RemoveHandCardDate(BYTE bCardData[]);                           //去掉手牌
	bool                        SetShootCard(const BYTE bCardData[], DWORD dwCardCount);
	void                        SetHandCardShoot(BYTE bCardData[],WORD wCardCount);             //设置自己手牌弹起
	int                         GetHandCardShoot(BYTE bCardData[]);                             //获得弹起的手牌
	void                        SetPayTruibuteShoot( BYTE bCardData,WORD wCardCount );

	WORD                        GetOutCard( CCard cbCardData[], WORD wBufferCount );
	//设置出牌
	bool	                    SetOutCardData(const BYTE cbOutCardData[], WORD wOutCardCount,WORD wViewChairID,WORD wOutCardShowCount=0);
	void                        SetHandAllShoot(bool bSelect);                                   //设置所有的牌是否弹起

	static void					DrawTextString(Sexy::Graphics* g, const SexyString& theString, int theX, int theY, int iWidth,int itheJustification, Sexy::Color crText, Sexy::Color crFrame);

	std::vector<CCard>          m_vecHandCard[GAME_PLAYER];                                                  //自己的手牌
	std::vector<CCard>          m_vecFaceCard;                                                  //对家的手牌
	std::vector<CCard>          m_vecSendCard[GAME_PLAYER];                                     //出的牌
	std::vector<CCard>			m_vecSendCard2[GAME_PLAYER];									//出的牌2
	std::vector<CCard>			m_vecResultCard[GAME_PLAYER];									//结算扑克
						

	WORD						m_cbOtherHandCard[GAME_PLAYER];						            //其他人的手牌
	

	int							m_iTransNow[MAX_COUNT];						//当前透明度值
	bool			            mLeftDown;
	bool			            mRightDown;
	bool			            mMiddleDown;
	int				            mMouseX, mMouseY;
	int				            mMouseDownX, mMouseDownY;
	CardCallback				*m_pCallBack;
	bool						m_bSendCard;
	bool						m_bShow;									//是否显示初始手牌
	int							m_iDisTrans;								//牌消失动画透明度
	bool						m_bDisappear;								//牌消失动画播放
private:
	CGameClientApp              *m_pGameApp;
	CGameClientDlg              *m_pGameDlg;                                         
	CGameView                   *m_pGameView;
	CGameLogic					*m_GameLogic;

	Sexy::SysFont				*m_pFont10WeiRuanYaHei;											//10号微软雅黑
	
	//test
	int							m_iCardx;
	int							m_iCardy;
public:
	void						SetPos(int x, int y);

	virtual	void				Update();

};
