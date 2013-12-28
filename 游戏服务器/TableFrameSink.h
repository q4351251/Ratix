#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MD5Checksum.h"
#include <algorithm>
#include "GameLogic.h"
#include "log.h"
#include "Contral.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////


//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//操作变量
protected:
	LONGLONG						m_wbOperateTime;
	BYTE							m_bOxCard[GAME_PLAYER];					//牛牛数据
	bool							m_bCheat;								//是否可以配牌
	DWORD							m_dwJettonTime;							//开始时间
	DWORD							m_dwOverTime;							//开始时间
	//下注变量
protected:
	LONGLONG						m_lCellScore;							//单位积分
	LONGLONG						m_lMinbet;								//最小下注
	
	//基本变量配置
	static LONGLONG					m_lBuffer;								//累计池
	static BOOL						m_bMeetConditions;						//是否符合条件
	static LONGLONG					m_lBufferFund;							//用来存放除不尽的数

	LONGLONG						m_lBufferNumSection[4];					//累计元宝分段
	BYTE							m_cbBufferGrandRate[4];					//累计比率

	LONG							m_lLikeIngot;							//喜金元宝
	LONG							m_lNewLikeIngot[4];						//发放数目
	int								m_iSameCountLine;						//喜金发放限制						

	LONGLONG						m_lUserRevenue[GAME_PLAYER];			//税收比例
	LONG							m_lDeskPay;								//桌费
	BYTE							m_cbEateRate;							//吃分比例
	LONG							m_lInitBuffer;							//初始聚宝盆元宝
	BYTE							m_cbForceRate;							//强发概率
	BYTE							m_cbBufferRate;							//聚宝盆累计比例
	//玩家扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//玩家手牌
	BYTE							m_cbRewardCardData[GAME_PLAYER][MAX_COUNT]; //随机奖励牌

	BYTE							m_cbCheatCardData[GAME_PLAYER][MAX_COUNT];	//配牌后的牌
	BYTE							m_cbCheatRewardCard[GAME_PLAYER][MAX_COUNT];				//
	
	
	//玩家属性
	BYTE							m_cbPlayStatus[GAME_PLAYER];				//游戏状态
protected:
	CRITICAL_SECTION				m_csPlaceJetton;
	LONGLONG						m_lChipNum[MAX_CHIP_COUNT];				//筹码数量
	LONG							m_lPercentage[MAX_CHIP_COUNT];			//服务费比例
	LONGLONG						m_lUserAddScore[GAME_PLAYER];			//玩家下注
	LONGLONG						m_lSysWinScore;							//系统输赢分
	LONGLONG						m_lUserWinScore[GAME_PLAYER];			//玩家输赢分
	LONGLONG						m_lTotalWinScore[GAME_PLAYER];			//玩家总的输赢分（加上桌费）
	BOOL							m_bCompare[GAME_PLAYER];				//比牌是否完成
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	const tagGameServiceOption		* m_pGameServiceOption;					//配置参数
	ITableFrameControl				* m_pITableFrameControl;				//框架接口
	ITableFrameManager	*		    m_pITableFrameManager;
/*	CMD_GR_TblCfg					m_strTableCfg;							//桌子配置*/

    //
	//属性变量
protected:
	static const WORD				m_wPlayerCount;							//游戏人数
	static const enStartMode		m_GameStartMode;						//开始模式


	CLog							m_oLog;									//日志
	Contral							m_oContral;								//牌型控制
	IServerUserItem *				m_pIUserItem;							//机器人玩家

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID __cdecl Release() { }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void __cdecl RepositTableFrameSink();

	//信息接口
public:
	//开始模式
	virtual enStartMode __cdecl GetGameStartMode();
	//新加的
	virtual bool __cdecl GetGameBGode(WORD nChair){return true;}
	//是否有用户游戏
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

public:
	//游戏开始
	virtual bool __cdecl OnEventGameStart();
	//游戏结束
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件 
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);


	//动作事件
public:
	//用户断线
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//用户重入
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//用户坐下
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起来
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);
	
// 	//获取桌子配置信息
// 	virtual const CMD_GR_TblCfg* __cdecl GetTableCfg()
// 	{
// 		return &m_strTableCfg;
// 	}
	
	//游戏事件
protected:
	//兑换筹码
	bool OnExchangeChip(WORD wChairID, LONGLONG lChip);
	//催牌
	bool OnHurryCard(WORD wChairID , WORD cbWords);


	//操作时间
	void OperateTimer(WORD wChairID,WORD wTime);
	//辅助函数
protected:
	void CountDownTime(WORD wChairID , bool bKill=false);
	//读取配置文件
	void ReadIniCfg();
	//判断游戏是否可开始
	BYTE CheckStartGame();
	//写入积分
	void WriteScore();
	//游戏结束
	void ConcludeGame();
	//游戏准备开始
	void GameReady(WORD wChair);
	//发送聚宝盆信息
	void SendBowl(WORD wChairID);
	//游戏开始
	void GameStart();
	//随机（普通）发牌
	bool DispathCard();
	//摊牌事件
	bool OnUserOpenCard(WORD wChairID, BYTE bOx);
	//比牌完成
	bool OnCompareCardFinish(WORD wChairID);
	//检查是否获得喜金
	void CheckXijin();
	


	
	//人工智能函数
protected:
	//检查玩家贡献度
	bool CheckUserContribution(WORD wChairID , LONGLONG lBuffer);
	//更新聚宝盆
	bool UpdateTask(BYTE cbBufferType=0);
	//记录聚宝盆牌型
	void logCard(WORD wChairID);
	//发送聚宝盆
	void SendTaskIngot();
	//屏蔽昵称
	void HideNickName(const TCHAR *szNickName , TCHAR *szOutNickName );
	//放入聚宝盆
	bool AddBuffer(BYTE cbBufferType=0 , LONGLONG lChip=0);
	//检查是否满足强发
	bool CheckBufferForce(BYTE cbBufferType=0);
	//强发
	bool ForceDispathCard();
public:
	//发送消息
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

};

//////////////////////////////////////////////////////////////////////////

#endif