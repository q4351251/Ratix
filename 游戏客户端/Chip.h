#pragma once
#include "Stdafx.h"
#include "widget.h"
#include "WidgetManager.h"
#include "GameClient.h"
#include "AnimLayer.h"

#pragma   warning(disable:4305)

#ifndef PI
#define PI 3.1415926535
#endif
	
enum ChipType
{
	CHIP_100 = 0,
	CHIP_1000,
	CHIP_5000,
	CHIP_10000,

	MAX_CHIP_TYPE
};

enum enDirection
{
	d_x,
	d_y
};

struct ChipInfo
{
	ChipType	type;				//筹码种类
	CPoint		ptNow;				//当前坐标
	CPoint		ptBegin;			//起点坐标
	CPoint		ptEnd;				//终点坐标
	bool		bFly;				//是否在飞
	WORD		wFrameNow;			//当前帧数
	WORD		wTotalFrame;		//总帧
	bool		bCollection;		//是否收集
	WORD		wWinnerID;			//
	INT			iIndex;				//排序
	bool		bAddSpeed;			//加速
};


struct Shadow
{
	bool		bShow;
	CPoint		ptNow;				//当前坐标
	CPoint		ptBegin;			//起点坐标
	CPoint		ptEnd;				//终点坐标
	bool		bFly;				//是否在飞
	WORD		wFrameNow;			//当前帧数
	WORD		wTotalFrame;		//总帧
};

//回调接口类
class ChipCallback
{
public:
	virtual void OnYaDiFinish() = NULL;		//发牌开始(主要用来播放发牌声音)
};

//筹码类
class DXChip:public Sexy::Widget
{
public:
	DXChip(void);
	~DXChip(void);

	//测试
	void setPos(int x, int y);
	//绘制函数
public:
	//绘制牌
	void Draw( Sexy::Graphics* g );
	//绘制桌面筹码
	void DrawDeskChip1(Sexy::Graphics* g, WORD index, LONGLONG lScrapChip);				
private:
	


public:
	LONGLONG GetChipScore(ChipType type);

	//移动辅助函数
	float GetInitSpeed(ChipInfo chipInfo , enDirection EnDirection);
	float GetAddSpeed(ChipInfo chipInfo , enDirection EnDirection);
	float GetCoordinate(ChipInfo chipInfo , enDirection EnDirection);

	//功能函数
	//下注
	void AddChips(LONGLONG lTotalPoint, int iPos , bool bAllIn=false);	
	//重置
	void Reset();
	//设置回调寒素
	void SetCallBack(ChipCallback * pCallBack);
	//筹码变化时钟
	void AniTimer();
	//计算位置
	void ChipAddFrame(ChipInfo& chipinfo,float fAngle=0);

	virtual void Update();

	Sexy::Image							*m_BigChipImg[4];
	Sexy::Image							*m_SmallChipImg[4];
	//界面位置变量
	BYTE								m_cbDeskHeapCount[5][4];				//每堆的情况
	CPoint								m_ptDesk[GAME_PLAYER][MAX_CHIP_TYPE];	//每个玩家桌面筹码坐标位置
	BYTE								m_iAddChipKind1[5][2];
	BYTE								m_iAddChipKind2[5][2];
	CPoint								m_ptBegin[GAME_PLAYER];					//每个玩家筹码的起始位置
	CPoint								m_ptEnd[GAME_PLAYER];					//每个玩家筹码的最后位置
	std::vector<Sexy::AnimLayer*>		m_vecChipAni;							//筹码动画	
	// 	//筹码变量
	std::vector<ChipInfo>				m_vecChip[GAME_PLAYER];					//所有筹码
	Shadow								m_Shadow[GAME_PLAYER][2];

	CGameClientApp						*m_pGameApp;
	CGameClientDlg						*m_pGameDlg;                                         
	CGameView							*m_pGameView;
private:
	int									m_ix;
	int									m_iy;
	ChipCallback						*m_pCallback;
	bool								m_bMoveFinish;

};

