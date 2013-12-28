#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					2									//最少时间

//游戏时间
#define TIME_START_GAME				3									//开始时间
//下注时间
#define TIME_OPEN_CARD				5									//摊牌时间

//游戏时间
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+1)			//开始时间
#define IDI_OPEN_CARD				(IDI_ANDROID_ITEM_SINK+2)			//摊牌时间
#define IDI_COMPARE_FINISH			(IDI_ANDROID_ITEM_SINK+4)			//下注时

#define IDI_READ_CONFIG				(IDI_ANDROID_ITEM_SINK+3)			//下注时间

#define IDI_LEAVE_GAME				(IDI_ANDROID_ITEM_SINK+6)			//结束离开
//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{

}

//接口查询
void * __cdecl CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool __cdecl CAndroidUserItemSink::InitUserItemSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;


	return true;
}

//重置接口
bool __cdecl CAndroidUserItemSink::RepositUserItemSink()
{

	return true;
}

//时间消息
bool __cdecl CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
 	switch(nTimerID)
 	{
 	case IDI_START_GAME:
 		{
			m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
			//玩家设置
			if (m_pIAndroidUserItem->GetMeUserItem()->GetUserStatus()!=US_SIT)
			{
				UINT nElapse=rand()%2+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
				return true;
			}
			//准备消息
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			
			return true;
 		}

	case IDI_OPEN_CARD:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);
			CMD_C_OxCard OxCard;
			OxCard.bOX = TRUE;
			
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD, &OxCard, sizeof(OxCard));
			return true;
		}
	case IDI_LEAVE_GAME:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_LEAVE_GAME);
			m_pIAndroidUserItem->SendUserReady(NULL,1);
			return true;
		}
	case IDI_COMPARE_FINISH:
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE_FINISH);
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD);
			return true;
		}

 	}

	return false;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
		{
		case SUB_S_GAME_START:		//游戏开始
			{
				return OnSubGameStart(pBuffer,wDataSize);
			}
		case SUB_S_GAME_END:		//游戏结束
			{
				return OnSubGameEnd(pBuffer,wDataSize);
			}
		case SUB_S_GAME_FINISH:
			{
				return OnSubGameAllFinish(pBuffer,wDataSize);
			}
		}
	
		//错误断言
		//ASSERT(FALSE);

	return true;
}

//游戏消息
bool __cdecl CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_GR_ROOM_OPTION:
		if (sizeof(CMD_CF_RoomOption) != wDataSize)
		{
			return false;
		}
		CMD_CF_RoomOption *pRoomOption = (CMD_CF_RoomOption *)pData;
		m_wServerType = pRoomOption->wServerType;
		//m_lCellScore = pRoomOption->lCellScore;
		break;
	}
	return true;
}

//场景消息
bool __cdecl CAndroidUserItemSink::OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//空闲状态
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;
			
			CopyMemory(m_lExchangeChip, pStatusFree->lChipNum, sizeof(m_lExchangeChip));
			m_lMinBet = pStatusFree->lMinBet;

			OnChangeChip();

			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();
			//玩家设置
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()% TIME_START_GAME + TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}
			
	
			//设置状态
			m_pIAndroidUserItem->SetGameStatus(GS_FREE);
			return true;
		}
	}

	return false;
}

//用户进入
void __cdecl CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void __cdecl CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void __cdecl CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void __cdecl CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void __cdecl CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

// 所有玩家都开始了
void __cdecl CAndroidUserItemSink::OnEventAllUserStart(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return ;
}


/*****************************************************************************************************************
*游戏特有函数定义开始
*****************************************************************************************************************/
//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;
	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_OX_PLAY);

	//消息处理
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )return FALSE;
		
	
	UINT nElapse=rand()%TIME_OPEN_CARD+3;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);


	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	UINT nElapse=rand()%1+1;
	m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_FINISH,nElapse);
	/*m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD);*/

	//设置状态
	m_pIAndroidUserItem->SetGameStatus(GS_OX_ACCOUNTS);
	return true;
}

bool CAndroidUserItemSink::OnChangeChip()
{
	IServerUserItem *pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();
	ASSERT(pIServerUserItem != NULL);
	if(pIServerUserItem == NULL) return false;
	const tagUserScore *pUserScore = pIServerUserItem->GetUserScore();

	
	if (pUserScore->lBodyChip <2* m_lMinBet)
	{
		int Index = rand() % MAX_CHIP_COUNT;
		
		LONGLONG lChangeChip = m_lExchangeChip[Index];

		lChangeChip = m_lExchangeChip[Index];

		m_pIAndroidUserItem->SendExchangeChip(lChangeChip);

	}
	return true;
}

bool CAndroidUserItemSink::OnSubGameAllFinish( const void * pBuffer, WORD wDataSize )
{
	m_pIAndroidUserItem->SetGameStatus(GS_FREE);
	IServerUserItem * pIServerUserItem = m_pIAndroidUserItem->GetMeUserItem();
	const tagUserScore *pUserScore = pIServerUserItem->GetUserScore();
	UINT nElapse = rand()%3 + 1;
	if (pUserScore->lBodyChip > 3*m_lMinBet)
	{
		int irand = rand()%100;
		if (irand < 70)
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME, nElapse);
		}
		else
		{
			m_pIAndroidUserItem->SetGameTimer(IDI_LEAVE_GAME, nElapse);
		}
		
	}
	else
	{

		/*m_pIAndroidUserItem->SendExchangeChip(-pUserScore->lBodyChip);*/
		m_pIAndroidUserItem->SetGameTimer(IDI_LEAVE_GAME, nElapse);
	}
	return true;
}


/*****************************************************************************************************************
*游戏特有函数定义结束
*****************************************************************************************************************/


//////////////////////////////////////////////////////////////////////////
