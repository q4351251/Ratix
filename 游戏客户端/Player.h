#pragma once
#include "Stdafx.h"
#include "AnimLayer.h"
#include "GameClient.h"
#include "AnimLayerListener.h"
#include <vector>
using namespace std;


enum PlayerAct																	//标示各种动作
{
	ZHENG_CHANG=0,
	DAI_JI,
	DAI_JI2,
	HAPPY,
	JU_SANG,
	JING_YA,
	MAX_NORMAL_ACT,

	MY_ACT_ONE
};

namespace Sexy
{
	class Graphics;
	class AnimLayer;
	class AnimLayerListener;
}


class CGameClientApp;
class Player: public Sexy::AnimLayerListener
{
public:
    static vector<int> m_vecPlayerIndex;
public:
	Player(void);
	~Player(void);
public:
	//绘制人物
	void					Draw(Sexy::Graphics* g);
	void					Update();
	//void					MouseMove( int x, int y );
private:
	int						m_palyerX;						//人物位置
	int						m_palyerY;
    bool                    m_bReversal;                    //是否翻转
	int					    m_iDirection;					//方向
	PlayerAct				m_PlayerAct;					//人物动作
	int						m_iSex;							//人物性别  1男0女	
public:
	bool					m_bShowUserInfo;				//显示人物信息
    int						m_iPlayerIndex;

	Sexy::AnimLayer			*m_pPlayerActAni;				//人物动画
	Sexy::AnimLayer			*m_pMeActAni1;
	CGameClientApp			*m_pGameApp;
public:
	void					InitPlayerAni(PlayerAct act);

	void					SetPlayerPos(int x,int y);

    void					SetReversal(bool bReversal);

	void					SetDirection(int iDirection);

	void					SetStatus(PlayerAct act);

	void SetPlayerSex(int isex=-1);

	//动画类回调
	void					OnAniFinish(int id);
};
