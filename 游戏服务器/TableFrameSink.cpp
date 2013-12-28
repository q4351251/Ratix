#include "StdAfx.h"
#include "strhelper.h"
#include "TableFrameSink.h"



////静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式
LONGLONG			CTableFrameSink::m_lBuffer=0;
LONGLONG			CTableFrameSink::m_lBufferFund=0;	
BOOL				CTableFrameSink::m_bMeetConditions=0;
//////////////////////////////////////////////////////////////////////////
//定时器标识

#define IDI_TIME_READCFG				1									//读取配置
#define IDI_CLEAR_BUFFER_STATE			2									//5分钟后清除对应强发的状态
#define IDI_READY_START_GAME			5									//游戏准备开始
#define IDI_COUNTDOWN					6									//游戏准备开始
#define IDI_WRITESCORE					15									//写分
#define IDI_CONCLUDEGAME				16									//游戏解散
#define IDI_REMNANT_DESTORY				17									//一秒调一次
#define IDI_OPERATE_CARD				18									//牌型操作

#define TIME_READY						250									//准备
#define TIME_LESS						15									//一般
#define TIME_OPERATE_TIME				1000								//一秒调一次
#define TIME_OFFLINE					5								
#define TIME_READY_START_GAME			5									//5秒后开始
#define	TIME_OPERATE_CARD				20000								//牌型操作时间20秒
#define TIME_READCONFIG					30000								//读取配置文件的时间
	
//////////////////////////////////////////////////////////////////////////
//构造函数
CTableFrameSink::CTableFrameSink()
{
	InitializeCriticalSection(&m_csPlaceJetton);

	//设置工作目录
	TCHAR szWorkDir[MAX_PATH];
	ZeroMemory(szWorkDir,sizeof(szWorkDir));
	GetModuleFileName(AfxGetInstanceHandle(),szWorkDir,sizeof(szWorkDir));
	int nModuleLen=lstrlen(szWorkDir);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".dll"));
	szWorkDir[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(szWorkDir);
	
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_bOxCard[i]=0xff;
	}
	m_lSysWinScore = 0;

/*	ZeroMemory(&m_strTableCfg, sizeof(m_strTableCfg));*/

	//玩家属性
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lTotalWinScore, sizeof(m_lTotalWinScore));
	ZeroMemory(m_bCompare , sizeof(m_bCompare));
	ZeroMemory(m_cbCheatCardData, sizeof(m_cbCheatCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbRewardCardData, sizeof(m_cbRewardCardData));
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pIUserItem = NULL;
	m_lCellScore = 0L;
	m_lMinbet = 0L;
	m_iSameCountLine =6;
	m_bCheat = false;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
	DeleteCriticalSection(&m_csPlaceJetton);
}

//接口查询
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化 
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//控制接口 
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	m_pITableFrameManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameManager);
	if (m_pITableFrameControl==NULL) return false;
	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	

	char szLogFileName[1024] = {0};
	SafeSprintf(szLogFileName,"%02d-桌子",(m_pITableFrame->GetTableID()+1));
	char szLogDir[128]={0};
	SafeSprintf(szLogDir,"%d[%s]",m_pGameServiceOption->wServerID,m_pGameServiceOption->szGameRoomName);
	m_lCellScore = m_pGameServiceOption->lCellScore;
	m_oLog.Init("Ratiox",szLogDir,szLogFileName);
	m_oLog.Log("==日志启动完毕==");

	ReadIniCfg();
	m_pITableFrame->SetGameTimer(IDI_TIME_READCFG , 5*60*1000 , -1 , 0);
	m_oContral.Init(m_pITableFrame , &m_oLog , m_pGameServiceOption->lCellScore , m_pGameServiceOption->wRevenue);
	srand((unsigned)time(NULL));
	UpdateTask();
	return true;
}

//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{    
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_bOxCard[i]=0xff;
	}
	m_lSysWinScore = 0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lTotalWinScore, sizeof(m_lTotalWinScore));
	ZeroMemory(m_bCompare , sizeof(m_bCompare));
	ZeroMemory(m_cbCheatCardData, sizeof(m_cbCheatCardData));
	ZeroMemory(m_cbRewardCardData, sizeof(m_cbRewardCardData));
	m_bCheat = false;
}

//开始模式
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_dwJettonTime = GetTickCount();
	m_pITableFrame->SetGameStatus(GS_OX_PLAY);
	CountDownTime(INVALID_CHAIR , TRUE);
 	m_oContral.Reset();
// 	m_oContral.GetNextGamePrizeMode();

	bool bAllAndroid=true;

	//用户状态
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);

		if(pIServerUser==NULL)
		{
			m_cbPlayStatus[i]=FALSE;
		}
		else
		{
			m_cbPlayStatus[i]=TRUE;
			if(!pIServerUser->IsAndroidUser())
			{
				bAllAndroid=false;
			}
		}
	}


	SendBowl(INVALID_CHAIR);
	GameStart();
	return true;
}

//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason) 
{
    switch (cbReason)
    {
    case GER_NORMAL:		//常规结束
        {
			m_pITableFrame->SetGameStatus(GS_OX_ACCOUNTS);
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			CountDownTime(INVALID_CHAIR, true);
			//获得获胜玩家
			WORD wWinUser =  m_GameLogic.GetWinScore(m_cbPlayStatus , m_cbHandCardData , m_lMinbet , m_lUserWinScore , m_pGameServiceOption->wRevenue , m_lUserRevenue);
			
			m_lSysWinScore = m_oContral.GetSysWinScore(m_lUserWinScore, wWinUser);
			for(WORD i=0; i<GAME_PLAYER ; i++)
			{
				IServerUserItem * pIServerUserItem1 = m_pITableFrame->GetServerUserItem(i);
				if(pIServerUserItem1)
				{
					m_oLog.Log("玩家[%s]牌型赢分[%I64d]", pIServerUserItem1->GetAccounts(), m_lUserWinScore[i]);
				}
				GameEnd.lGameScore[i] = m_lUserWinScore[i];
			}
			GameEnd.lDeskPay = m_lDeskPay;
			CopyMemory(GameEnd.cbCardData, m_cbHandCardData, sizeof(m_cbHandCardData));
			//发送信息
 			for (WORD i=0;i<m_wPlayerCount;i++)
 			{
 				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
 			}
 			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			SendTaskIngot();
 			m_pITableFrame->SetGameTimer(IDI_WRITESCORE , 10*1000 , 1 , 0);
			return true;
        }        
    case GER_USER_LEFT:		//用户离开
        {
            return true;
        }        
    }
    return false;
}

//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStation, bool bSendSecret)
{
	switch (cbGameStation)
	{
	case GS_OX_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			StatusFree.lCellScore = m_pGameServiceOption->lCellScore;
			StatusFree.wServerType = m_pGameServiceOption->wServerType;
			StatusFree.wServerID = m_pGameServiceOption->wServerID;
			StatusFree.lMinBet = m_lMinbet;
			CopyMemory(StatusFree.lChipNum,m_lChipNum, sizeof(m_lChipNum));
			CopyMemory(StatusFree.lPercentage, m_lPercentage, sizeof(m_lPercentage));
			SendBowl(wChairID);
			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_OX_PLAY:	//游戏状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));
			//变量定义
			StatusPlay.lCellScore = m_pGameServiceOption->lCellScore;
			StatusPlay.wServerType = m_pGameServiceOption->wServerType;
			StatusPlay.wServerID = m_pGameServiceOption->wServerID;
			StatusPlay.lMinBet = m_lMinbet;
			CopyMemory(StatusPlay.lChipNum,m_lChipNum, sizeof(m_lChipNum));
			CopyMemory(StatusPlay.lPercentage, m_lPercentage, sizeof(m_lPercentage));

			CopyMemory(StatusPlay.cbOx, m_bOxCard, sizeof(m_bOxCard));
			CopyMemory(StatusPlay.cbCardData[wChairID], m_cbHandCardData[wChairID], MAX_COUNT);
			CopyMemory(StatusPlay.cbPlayStatus, m_cbPlayStatus, sizeof(m_cbPlayStatus));
			CopyMemory(StatusPlay.cbRewardCardData, m_cbRewardCardData, sizeof(m_cbRewardCardData));
			//发送场景
			SendBowl(wChairID);
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_OX_ACCOUNTS:	//海底状态
		{
			//变量定义
			CMD_S_StatusEnd StatusEnd;
			memset(&StatusEnd,0,sizeof(StatusEnd));
			StatusEnd.lCellScore = m_pGameServiceOption->lCellScore;
			StatusEnd.wServerType = m_pGameServiceOption->wServerType;
			StatusEnd.wServerID = m_pGameServiceOption->wServerID;
			StatusEnd.lMinBet = m_lMinbet;
			StatusEnd.lDeskPay = m_lDeskPay;
			CopyMemory(StatusEnd.lChipNum,m_lChipNum, sizeof(m_lChipNum));
			CopyMemory(StatusEnd.lPercentage, m_lPercentage, sizeof(m_lPercentage));
			//CopyMemory(StatusEnd.lNewLikeIngot,m_lNewLikeIngot, sizeof(m_lNewLikeIngot));
			CopyMemory(StatusEnd.cbCardData, m_cbHandCardData, sizeof(m_cbHandCardData));
			CopyMemory(StatusEnd.lGameScore, m_lUserWinScore, sizeof(m_lUserWinScore));
			CopyMemory(StatusEnd.cbPlayStatus, m_cbPlayStatus, sizeof(m_cbPlayStatus));
			CopyMemory(StatusEnd.cbRewardCardData, m_cbRewardCardData, sizeof(m_cbRewardCardData));

			SendBowl(wChairID);
			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusEnd,sizeof(StatusEnd));
		}
	}

	return false;
}


//定时器事件 
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	switch(wTimerID)
	{
	case IDI_CLEAR_BUFFER_STATE:
		{
			m_bMeetConditions=false;
			return true;
		}
	case IDI_TIME_READCFG:
		{
			ReadIniCfg();
			return true;
		}
	case IDI_READY_START_GAME:
		{
			BYTE cbTime = CheckStartGame();
			if(cbTime != INVALID_BYTE)
			{
				m_pITableFrameControl->StartGame();
			}			
			return true;
		}
	case IDI_WRITESCORE:
		{
			if (m_pITableFrame->GetGameStatus() != GS_OX_ACCOUNTS) return true;
			WriteScore();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FINISH,NULL, 0);
			return true;
		}
	case IDI_CONCLUDEGAME:
		{
			ConcludeGame();
			return true;
		}
	}
	if(wTimerID>=IDI_COUNTDOWN && (wTimerID<(IDI_COUNTDOWN+GAME_PLAYER)))
	{
		WORD wChairID = wTimerID-IDI_COUNTDOWN;
		IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
		if(pServerUserItem == NULL) return true;
		switch(m_pITableFrame->GetGameStatus())
		{
		case GS_FREE:
			{
				m_pITableFrameManager->PerformStandUpAction(pServerUserItem);
				break;
			}
		case GS_OX_PLAY:
			{
				OnUserOpenCard(wChairID , 0);
				break;
			}

		}
		return true;
	}
	return false;
}

//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
// 	if (pIServerUserItem == NULL)
// 	{
// 		return false;
// 	}
	switch (wSubCmdID)
	{
	case SUB_C_HURRY_CARD:			//催牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_HurryCard));
			if (wDataSize!=sizeof(CMD_C_HurryCard)) return false;

			//变量定义
			CMD_C_HurryCard * pHurryCard=(CMD_C_HurryCard *)pDataBuffer;
			//状态判断
			ASSERT(m_cbPlayStatus[pIServerUserItem->GetChairID()]);
			if(!m_cbPlayStatus[pIServerUserItem->GetChairID()])return true;

			return OnHurryCard(pIServerUserItem->GetChairID() , pHurryCard->cbWords);
		}
	case SUB_C_CHEAT:				//测试配牌
		{
			ASSERT(wDataSize==sizeof(CMD_C_Cheat));
			if (wDataSize!=sizeof(CMD_C_Cheat)) return false;

			CMD_C_Cheat *pCheat = (CMD_C_Cheat *)pDataBuffer;
			if (pCheat->iCheatControl)
			{	
				m_bCheat = true;
				for(int i=0; i<GAME_PLAYER;i++)
				{
					CopyMemory(m_cbCheatCardData, pCheat->cbHandCardData, sizeof(m_cbCheatCardData));
					CopyMemory(m_cbCheatRewardCard, pCheat->cbRewardCard, sizeof(m_cbRewardCardData));
				}
				Contral::m_lScorePool = pCheat->lPoolScore;
			}
			return true;
		}
	case SUB_C_EX_CHIP:				//兑换筹码
		{
			ASSERT(wDataSize==sizeof(CMD_C_ExchangeChip));
			if (wDataSize!=sizeof(CMD_C_ExchangeChip)) return false;

			CMD_C_ExchangeChip *pExchangeChip = (CMD_C_ExchangeChip *)pDataBuffer;
			return OnExchangeChip(pIServerUserItem->GetChairID(), pExchangeChip->lExchangeCount);
		}
	case SUB_C_OPEN_CARD:			//摊牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//变量定义
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//用户效验
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//状态判断
			ASSERT(m_cbPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_cbPlayStatus[pUserData->wChairID]==FALSE)return false;

			//消息处理
			return OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
		}
	case SUB_C_COMPARE_CARD:		//摊牌后比牌
		{
			return OnCompareCardFinish(pIServerUserItem->GetChairID());
		}

	}
	return false;
}

//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户断线
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) 
{

	CMD_S_OffLine_Reconn OffLine;
	OffLine.wOperaUser = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OFFLINE,&OffLine,sizeof(CMD_S_OffLine_Reconn));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OFFLINE,&OffLine,sizeof(CMD_S_OffLine_Reconn));


	if (m_bOxCard[wChairID] !=1)
	{
		OnUserOpenCard(wChairID, 1);
	}
	return true;
}
//用户坐下
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{    
	CountDownTime(wChairID);
	OnActionUserReady(INVALID_CHAIR , NULL , NULL , INVALID_WORD);
	return true;
}

//用户起来
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{        
	CountDownTime(wChairID , TRUE);
	/*OnActionUserReady(INVALID_CHAIR , NULL , NULL , INVALID_WORD);*/
	//m_oLog.Log("玩家[%s]离开桌子。", pIServerUserItem->GetUserData()->szAccounts);
 	m_pITableFrame->KillGameTimer(IDI_READY_START_GAME);
 	if (CheckStartGame() != INVALID_BYTE)
 	{
		//m_oLog.Log("游戏能开始");
 		m_pITableFrameControl->StartGame();
 	}
	else
	{
		//m_oLog.Log("游戏不能开始");
		m_pITableFrame->KillGameTimer(IDI_READY_START_GAME);
		//m_bStartCountDown = false;
		m_pITableFrame->SetGameStatus(GS_FREE);
		CMD_S_GameFree GameFree;
		ZeroMemory(&GameFree , sizeof(GameFree));
		GameFree.cbTimeLeave=-1;
		for(INT i=0 ; i<GAME_PLAYER ; i++)
		{
			if(i!=wChairID &&m_pITableFrame->GetServerUserItem(i) && !m_pITableFrame->GetServerUserItem(i)->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(i, SUB_S_GAME_FREE, &GameFree, sizeof(GameFree));
			}
		}
	}
	return true;
}

//发送消息 
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	
	if (wChairID==INVALID_CHAIR)
	{
		//游戏玩家
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO);
		}

		//旁观玩家
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_INFO);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_INFO|SMT_EJECT);
	}
}

bool __cdecl CTableFrameSink::OnActionUserReady( WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize ) 
{
	//m_oLog.Log("玩家[%s]准备。", pIServerUserItem->GetUserData()->szAccounts);
	if (wChairID != INVALID_CHAIR)
	{
		if(pIServerUserItem->GetUserScore()->lBodyChip < m_lMinbet*MAXMULITY)
		{
			m_pITableFrameManager->PerformStandUpAction(pIServerUserItem);
		}
		else
		{
			CountDownTime(wChairID,TRUE);
			GameReady(wChairID);
		}
		
	}
	else
	{
		GameReady(wChairID);
	}
	
    return true;
}

void CTableFrameSink::OperateTimer( WORD wChairID,WORD wTime )
{
	if(US_OFFLINE != m_pITableFrame->GetServerUserItem(wChairID)->GetUserStatus())
	{
		m_pITableFrame->KillGameTimer(IDI_REMNANT_DESTORY);
		m_wbOperateTime = wTime;
		m_pITableFrame->SetGameTimer(IDI_REMNANT_DESTORY, TIME_OPERATE_TIME, wTime, wChairID);

		m_pITableFrame->KillGameTimer(IDI_OPERATE_CARD);
		m_pITableFrame->SetGameTimer(IDI_OPERATE_CARD, TIME_OPERATE_CARD, 1, wChairID);
	}
}

void CTableFrameSink::CountDownTime( WORD wChairID , bool bKill/*=false*/ )
{
	if(bKill)
	{
		if(wChairID == INVALID_CHAIR)
		{
			for (int i=0; i<GAME_PLAYER ;i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pServerUserItem == NULL) continue;
				m_pITableFrame->KillGameTimer(IDI_COUNTDOWN+i);
			}
		}
		else
		{
			m_pITableFrame->KillGameTimer(IDI_COUNTDOWN+wChairID);
		}
	}
	else
	{
		BYTE cbLeaveTime = TIME_LESS;
		if(m_pITableFrame->GetGameStatus() == GS_FREE)
		{
			cbLeaveTime = TIME_LESS+10;
		}
		else if(m_pITableFrame->GetGameStatus() == GS_OX_PLAY)
		{
			cbLeaveTime = TIME_LESS*2;
		}

		if(wChairID != INVALID_CHAIR)
		{
			IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
			ASSERT(pServerUserItem != NULL);
			if(pServerUserItem!=NULL && pServerUserItem->GetUserStatus() == US_OFFLINE)
			{
				cbLeaveTime = TIME_OFFLINE;
			}
			m_pITableFrame->SetGameTimer(IDI_COUNTDOWN+wChairID , cbLeaveTime*1000 , 1 , 0);
		}
		else
		{
			for(int i=0 ; i<GAME_PLAYER; i++)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(i);
				if(pServerUserItem == NULL) continue;
				if(pServerUserItem->GetUserStatus() == US_OFFLINE)
				{
					cbLeaveTime = TIME_OFFLINE;
				}
				m_pITableFrame->SetGameTimer(IDI_COUNTDOWN+i , cbLeaveTime*1000 , 1 , 0);
			}
		}

	}
	m_pITableFrame->KillGameTimer(IDI_CONCLUDEGAME);
	m_pITableFrame->SetGameTimer(IDI_CONCLUDEGAME,60*1000,1,0);
}

void CTableFrameSink::ReadIniCfg()
{
	//设置文件名
	m_oLog.Log("================================读取配置文件成功=======================================");
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf_s(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\%d_RatiOx.ini"),szPath,m_pGameServiceOption->wServerID);


	m_lMinbet = GetPrivateProfileInt(TEXT("BASE"), TEXT("MINBET"), 100, szConfigFileName);
	m_lDeskPay= GetPrivateProfileInt(TEXT("BASE"), TEXT("DeskPay"), 1, szConfigFileName);
	m_cbEateRate = GetPrivateProfileInt("SCOREPOOL","SysEateRate",20,szConfigFileName);
	m_iSameCountLine = GetPrivateProfileInt("BUFFER","NewLikeIngotCount",3,szConfigFileName);

	
	TCHAR szBuffer[MAX_PATH];
	std::string strAnimalSqu;
	std::vector<std::string> vtStrAnimalPrizeSeq;
	//（新）喜金
	GetPrivateProfileString(TEXT("BUFFER"),TEXT("NewLikeIngot"),TEXT("5,10,20,30"),szBuffer,CountArray(szBuffer),szConfigFileName);
	strAnimalSqu = szBuffer;
	vtStrAnimalPrizeSeq.clear();

	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
	{
		m_lNewLikeIngot[i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
	}


	GetPrivateProfileString(TEXT("BUFFER"),TEXT("BigBufferGrandSection"),TEXT("500,1000,5000,10000"),szBuffer,CountArray(szBuffer),szConfigFileName);
	strAnimalSqu = szBuffer;
	vtStrAnimalPrizeSeq.clear();

	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
	{
		m_lBufferNumSection[i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
	}

	GetPrivateProfileString(TEXT("BUFFER"),TEXT("BigBufferGrandRate"),TEXT("30,20,10,1"),szBuffer,CountArray(szBuffer),szConfigFileName);
	strAnimalSqu = szBuffer;
	vtStrAnimalPrizeSeq.clear();

	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
	{
		m_cbBufferGrandRate[i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
	}

// 	GetPrivateProfileString(TEXT("BUFFER"),TEXT("SmallBufferGrandSection"),TEXT("500,1000,2000,10000"),szBuffer,CountArray(szBuffer),szConfigFileName);
// 	strAnimalSqu = szBuffer;
// 	vtStrAnimalPrizeSeq.clear();

// 	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
// 	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
// 	{
// 		m_lBufferNumSection[SMALL_BUFFER][i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
// 	}
// 
// 	GetPrivateProfileString(TEXT("BUFFER"),TEXT("SmallBufferGrandRate"),TEXT("30,20,10,1"),szBuffer,CountArray(szBuffer),szConfigFileName);
// 	strAnimalSqu = szBuffer;
// 	vtStrAnimalPrizeSeq.clear();

// 	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
// 	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
// 	{
// 		m_cbBufferGrandRate[SMALL_BUFFER][i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
// 	}
	m_cbBufferRate=GetPrivateProfileInt("BUFFER","BufferRate",10,szConfigFileName);
	m_cbForceRate = GetPrivateProfileInt(TEXT("BUFFER"), TEXT("ForceRate"), 100, szConfigFileName);
	m_lInitBuffer =  GetPrivateProfileInt(TEXT("BUFFER"), TEXT("InitBigBuffer"), 300, szConfigFileName);
	/*m_lInitBuffer[SMALL_BUFFER] =  GetPrivateProfileInt(TEXT("BUFFER"), TEXT("InitSmallBuffer"), 100, szConfigFileName);*/
	m_lLikeIngot = GetPrivateProfileInt(TEXT("BUFFER"), TEXT("LikeIngot"), 2, szConfigFileName);


	m_oContral.LoadCfg(szConfigFileName, m_cbEateRate);
	for (int i=0; i<MAX_CHIP_COUNT; i++)
	{
		char szBuf[100] = {0};
		sprintf_s(szBuf, 100, TEXT("Chip%d"), i+1);
		m_lChipNum[i] = GetPrivateProfileInt(TEXT("EXCHANGE_CHIP"), szBuf, 100, szConfigFileName);
		sprintf_s(szBuf, 100, TEXT("Percentage%d"), i+1);
		m_lPercentage[i] = GetPrivateProfileInt(TEXT("EXCHANGE_CHIP"), szBuf, 10, szConfigFileName);
	}
}

BYTE CTableFrameSink::CheckStartGame()
{
	//准备人数
	WORD wCount=0;
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			wCount++;
			pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_READY) return INVALID_BYTE;
		}
	}
	if(wCount < 2) return INVALID_BYTE;
	//m_oLog.Log("准备人数[%d]。",wCount);
	if(wCount == GAME_PLAYER)
	{
		return 0;
	}
	return TIME_READY_START_GAME;
}

void CTableFrameSink::WriteScore()
{
	m_dwOverTime = GetTickCount();
	DWORD temp_dwPlayTime = (m_dwOverTime - m_dwJettonTime)/1000;
	m_pITableFrame->KillGameTimer(IDI_WRITESCORE);
	//SendTaskIngot();

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(m_cbPlayStatus[i] == FALSE) continue;
		m_lTotalWinScore[i] = m_lUserWinScore[i] - m_lDeskPay;
	}
	
	//聚宝盆
	//SendTaskIngot();
	LONGLONG lTotalRevenue = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_lTotalWinScore[i] != 0)
		{
			enScoreKind nScoreKind = enScoreKind_Draw;
			nScoreKind = (m_lUserWinScore[i] > 0 )?enScoreKind_Win:enScoreKind_Lost;
			IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
			if(pIServerUserItem)
			{
				m_oLog.Log("玩家[%s] 写入积分[%I64d]" , pIServerUserItem->GetUserData()->szAccounts , m_lTotalWinScore[i]);
				if(!pIServerUserItem->IsAndroidUser())
				{
					lTotalRevenue += m_lUserRevenue[i];

					LONGLONG temp_SysEat=0;
					temp_SysEat += m_lDeskPay;
					if (m_lUserWinScore[i] < 0)
					{
						temp_SysEat += ((-m_lUserWinScore[i])*m_cbEateRate /100);
					}

					//写入积分
					m_pITableFrame->WriteUserScore(i,m_lTotalWinScore[i],m_lUserRevenue[i],nScoreKind,temp_dwPlayTime,temp_SysEat,m_lMinbet);

				}
				else
				{
					//写入积分
					m_pITableFrame->WriteUserScore(i,m_lTotalWinScore[i],m_lUserRevenue[i],nScoreKind);
				}


			}
		}						 
	}

	for(int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem == NULL) continue;
		if (!pIServerUserItem->IsAndroidUser())
		{
			m_oLog.Log("玩家[%s] 身上剩余[%I64d]" , pIServerUserItem->GetAccounts() , pIServerUserItem->GetUserScore()->lBodyChip);
		}
	}
	m_oLog.Log("将要写入放分池的系统输赢分[%I64d]" , m_lSysWinScore);
	//系统输赢放入放分池
	m_oContral.ChangeBuffer(m_lSysWinScore);
	//放入聚宝盆
// 	if (m_lSysWinScore > 0)
// 	{
// 		AddBuffer(0,m_lSysWinScore*m_cbBufferRate/100);
// 	}

	
	if(lTotalRevenue > 0)
	{
		// 		AddBuffer(BIG_BUFFER , lTotalRevenue);
		// 		AddBuffer(SMALL_BUFFER , lTotalRevenue);
	}

	//设置文件名
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_snprintf_s(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\%d_RatiOx.ini"),szPath,m_pGameServiceOption->wServerID);
	m_oLog.Log("=============放分池数据[%I64d]写入配置文件[%s]=================" ,m_oContral.GetPoolScore(),szConfigFileName);
	CString strPoolScore;
	strPoolScore.Format("%I64d", m_oContral.GetPoolScore());
	WritePrivateProfileString(TEXT("SCOREPOOL"), TEXT("ScorePool"),strPoolScore,szConfigFileName);
	ConcludeGame();
	m_oLog.Log("////------------游戏结束----------------------////");
	//结束游戏
}

void CTableFrameSink::ConcludeGame()
{
	m_pITableFrame->ConcludeGame();
	m_pITableFrame->SetGameStatus(GS_FREE);
	/*m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_FREE);*/
	CountDownTime(INVALID_CHAIR);
}
//游戏准备开始
void CTableFrameSink::GameReady( WORD wChair )
{
	m_pITableFrame->KillGameTimer(IDI_READY_START_GAME);
	BYTE cbLevelTime = CheckStartGame();
	if(cbLevelTime != INVALID_BYTE)
	{
		if(cbLevelTime == 0)
		{
			m_pITableFrameControl->StartGame();
		}
		else
		{
			CMD_S_GameReady GameReady;
			ZeroMemory(&GameReady , sizeof(GameReady));
			GameReady.cbTimeLeave=cbLevelTime;
			for(INT i=0; i<GAME_PLAYER; i++)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pIServerUserItem!=NULL && !pIServerUserItem->IsAndroidUser())
				{
					m_pITableFrame->SendTableData(i, SUB_S_GAME_READY, &GameReady, sizeof(GameReady));
				}
			}
			m_pITableFrame->SetGameTimer(IDI_READY_START_GAME , cbLevelTime*1000 , 1 , 0);
		}
	}
	else
	{
		m_pITableFrame->KillGameTimer(IDI_READY_START_GAME);
		//m_bStartCountDown = false;
		m_pITableFrame->SetGameStatus(GS_FREE);
		CMD_S_GameFree GameFree;
		ZeroMemory(&GameFree , sizeof(GameFree));
		GameFree.cbTimeLeave=-1;
		for(INT i=0 ; i<GAME_PLAYER ; i++)
		{
			if(i!= wChair&&m_pITableFrame->GetServerUserItem(i) && !m_pITableFrame->GetServerUserItem(i)->IsAndroidUser())
			{
				m_pITableFrame->SendTableData(i, SUB_S_GAME_FREE, &GameFree, sizeof(GameFree));
			}
		}
	}
}

bool CTableFrameSink::OnExchangeChip( WORD wChairID, LONGLONG lChip )
{
	IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	LONGLONG lScore;

	if (GAME_GENRE_GOLD == m_pGameServiceOption->wServerType)//欢乐豆筹码
	{
		lScore = pIServerUserItem->GetUserScore()->lBodySilverScore - 10000;
	}
	else if (GAME_GENRE_SCORE_CHIP == m_pGameServiceOption->wServerType)//灵珠筹码
	{
		lScore = pIServerUserItem->GetUserScore()->lBodyScore;
	}
	else
	{
		m_pITableFrame->SendGameMessage(pIServerUserItem, "房间类型错误，请联系在线客服解决此问题！", SMT_CLOSE_GAME|SMT_EJECT|SMT_INFO);
		return false;
	}

	for (int i=MAX_CHIP_COUNT-1; i>=0; i--)
	{
		if (lChip >= m_lChipNum[i])
		{
			if (lScore < lChip*m_pGameServiceOption->lCellScore + lChip*m_pGameServiceOption->lCellScore*m_lPercentage[i]/100)
			{
				char szBuf[100];
				sprintf_s(szBuf, 100 , "您所带的游戏币不足以兑换%I64d筹码！", lChip);
				m_pITableFrame->SendGameMessage(pIServerUserItem, szBuf, SMT_CLOSE_GAME|SMT_EJECT|SMT_INFO);
				return false;
			}
			else
			{
				m_pITableFrame->SendExchangeChip(pIServerUserItem, lChip+lChip*m_lPercentage[i]/100);
				m_pITableFrame->WriteUserScore(pIServerUserItem, -(lChip*m_lPercentage[i]/100), 0, enScoreKind_Service);
				m_pITableFrame->SendTableData(wChairID,SUB_S_EX_CHIP);
				return true;
			}
		}
	}
	return true;
}
//发送聚宝盆信息
void CTableFrameSink::SendBowl( WORD wChairID )
{
	CMD_S_Bowl bowl;
	ZeroMemory(&bowl ,sizeof(bowl));
	bowl.lBowlIngot = m_lBuffer;
	bowl.lLikeGoldIngot = m_lLikeIngot;
	CopyMemory(bowl.lNewLikeIngot, m_lNewLikeIngot, sizeof(m_lNewLikeIngot));
	m_pITableFrame->SendTableData(wChairID,SUB_S_BOWL,&bowl,sizeof(bowl));
}


//游戏开始
void CTableFrameSink::GameStart()
{
	m_oLog.Log("//////-------游戏开始--------////////");
	for(int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem == NULL) continue;
		if (!pIServerUserItem->IsAndroidUser())
		{
			m_oLog.Log("玩家[%s] 带入筹码[%I64d]" , pIServerUserItem->GetAccounts() , pIServerUserItem->GetUserScore()->lBodyChip);
		}
	}
	CountDownTime(INVALID_CHAIR);
 	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	//强发聚宝盆
	//m_oLog.Log("强发状态：%s",m_bMeetConditions?"已经进行过强发，还未清除状态不能强发！":"未进行过强发，可以强发！");
	bool bSuccess = false;//= ForceDispathCard();
	//m_oLog.Log("1");
	while (!bSuccess)
	{
		//m_oLog.Log("2");
		bSuccess = DispathCard();
	}
	
	//随机喜金牌
	ZeroMemory(m_cbRewardCardData,sizeof(m_cbRewardCardData));
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] == FALSE) continue;
		m_GameLogic.RandCardList(m_cbRewardCardData[i],MAX_COUNT);
	}
	
	//测试配牌
 	if (m_bCheat)
 	{
 		for (int i=0; i<GAME_PLAYER; i++)
 		{
 			if (m_cbCheatCardData[i][0] == 0)continue;
 			CopyMemory(m_cbHandCardData[i], m_cbCheatCardData[i],MAX_COUNT);
 	
 		}
 		for (int i=0; i<GAME_PLAYER; i++)
 		{
 			if (m_cbCheatRewardCard[i][0] ==0)continue;
 			{
 				CopyMemory(m_cbRewardCardData[i], m_cbCheatRewardCard[i], MAX_COUNT);
 			}
 		}
 	}

	m_oLog.Log("原始手牌：");

	for(int i=0; i<GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);
		if(pIServerUser==NULL) continue;
		m_oLog.Log("%s手牌:[%s%s][%s%s][%s%s][%s%s][%s%s]---牌型为[%s]",pIServerUser->GetUserData()->szAccounts,cn(m_cbHandCardData[i][0]),vn(m_cbHandCardData[i][0]),
			cn(m_cbHandCardData[i][1]),vn(m_cbHandCardData[i][1]),cn(m_cbHandCardData[i][2]),vn(m_cbHandCardData[i][2]),cn(m_cbHandCardData[i][3]),vn(m_cbHandCardData[i][3]),
			cn(m_cbHandCardData[i][4]),vn(m_cbHandCardData[i][4]),CardTypeName(m_GameLogic.GetBestCardType(m_cbHandCardData[i],MAX_COUNT)));
	}
	
	m_oContral.ControlDispath(m_cbHandCardData, m_cbPlayStatus);

	m_oLog.Log("控制后手牌：");

	for(int i=0; i<GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetServerUserItem(i);
		if(pIServerUser==NULL) continue;
		m_oLog.Log("%s手牌:[%s%s][%s%s][%s%s][%s%s][%s%s]---牌型为[%s]",pIServerUser->GetUserData()->szAccounts,cn(m_cbHandCardData[i][0]),vn(m_cbHandCardData[i][0]),
			cn(m_cbHandCardData[i][1]),vn(m_cbHandCardData[i][1]),cn(m_cbHandCardData[i][2]),vn(m_cbHandCardData[i][2]),cn(m_cbHandCardData[i][3]),vn(m_cbHandCardData[i][3]),
			cn(m_cbHandCardData[i][4]),vn(m_cbHandCardData[i][4]),CardTypeName(m_GameLogic.GetBestCardType(m_cbHandCardData[i],MAX_COUNT)));
	}
// 	
	//发送扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;
		
		CopyMemory(GameStart.cbPlayStatus, m_cbPlayStatus,sizeof(m_cbPlayStatus));
		//派发扑克
		CopyMemory(GameStart.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
		std::random_shuffle(GameStart.cbCardData[i], GameStart.cbCardData[i]+MAX_COUNT);
		//派发喜金扑克
		CopyMemory(GameStart.cbRewardCardData[i], m_cbRewardCardData[i],MAX_COUNT);
		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		ZeroMemory(GameStart.cbCardData, sizeof(GameStart.cbCardData));
	}


	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));


}
//随机（普通）发牌
bool CTableFrameSink::DispathCard()
{
	BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
	m_oContral.DispathCard(bTempArray , m_lUserAddScore ,m_cbPlayStatus,  0);
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;

		//派发扑克
		CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}
	//聚宝盆发放检查
// 	for (WORD i=0 ; i<GAME_PLAYER; i++)
// 	{
// 		if(m_pITableFrame->GetServerUserItem(i) == NULL) continue;
// 		if(m_cbPlayStatus[i] == FALSE) continue;
// 		//logCard(i);
// 		for (int j=0; j<GAME_PLAYER; j++)
// 		{
// 			ASSERT(m_cbHandCardData[i][j] !=0);
// 			if (m_cbHandCardData[i][j] == 0) 
// 			{
// 				m_oLog.Log("[BUG]玩家手牌为空！！！");
// 				return false;
// 			}
// 		}
// 		if(m_GameLogic.GetCardType(m_cbHandCardData[i] , MAX_COUNT) == OX_DOUBLE_JOKE)
// 		{
// 			if(m_bMeetConditions) return false;
// 			if(!CheckUserContribution(i , m_lBuffer)) return false;
// 			m_bMeetConditions=true;
// 			m_pITableFrame->SetGameTimer(IDI_CLEAR_BUFFER_STATE, 5*60*1000, 1, 0);
// 		}
// 	}

	return true;
}
//摊牌事件
bool CTableFrameSink::OnUserOpenCard( WORD wChairID, BYTE bOx )
{
	//状态效验
	ASSERT (m_pITableFrame->GetGameStatus()==GS_OX_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_OX_PLAY) return false;
	CountDownTime(wChairID , TRUE);
	IServerUserItem* pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	//m_oLog.Log("玩家[%s]摊牌。", pServerUserItem->GetUserData()->szAccounts);

	//效验数据
	ASSERT(bOx==FALSE || bOx==TRUE);
	if(bOx!=FALSE && bOx!=TRUE)return false;

	//效验数据
	if(bOx)
	{
		ASSERT(m_GameLogic.GetBestCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=0);
		if(!(m_GameLogic.GetBestCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=0))return false;
	}

	//牛牛数据
	m_bOxCard[wChairID] = bOx;

	//摊牌人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE)bUserCount++;
	}

	//构造变量
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//设置变量
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

	//发送数据
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	//结束游戏
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameEnd(INVALID_CHAIR,NULL,GER_NORMAL);
	}
	return true;
}

bool CTableFrameSink::OnCompareCardFinish( WORD wChairID )
{
	//状态效验
	ASSERT(m_pITableFrame->GetGameStatus()==GS_OX_PLAY || m_pITableFrame->GetGameStatus()==GS_OX_ACCOUNTS);
	if (m_pITableFrame->GetGameStatus()!=GS_OX_PLAY && m_pITableFrame->GetGameStatus()!=GS_OX_ACCOUNTS) return true;
	ASSERT(!m_bCompare[wChairID]);

	IServerUserItem* pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID);
	//m_oLog.Log("玩家[%s]比牌完成。", pServerUserItem->GetUserData()->szAccounts);


	m_bCompare[wChairID]=true;
	bool bFinish = true;
	for (WORD i=0 ; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i] && !m_bCompare[i])
		{
			bFinish=false;
		}
	}
	if(bFinish)
	{
		WriteScore();
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FINISH,NULL, 0);
		
	}
	return true;
}

bool CTableFrameSink::OnHurryCard( WORD wChairID , WORD cbWords )
{
	CMD_S_HurryCard hurryCard;
	ZeroMemory(&hurryCard ,sizeof(hurryCard));
	hurryCard.wCurrentUser = wChairID;
	hurryCard.cbWords = cbWords;
	for (INT i=0 ; i<GAME_PLAYER; i++)
	{
		IServerUserItem *pServerItem = m_pITableFrame->GetServerUserItem(i);
		if(pServerItem == NULL || pServerItem->IsAndroidUser()) continue;
		m_pITableFrame->SendTableData(i , SUB_S_HURRY_CARD , &hurryCard , sizeof(CMD_S_HurryCard));
	}	
	return true;
}

void CTableFrameSink::CheckXijin()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser()) continue;
		if(m_GameLogic.GetBestCardType(m_cbHandCardData[i], MAX_COUNT) >= 10)
		{
			m_oLog.Log("玩家[%s]获得喜金座位[%d]身上携带筹码[%d]获得奖励元宝数[%d]",pIServerUserItem->GetAccounts(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserScore()->lBodyChip, m_lLikeIngot);
			m_pITableFrame->SendTableData(i,SUB_S_XIJIN,NULL,0);
			m_pITableFrame->WriteUserTask(i,1,1,m_lLikeIngot);
		}
	}
}

bool CTableFrameSink::CheckUserContribution( WORD wChairID , LONGLONG lBuffer )
{
	IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID) ;
	if(pServerUserItem == NULL) return false;
	if(pServerUserItem->IsAndroidUser()) return false;
	const tagUserScore *pUserScore = pServerUserItem->GetUserScore();

	if(pUserScore->lInfullContribution/200 < lBuffer || pUserScore->lIngotContribution/200 < lBuffer)
	{
		m_oLog.LogWithoutTimeMark("检查元宝贡献度：%s 充值贡献度%I64d/200 元宝贡献度%I64d/200 元宝累计%I64d 不满足",pServerUserItem->GetNickName(),pUserScore->lInfullContribution ,pUserScore->lIngotContribution, lBuffer);
		return false;
	}
	m_oLog.LogWithoutTimeMark("检查元宝贡献度：%s 充值贡献度%I64d/200 元宝贡献度%I64d/200 元宝累计%I64d 满足",pServerUserItem->GetNickName(),pUserScore->lInfullContribution ,pUserScore->lIngotContribution, lBuffer);
	return true;
}

bool CTableFrameSink::UpdateTask( BYTE cbBufferType/*=0*/ )
{
	m_lBuffer = m_lInitBuffer;
	m_oLog.LogWithoutTimeMark("更新聚宝盆[%d]元宝"  , m_lBuffer);
	return true;
}

void CTableFrameSink::logCard( WORD wChairID )
{
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	if(pIServerUserItem == NULL) return;

	m_oLog.Log(TEXT("玩家：%s 机器人[%s] 手牌：[%s%s][%s%s][%s%s][%s%s][%s%s] 牌型：%s") , pIServerUserItem->GetAccounts() , pIServerUserItem->IsAndroidUser()?"是":"否" , 
		cn(m_cbHandCardData[wChairID][0]),vn(m_cbHandCardData[wChairID][0]),cn(m_cbHandCardData[wChairID][1]),vn(m_cbHandCardData[wChairID][1]),cn(m_cbHandCardData[wChairID][2]),vn(m_cbHandCardData[wChairID][2]),
		cn(m_cbHandCardData[wChairID][3]),vn(m_cbHandCardData[wChairID][3]),cn(m_cbHandCardData[wChairID][4]),vn(m_cbHandCardData[wChairID][4]) , CardTypeName(m_GameLogic.GetBestCardType(m_cbHandCardData[wChairID],MAX_COUNT)));
	m_oLog.Log(TEXT("玩家：%s 机器人[%s] 奖励牌：[%s%s][%s%s][%s%s][%s%s][%s%s] 相同牌数：%d" ) , pIServerUserItem->GetAccounts() , pIServerUserItem->IsAndroidUser()?"是":"否" , 
		cn(m_cbRewardCardData[wChairID][0]),vn(m_cbRewardCardData[wChairID][0]),cn(m_cbRewardCardData[wChairID][1]),vn(m_cbRewardCardData[wChairID][1]),cn(m_cbRewardCardData[wChairID][2]),vn(m_cbRewardCardData[wChairID][2]),
		cn(m_cbRewardCardData[wChairID][3]),vn(m_cbRewardCardData[wChairID][3]),cn(m_cbRewardCardData[wChairID][4]),vn(m_cbRewardCardData[wChairID][4]),m_GameLogic.GetSameCount(m_cbHandCardData[wChairID], m_cbRewardCardData[wChairID]) );

}

void CTableFrameSink::SendTaskIngot()
{
	for (WORD wChairID=0 ; wChairID<GAME_PLAYER; wChairID++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
		if(pIServerUserItem == NULL) continue;
		logCard(wChairID);
		BYTE cbBuffer = INVALID_BYTE;
// 		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID] ,MAX_COUNT) == OX_DOUBLE_JOKE)
// 		{
// 			cbBuffer = BIG_BUFFER;
// 		}
		if(cbBuffer != INVALID_BYTE)
		{
// 			CMD_S_Send_Bowl bowl;
// 			ZeroMemory(&bowl , sizeof(bowl));
// 			bowl.dwUserID = pIServerUserItem->GetUserID();
// 			bowl.lIngot = m_lBuffer;
// 			UpdateTask();
// 			bowl.cbBowlType = cbBuffer;
// 			bowl.lNewIngot = m_lBuffer;
// 			//发送信息
// 			m_pITableFrame->SendGameData(SUB_S_SEND_BOWL,&bowl,sizeof(bowl));
// 			m_pITableFrame->WriteUserTask(wChairID , OX_DOUBLE_JOKE , 1 , bowl.lIngot);
// 			TCHAR szOutNickName[128]=TEXT("");
// 			HideNickName(pIServerUserItem->GetNickName() , szOutNickName);
// 			TCHAR szMessage[128]=TEXT("");
// 			_sntprintf(szMessage , sizeof(szMessage) , TEXT("喜报：%s在%s中拿到%s获得%ld元宝的聚宝盆！") , szOutNickName , m_pGameServiceOption->szGameRoomName , "花牌牛牛" , bowl.lIngot);
// 			m_oLog.Log("玩家[%s]获得聚宝盆 座位[%d]身上携带筹码[%I64d]获得奖励元宝数[%ld]",pIServerUserItem->GetAccounts(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserScore()->lBodyChip, bowl.lIngot);
// 			m_pITableFrame->SendGameNotice(szMessage , 4);
		}
		else if(m_GameLogic.GetSameCount(m_cbHandCardData[wChairID], m_cbRewardCardData[wChairID])>=m_iSameCountLine)
		{	
// 			if(pIServerUserItem->IsAndroidUser()) 
// 			{
// 				m_oLog.Log("小八跳过发放喜金!");
// 				continue;
// 			}
			BYTE cbCount = m_GameLogic.GetSameCount(m_cbHandCardData[wChairID], m_cbRewardCardData[wChairID]);
			CMD_S_Send_Bowl bowl;
			ZeroMemory(&bowl , sizeof(bowl));
			bowl.dwUserID = pIServerUserItem->GetUserID();
			bowl.lIngot = m_lLikeIngot;
			bowl.cbBowlType = cbBuffer;
			bowl.lNewIngot = m_lNewLikeIngot[cbCount-2];
			//发送信息
			m_pITableFrame->SendTableData(wChairID , SUB_S_SEND_BOWL,&bowl,sizeof(bowl));
			m_pITableFrame->WriteUserTask(wChairID , 10 , 1 , bowl.lNewIngot);
			if (!pIServerUserItem->IsAndroidUser())
			{
				m_oLog.Log("玩家[%s]获得喜金 座位[%d]身上携带筹码[%I64d]获得奖励元宝数[%ld]",pIServerUserItem->GetAccounts(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserScore()->lBodyChip, m_lNewLikeIngot[cbCount-2]);

			}

			if (cbCount > 4)//五张一样
			{
	 			//发送信息
	 			TCHAR szOutNickName[128]=TEXT("");
	 			HideNickName(pIServerUserItem->GetNickName() , szOutNickName);
	 			TCHAR szMessage[128]=TEXT("");
	 			_sntprintf(szMessage , sizeof(szMessage) , TEXT("喜报：%s在%s中拿到%s获得%ld元宝的喜金！") , szOutNickName , m_pGameServiceOption->szGameRoomName , "五张相同牌" , bowl.lNewIngot);
	 			m_pITableFrame->SendGameNotice(szMessage , 4);
			}
			//m_oLog.Log("玩家[%s]获得喜金 座位[%d]身上携带筹码[%I64d]获得奖励元宝数[%ld]",pIServerUserItem->GetAccounts(), pIServerUserItem->GetChairID(), pIServerUserItem->GetUserScore()->lBodyChip, m_lNewLikeIngot[cbCount-2]);
		}
	}
}

void CTableFrameSink::HideNickName( const TCHAR *szNickName , TCHAR *szOutNickName )
{
	int totallenth = 0;

	for (INT i=0 ; i<strlen(szNickName); i++)
	{
		if(szNickName[i] < 0)
		{
			i++;
			totallenth ++;
		}
		else
		{
			totallenth ++;
		}
	}

	int lenth = 0;

	std::string szOut = "";
	TCHAR szTemp[10] = {0};

	for (INT i=0 ; i<strlen(szNickName); i++)
	{


		bool bChinese = false;
		if(szNickName[i] < 0)
		{
			i++;
			lenth ++;
			bChinese = true;
		}
		else
		{
			lenth ++;
		}

		if(lenth<=2 || lenth>totallenth-2)
		{
			ZeroMemory(szTemp , sizeof(szTemp));
			if(bChinese)
			{
				szTemp[0] = szNickName[i-1];
				szTemp[1] = szNickName[i];
			}
			else
			{
				szTemp[0] = szNickName[i];
			}			
		}
		else
		{
			sprintf(szTemp , "*");
		}
		szOut+=szTemp;
	}
	sprintf(szOutNickName , "%s" , szOut.c_str());

}

bool CTableFrameSink::AddBuffer( BYTE cbBufferType/*=0 */, LONGLONG lChip )
{
	m_oLog.Log("聚宝盆 累积前[%I64d], [%I64d]筹码放进聚宝盆 ,上局累积值：[%I64d] ,筹码兑换比例[%I64d]", 
		 m_lBuffer, lChip, m_lBufferFund, m_lCellScore);
	lChip += m_lBufferFund;
	for(int i=0; i<4; i++)
	{
		if(m_lBuffer < m_lBufferNumSection[i])
		{
			if (m_pGameServiceOption->wServerType == GAME_GENRE_SCORE_CHIP)
			{
				//m_lBuffer[cbBufferType] += lChip*m_lCellScore*200*m_cbBufferGrandRate[cbBufferType][i]/100/400;
				//m_lBufferFund[cbBufferType] = (lChip*m_lCellScore*200*m_cbBufferGrandRate[cbBufferType][i]/100%400)*100/m_cbBufferGrandRate[cbBufferType][i]/m_lCellScore/200;
				m_lBuffer += lChip*m_lCellScore*20*m_cbBufferGrandRate[i]/100/400;
				m_lBufferFund = (lChip*m_lCellScore*20*m_cbBufferGrandRate[i]/100%400)*100/m_cbBufferGrandRate[i]/m_lCellScore/20;
			}
			else
			{
				m_lBuffer += lChip*m_lCellScore*m_cbBufferGrandRate[i]/100/400;
				m_lBufferFund = (lChip*m_lCellScore*m_cbBufferGrandRate[i]/100%400)*100/m_cbBufferGrandRate[i]/m_lCellScore;
			}			
			break;
		}
	}
	m_oLog.Log("聚宝盆累积后[%I64d], 不能整除的累计值[%I64d]", m_lBuffer, m_lBufferFund);
	if(m_lBuffer > m_lBufferNumSection[3])
	{
		m_lBuffer = m_lBufferNumSection[3];
	}
	return true;
}

bool CTableFrameSink::CheckBufferForce( BYTE cbBufferType/*=0*/ )
{
	return m_lBuffer >= m_lBufferNumSection[2];
}

bool CTableFrameSink::ForceDispathCard()
{
	bool bForce = CheckBufferForce();
	if(!(bForce && !m_bMeetConditions)) return false;
	bool bCondition=false;
	WORD wChairID=INVALID_CHAIR;
	for (WORD i=0 ; i<GAME_PLAYER; i++)
	{
		bCondition = CheckUserContribution(i , m_lBuffer);
		if(bCondition)
		{
			wChairID = i;
			m_bMeetConditions=true;
			break;
		}
	}
	if(!bCondition) return false;
	BYTE cbBuffer = OX_DOUBLE_JOKE;
	BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
	BYTE cbMathCard[MAX_COUNT];
	ZeroMemory(cbMathCard , sizeof(cbMathCard));
	CopyMemory(cbMathCard , Contral::m_cbDefaultCardType[cbBuffer] , sizeof(cbMathCard));
	m_GameLogic.RandCardList(bTempArray , GAME_PLAYER*MAX_COUNT , false);
	m_GameLogic.RandMathCard(bTempArray , GAME_PLAYER*MAX_COUNT , cbMathCard , wChairID);
	m_GameLogic.MathCardList(bTempArray , GAME_PLAYER*MAX_COUNT , cbMathCard , wChairID);
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;

		//派发扑克
		CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}
	return true;
}

bool __cdecl CTableFrameSink::OnActionUserReConnect( WORD wChairID, IServerUserItem * pIServerUserItem )
{
	CMD_S_OffLine_Reconn OffLine;
	OffLine.wOperaUser = wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_RECONN,&OffLine,sizeof(CMD_S_OffLine_Reconn));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_RECONN,&OffLine,sizeof(CMD_S_OffLine_Reconn));
	return true;
}


//////////////////////////////////////////////////////////////////////////
