#pragma once
#include "Widget.h"
#include "ButtonListener.h"
#include "TimerListener.h"
#include "ListListener.h"
#include "AnimLayer.h"
#include "GameTimerListener.h"
#include "AnimLayerListener.h"
#include "Player.h"
#include "Chip.h"
#include "DXCard.h"
#include "GameEnd.h"
#include "ResultNum.h"
#include "ListWidget.h"
#include "MyBasicInfo.h"

//常量定义
#define						   MAX_DOUBLEHEAP_COUNT					32		//初始牌摞数
#define						   DOUBLE_COUNT							2		//一摞牌个数


//按钮标识
enum
{
	BT_EXCHANGECHIP_SPACE1 = 0,
	BT_EXCHANGECHIP_SPACE2,
	BT_EXCHANGECHIP_SPACE3,
	BT_EXCHANGECHIP_SPACE4,
	BT_EXCHANGECHIP_SPACE5,

	BT_OPEN_CARD,			//摊牌
	BT_HINT_CARD,			//提示
	BT_RELIST_CARD,			//重排
	BT_ADD_CHIP,			//补充筹码
	BT_CUI_PAI,				//催牌
	BT_TUOGUAN,				//托管
	BT_CANCEL_TUOGUAN,		//取消托管
};
namespace Sexy
{
	class Graphics;
	class Color;
	class ButtonWidget;
	class Timer;
	class GameTimer;
	class SysFont;
	class AnimLayer;
	class ListWidget;
}


class CGameClientApp;
class CGameClientDlg;
class CTopBullWidget;

class CGameView : public Sexy::Widget, public Sexy::ButtonListener, public Sexy::ListListener,public Sexy::TimerListener, public Sexy::AnimLayerListener,
	public Sexy::GameTimerListener,public ChipCallback,public CardCallback
	
{
public:
	CGameView(void);
	~CGameView(void);
	virtual void				Draw(Sexy::Graphics* g);										//绘制函数
	virtual void				Update();														//刷新函数
	virtual void				MouseLeave();													//鼠标离开
	virtual void				MouseEnter();													//鼠标进入
	virtual void				MouseMove(int x, int y);										//鼠标移动
	virtual void				MouseDrag(int x, int y);										//鼠标拖拽
	virtual void				MouseDown(int x, int y, int theBtnNum, int theClickCount);		//鼠标按下
	virtual void				ButtonDepress(int theId);										//按钮抬起
	virtual	void				KeyDown(Sexy::KeyCode theKey);									//键盘消息
	virtual void				Resize(int theX, int theY, int theWidth, int theHeight);		//大小改变

	int	m_x,m_y,m_index,iAciton, iActor;			//测试用坐标

	void						OnAniFinish(int id);											//动画回调	

	//辅助函数
public:
	void						InitWidget();													//初始化Widget
	void						AddBull(LPCTSTR pszString);										//添加公告
	//重置数据
	void						ResetGameFrame();
	
	//动画播放函数
public:
	//播放等待动画
	void						ShowWaitAni();									
	//播放自动配桌动画
	void						ShowAutoDeskAni(bool bShow=true);									
	//开始播放输赢分动画
	void						ShowMoneyAni(LONGLONG lScore[]);
	//绘制函数
private:
	//绘制数字的方法 theJustification 对齐方式 -1为左对齐 0为居中对齐(x就为中心坐标了),1为右对齐(x就为最右边的坐标)
	static void					DrawNum1(Sexy::Graphics* g, Sexy::Image *img, LONGLONG iNum, int x, int y, int theJustification = 0);
	static void 				DrawTextString(Sexy::Graphics* g, const SexyString& theString, int theX, int theY, int iWidth,int itheJustification, Sexy::Color crText, Sexy::Color crFrame);;
	
	//自定义绘制函数
	void						DrawBg(Sexy::Graphics* g);										//画背景
	void						DrawBaseInfo(Sexy::Graphics* g);								//画信息（自己信息，房间相关信息）
	void						DrawPlayer(Sexy::Graphics* g);									//画玩家
	//画测试
	void						DrawTest(Sexy::Graphics* g);
	//绘制准备
	void                        DrawReady(Sexy::Graphics* g);
	//画桌面筹码
	void						DrawDeskChip(Sexy::Graphics* g);
	//画押注数额
	void						DrawAddScore(Sexy::Graphics *g);
	//画比牌阶段个人的牌型
	void						DrawOxType(Sexy::Graphics *g);
	//画兑换筹码框
	void						DrawExChangeChipDlg(Sexy::Graphics *g);
	//画催牌
	void						DrawUcCard(Sexy::Graphics *g);
	//画其他玩家信息框			
	void						DrawOtherInfo(Sexy::Graphics *g);
	//画摊牌状态
	void						DrawOpenCard(Sexy::Graphics *g);						
	//画聚宝盆信息
	void						DrawBuffer(Sexy::Graphics *g);
	//画喜金牌
	void						DrawReward(Sexy::Graphics *g);
	//画喜金栏提示
	void						DrawRewardHint(Sexy::Graphics *g);



	void						DrawAni(Sexy::Graphics* g);										//画动画			

public:
	void						RemoveAllAni();

public:
	void						OnGameMessage(WPARAM wParam, LPARAM lParam);					//窗口消息
	//网络消息处理
	virtual	bool				OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//游戏准备开始
	bool                        OnSubGameReady(const void * pBuffer, WORD wDataSize);
	//游戏开始
	bool                        OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool						OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//游戏全部结束
	bool						OnSubGameAllFinish(const void * pBuffer, WORD wDataSize);
	//催牌消息处理
	bool						OnSubHurryCard(const void * pBuffer, WORD wDataSize);
	//摊牌消息处理
	bool						OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//游戏空闲
	bool						OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//聚宝盆消息处理
	bool						OnSubBowl(const void * pBuffer, WORD wDataSize);

	//辅助函数
public:
	//兑换筹码 
	void						OnExChip(UINT nIDEvent);
	//显示兑换筹码框
	void						ShowExChip(bool bShow = false);
	//获得等级索引
	WORD						GetUserOrder( LONG lExperience );	
	//获得称号索引
	WORD						GetDescription( LONGLONG lBuffer );	
	//获得称号颜色索引
	WORD						GetDesColor(WORD wIndex);
	//时钟操作
	void						OperaTimer(UINT nTimerID, UINT nElapse ,WORD wChair, BOOL bStart=true);
	//发牌结束
	void						OnSendCardFinish();
	//提示按钮响应
	void						OnBtnHintCard();
	//摊牌按钮响应
	void						OnBtnOpenCard();
	//重排按钮响应
	void						OnBtnReListCard();
	//播放牌型动画
	void						ShowCardAni(BYTE cbCardType, WORD wViewChair);
	//显示催牌框
	void						ShowUcCardDlg(bool bShow = false);
	//催牌消息
	void						OnUcCard(int iIndex);
	//用户进入消息				
	void						OnUserEnter(tagUserData * pUserData, WORD wViewChairID, bool bLookonUser);
	//移动按钮	
	void						MoveButton(int iBtnTag, int iUp);
	//准备后事件
	void						AfterReady();
	//生成喜金元宝数图片
	void						MakeLikeIngotImg();
	
	//测试配牌-------------------------------------------------------------------
	void						ControlCardData();
	
	Sexy::ButtonWidget			*m_pBtnTuoGuan;													//托管按钮
	//Sexy::ButtonWidget			*m_pBtnAddChip;													//补充筹码
private:
	CGameClientApp				*m_pGameApp;													//初始化类
	CGameClientDlg				*m_pGameDlg;													//主逻辑处理类
	Sexy::Timer					*m_pTimer;														//定时器类	
	Sexy::GameTimer				*m_pGameTimer;													//定时器类	
	CGameLogic					*m_pGameLogic;													//游戏逻辑
	DXChip						m_pChip;
	
	
	virtual		void            OnTimer(int id, WORD wChairID, UINT nElapse);					//定时器回调函数
	virtual		void			OnTimer(int id);												//定时器回调函数

	virtual		void			OnYaDiFinish();

	virtual		void			OnMoveFinish( int id );

	virtual		void			DrawOverlay( Sexy::Graphics* g, int thePriority );

	virtual		void			ListClicked( int theId, int theIdx, int theClickCount );

private:
	Sexy::Point					m_ptMouse;														//鼠标坐标
	CTopBullWidget				*m_pTopBullWidget;												//顶部公告

	//字体
	Sexy::SysFont				*m_pFont10Song;													//10号宋体
	Sexy::SysFont				*m_pFont10Hei;													//10号宋体
	
	//按钮变量
	Sexy::ButtonWidget			*m_pBtnExChip[MAX_CHIP_COUNT];													//押注按钮
// 	Sexy::ButtonWidget			*m_pBtnExChip2;													//押注按钮
// 	Sexy::ButtonWidget			*m_pBtnExChip3;													//押注按钮
// 	Sexy::ButtonWidget			*m_pBtnExChip4;													//押注按钮
	Sexy::ButtonWidget			*m_pBtnOpenCard;												//摊牌按钮
	Sexy::ButtonWidget			*m_pBtnHintCard;												//提示
	Sexy::ButtonWidget			*m_pBtnReListCard;												//重排
	Sexy::ButtonWidget			*m_pBtnQuickSub;												//快速喊话
	
	Sexy::ButtonWidget			*m_pBtnCancelTuoGuan;											//取消托管
	

	Sexy::ButtonWidget			*m_pBtnTest;													//测试按钮

	//动画变量
	Sexy::AnimLayer             *m_pDiceAni0;													//骰子动画1
	Sexy::AnimLayer				*m_pDiceAni1;													//骰子动画2
	/*Sexy::AnimLayer				*m_pCardAppearAni[MAX_DOUBLEHEAP_COUNT];						//牌出现动画*/
	Player						*m_pPlayer[GAME_PLAYER];
	Sexy::AnimLayer				*m_pOxAni[11];													//牌型动画
	Sexy::AnimLayer				*m_pAddMoneyAni[GAME_PLAYER];				//加钱动画
	Sexy::AnimLayer				*m_pWaitOtherAni;							//等待其他玩家动画
	Sexy::AnimLayer				*m_pAutoDeskAni;							//自动配桌动画
	Sexy::AnimLayer				*m_pLikeIngotAni;							//喜金动画
	Sexy::AnimLayer				*m_pOffLineAni[GAME_PLAYER];				//断线动画
// 	Sexy::AnimLayer				*m_pJackocur;								//财神路过动画
// 	Sexy::AnimLayer				*m_pJackpotwin;								//聚宝盆动画
	Sexy::AnimLayer				*m_pTuoGuanAni;
	//界面辅助变量
protected:
	bool							m_bShowExChangeChipDlg;					//是否显示兑换筹码框
	bool							m_bGetMsg;								//开启接受网络信息
	bool							m_bOpenCard[GAME_PLAYER];				//是否摊牌
	GameEnd							*m_pGameEnd;							//结算视图
	bool							m_bAddScore;							//是否押注
	bool							m_bShowOxType;							//显示个人牌型
	bool							m_bShowUcCard;							//显示催牌框
	//游戏变量
public:
	WORD							m_wMeChairID;								//自己的实际椅子
	int								m_iMyFaceIndex;								//自己头像索引
	WORD							m_wTimeNum;
	BYTE							m_cbPalyStatus[GAME_PLAYER];			//玩家游戏状态
	BYTE							m_cbHandCard[GAME_PLAYER][MAX_COUNT];	//玩家手牌
	BYTE							m_cbRewardCard[GAME_PLAYER][MAX_COUNT];	//喜金牌
	BYTE							m_cbCardType[GAME_PLAYER];				//玩家牌型
	CResultNum						*m_pResultNum;							//结算分数
	CMyBasicInfo					*m_pMyBasicInfo;						//我的信息
	Sexy::DDImage					*m_pLikeIngotImg[4];					//喜金发放数目字转换成图片


	CGameCard						m_GameCard;								//牌
	CPoint							m_cpTimer;								//时钟位置
	bool							m_bTrustee;								//是否托管

	LONG							m_lBowl;								//聚宝盆
	LONG							m_lLikeIngot;							//喜金

	bool							m_bMouseSelect[GAME_PLAYER];			//鼠标选择玩家]
	int								m_iOtherInfoTrans[GAME_PLAYER];			//玩家信息框透明度
	int								m_iOtherInfofPos[GAME_PLAYER][3];

	Sexy::ListWidget				*m_pListUcCard;							//催牌列表
	WORD						    m_wHurryCardUser;						//催牌玩家
	WORD						    m_wHurryWordID;							//催牌玩家声音ID

	LONGLONG						m_lCellScore;							//单位积分
	LONGLONG						m_lMinbet;								//最小下注
	LONGLONG						m_lUserAddScore[GAME_PLAYER];			//玩家下注
	LONGLONG						m_lChipNum[MAX_CHIP_COUNT];				//筹码数量
	LONG							m_lNewLikeIngot[4];
	LONG							m_lPercentage[MAX_CHIP_COUNT];			//服务费比例
	WORD							m_wServerType;							//服务器类型
	bool							m_bShowBowlHint;						//是否显示聚宝盆提示
	bool							m_bShowLikeIngotHint;
	bool							m_bShowRewardCard;						//是否显示喜金牌
	CPoint							m_cpRewardCard;
	bool							m_bShowSameTag[MAX_COUNT];							//显示喜金牌相同
	int								m_iTransBowlHint;						//聚宝盆提示透明度
	int								m_iTransLikeIngotHing;					//喜金提示
	

	//字体
protected:
	Sexy::SysFont					*m_pFont10WeiRuanYaHei;					//10号微软雅黑
	Sexy::SysFont					*m_pFont9WeiRuanYaHei;					//9号微软雅黑
	Sexy::SysFont					*m_pFont9HeiTi;							//9号黑体
};
