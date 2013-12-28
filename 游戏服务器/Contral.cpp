#include "Stdafx.h"
#include "Contral.h"
#include "TableFrameSink.h"

#define ABS(a) ((a)>0)?(a):(-(a))

void StrSplit(std::string src, std::string token, std::vector<std::string>& vect)    
{    
	int nend=0;    
	int nbegin=0;    
	while(nend != -1)    
	{    
		nend = src.find_first_of(token, nbegin);    
		if(nend == -1)    
			vect.push_back(src.substr(nbegin, src.length()-nbegin));    
		else   
			vect.push_back(src.substr(nbegin, nend-nbegin));    
		nbegin = nend + 1;    
	}    
}   

BYTE				Contral::m_cbDefaultCardType[13][MAX_COUNT]={{1,2,3,2,1} ,{10,10,10,10,1},{10,10,10,10,2},{10,10,10,10,3},{10,10,10,10,4},{10,10,10,10,5},{10,10,10,10,6},{10,10,10,10,7},{10,10,10,10,8},{10,10,10,10,9},{10,10,10,10,0xD},{0xD,0xD,0xD,0xD,0xE},{0xD,0xD,0xD,0xE,0xF} };
LONGLONG			Contral::m_lThreshold[5]={0};
LONGLONG			Contral::m_lScorePool=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////
Contral::Contral(void)
{

}

Contral::~Contral(void)
{

}



void Contral::Init(ITableFrame *pTableFrame,CLog *pLog,LONG lCellScore , WORD wRevenue)
{
	m_pTableFrame = pTableFrame;
	m_pLog = pLog;
	m_lCellScore = lCellScore;
	m_wRevenue = wRevenue;
}

void Contral::LoadCfg( TCHAR *szINIPath, int iEatRate/*=0*/ )
{
	srand(GetTickCount());
	//配置信息
	TCHAR szBuffer[MAX_PATH];
	std::string strAnimalSqu;
	std::vector<std::string> vtStrAnimalPrizeSeq;

	GetPrivateProfileString(TEXT("SCOREPOOL"),TEXT("ScoreValue"),TEXT("0,0"),szBuffer,CountArray(szBuffer),szINIPath);
	strAnimalSqu = szBuffer;
	vtStrAnimalPrizeSeq.clear();

	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
	{
		m_lScoreValue[i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
	}

// 	GetPrivateProfileString(TEXT("CONTRAL"),TEXT("Threshold"),TEXT("0,0,0,0,0"),szBuffer,CountArray(szBuffer),szINIPath);
// 	strAnimalSqu = szBuffer;
// 	vtStrAnimalPrizeSeq.clear();

// 	StrSplit(strAnimalSqu,",",vtStrAnimalPrizeSeq);
// 	for (int i = 0; i < vtStrAnimalPrizeSeq.size(); i++)
// 	{
// 		m_lThreshold[i]=_atoi64(vtStrAnimalPrizeSeq[i].c_str());
// 	}

	m_WinRete=GetPrivateProfileInt("SCOREPOOL","WinRate",20,szINIPath);
	m_LoseRate=GetPrivateProfileInt("SCOREPOOL","LoseRate",100,szINIPath);
	m_cbBufferRate=GetPrivateProfileInt("BUFFER","BufferRate",10,szINIPath);
	GetPrivateProfileString(TEXT("SCOREPOOL"),TEXT("ScorePool"),TEXT("0"),szBuffer,CountArray(szBuffer),szINIPath);
	std::string strScorePool;
	strScorePool = szBuffer;
	m_lScorePool = _atoi64(strScorePool.c_str());
	m_cbSysEateRate=iEatRate;

// 	for (INT i=0 ; i<2 ; i++)
// 	{
// 		if (m_lScorePool[i] == 0)
// 		{
// 			m_lScorePool[i] = m_lScoreValue[i];
// 		}
// 	}

}

BYTE Contral::GetNextGamePrizeMode()
{

	for(int i=0; i<GAME_PLAYER; i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem = m_pTableFrame->GetServerUserItem(i);
		if (pIServerUserItem == NULL || pIServerUserItem->IsAndroidUser()) continue;
		m_bContral[i] = CheckTriggerContral(m_pTableFrame->GetServerUserItem(i));
		if(m_bContral[i] != 0)
		{
			m_cbMode=1;
			break;
		}
	}
	if(m_cbMode != 1)
	{
		m_cbMode=0;
	}
// 	m_pLog->Log("游戏模式[%s] 放分池[%I64d][%I64d][%I64d][%I64d][%I64d] 炒场概率[%d] 阀值概率[%d]" , ((m_cbMode==0)?"随机":"控制"),m_lBuffer[0],m_lBuffer[1],m_lBuffer[2],m_lBuffer[3],m_lBuffer[4],m_cbFriedRate,m_cbThresholdRate);
	return 0;
}

void Contral::Reset()
{
	m_cbMode=0;
	ZeroMemory(m_bContral , sizeof(m_bContral));
}

BYTE Contral::CheckTriggerContral(IServerUserItem* pServerUserItem)
{
	if(pServerUserItem == NULL) return false;
	const tagUserScore* pUserScore = pServerUserItem->GetUserScore();
	if(pUserScore == NULL) return false;
	if(pUserScore->lFriedFieldValue > 0 && rand()%100 < m_cbFriedRate)
	{
		m_pLog->Log("玩家[%s] 炒场[%ld] 阀值[%ld] 净分[%I64d] 触发炒场" , pServerUserItem->GetNickName() , pUserScore->lFriedFieldValue ,pUserScore->lThreshold, pUserScore->lLostMoney);
		return 1;
	}
	else if(pUserScore->lLostMoney-pUserScore->lThreshold > 0 && rand()%100 < m_cbThresholdRate)
	{
		m_pLog->Log("玩家[%s] 炒场[%ld] 阀值[%ld] 净分[%I64d] 触发阀值" , pServerUserItem->GetNickName() , pUserScore->lFriedFieldValue ,pUserScore->lThreshold, pUserScore->lLostMoney);
		return 2;
	}
	m_pLog->Log("玩家[%s] 炒场[%ld] 阀值[%ld] 净分[%I64d] 没有触发控制" , pServerUserItem->GetNickName() , pUserScore->lFriedFieldValue ,pUserScore->lThreshold, pUserScore->lLostMoney);
	return 0;
}

BOOL Contral::CheckBuffer(LONGLONG lScore)
{
// 	lScore = -lScore;
// 	for(int i=0 ;i<5; i++)
// 	{
// 		if(m_lScorePool[i] >= m_lThreshold[i] && m_lScorePool[i] >= lScore){
// 			return true;
// 		}
// 	}
// 	return false;
	return true;
}

VOID Contral::DeductionBuffer(LONGLONG lScore)
{
// 	m_pLog->LogWithoutTimeMark("放分 ：DeductionBuffer = [%I64d]" , lScore);
// 	for(int i=0 ;i<5; i++)
// 	{
// 		if(m_lScorePool[i] >= m_lThreshold[i] && m_lScorePool[i] >= lScore){
// 			m_lScorePool[i] -= lScore;
// 			break;
// 		}
// 	}
// 	m_pLog->LogWithoutTimeMark("放分后放分池 ：Buffer[%I64d][%I64d][%I64d][%I64d][%I64d]" , m_lBuffer[0],m_lBuffer[1],m_lBuffer[2],m_lBuffer[3],m_lBuffer[4]);
}

VOID Contral::AddBuffer(LONGLONG lScore)
{
// 	m_pLog->LogWithoutTimeMark("税收前 ：AddBuffer = [%I64d]" , lScore);
// 	lScore -= lScore*(m_cbSysEateRate+m_cbBufferRate)/100;
// 	m_pLog->LogWithoutTimeMark("税收后 ：AddBuffer = [%I64d]" , lScore);
// 	for(int i=0 ;i<5; i++)
// 	{
// 		m_lScorePool[i] += lScore/5;
// 	}
// 	m_pLog->LogWithoutTimeMark("吃分后放分池 ：Buffer[%I64d][%I64d][%I64d][%I64d][%I64d]", m_lBuffer[0],m_lBuffer[1],m_lBuffer[2],m_lBuffer[3],m_lBuffer[4]);
}


LONGLONG Contral::CalculateSysScore(LONGLONG lUserWinScore[GAME_PLAYER])
{
	//系统输赢
	LONGLONG lSystemScore = 0l;

	for(WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++){
		//获取用户
		IServerUserItem * pIServerUserItem = m_pTableFrame->GetServerUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		if(!pIServerUserItem->IsAndroidUser())
		{
			lSystemScore -= lUserWinScore[wChairID];
		}

	}
	return lSystemScore;
}

LONGLONG Contral::CalculateContralScore(LONGLONG lUserWinScore[GAME_PLAYER])
{
	LONGLONG lContralScore=0;
	for(int k=0 ; k<GAME_PLAYER ; k++)
	{
		if(m_bContral[k] != 0)
		{
			IServerUserItem* pServerUserItem = m_pTableFrame->GetServerUserItem(k);
			if(pServerUserItem == NULL) continue;
			const tagUserScore* pUserScore = pServerUserItem->GetUserScore();
			if(pUserScore == NULL) continue;
			if(m_bContral[k] == 1)
			{
				m_pLog->LogWithoutTimeMark("炒场%s 控制得分：%I64d=%ld-%I64d" , pServerUserItem->GetNickName(), ABS((pUserScore->lFriedFieldValue-lUserWinScore[k]*m_lCellScore)), pUserScore->lFriedFieldValue , lUserWinScore[k]*m_lCellScore);
				lContralScore += ABS((pUserScore->lFriedFieldValue-lUserWinScore[k]*m_lCellScore));
			}
			else if(m_bContral[k] == 2)
			{
				m_pLog->LogWithoutTimeMark("阀值%s 控制得分：%I64d=%I64d+%I64d" , pServerUserItem->GetNickName() ,ABS((pUserScore->lLostMoney-pUserScore->lThreshold+lUserWinScore[k]*m_lCellScore)), pUserScore->lLostMoney-pUserScore->lThreshold , lUserWinScore[k]*m_lCellScore);
				lContralScore += ABS((pUserScore->lLostMoney-pUserScore->lThreshold+lUserWinScore[k]*m_lCellScore));
			}
		}		
	}

	return lContralScore;
}

void Contral::CalculateScore(LONGLONG lPlayScore[GAME_PLAYER] ,WORD wBankerUser ,BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT], LONGLONG& dwSysScore , LONGLONG& dwContralScore)
{
	LONGLONG lUserWinScore[GAME_PLAYER] = {0};
//	m_GameLogic.GetWinScore(wBankerUser , cbHandCardData , lPlayScore , lUserWinScore ,m_wRevenue);
	dwSysScore = CalculateSysScore(lUserWinScore);
	dwContralScore = CalculateContralScore(lUserWinScore);
}

void Contral::DispathCard( BYTE cbHandCardData[GAME_PLAYER*MAX_COUNT] , LONGLONG lPlayScore[GAME_PLAYER] ,BYTE cbPlayStatus[GAME_PLAYER] ,WORD wBankerUser )
{
	RandDispath(cbHandCardData , lPlayScore , wBankerUser);
	


// 	if(m_cbMode == 1)
// 	{
// 		BOOL bSuccess = MathOptimalCard(cbHandCardData , lPlayScore , wBankerUser);
// 		if(!bSuccess)
// 		{
// 			RandDispath(cbHandCardData , lPlayScore , wBankerUser);
// 		}
// 	}
}

BOOL Contral::MathOptimalCard(BYTE cbMathCardData[GAME_PLAYER*MAX_COUNT] , LONGLONG lPlayScore[GAME_PLAYER] , WORD wBankerUser)
{
	BYTE cbContralIndex[2]={INVALID_BYTE,INVALID_BYTE};
	BYTE cbIndex = 0;
	for (INT i=0 ; i<GAME_PLAYER ; i++)
	{
		if(m_bContral[i] != 0)
		{
			cbContralIndex[cbIndex++] = i;
		}
	}
	BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT]={0};
	m_vcPrizeValue.clear();
	LONGLONG lOptimalScore = 0xFFFFFFFFFFFFFFF;
	std::string szStr="";
	TCHAR szChar[1280]={0};

	for (INT i=0 ; i<11; i++)
	{
		for (INT j=0; j<11; j++)
		{
			BYTE cbMath1[MAX_COUNT]={0};
			CopyMemory(cbMath1 , m_cbDefaultCardType[i] , MAX_COUNT);
			BYTE *cbMath2=NULL;
			if(cbContralIndex[1] != INVALID_BYTE)
			{
				cbMath2 = new BYTE[MAX_COUNT];
				CopyMemory(cbMath2 , m_cbDefaultCardType[j] , MAX_COUNT);
			}
			else
			{
				if(j>0) break;
			}
			m_GameLogic.RandMathCard(cbMathCardData , GAME_PLAYER*MAX_COUNT , cbMath1 , cbContralIndex[0] , cbMath2 , cbContralIndex[1]);
			m_GameLogic.MathCardList(cbMathCardData , GAME_PLAYER*MAX_COUNT , cbMath1 , cbContralIndex[0] , cbMath2 , cbContralIndex[1]);
			if(cbContralIndex[1] != INVALID_BYTE)
			{
				delete[] cbMath2;
			}
			for (WORD k=0;k<GAME_PLAYER;k++)
			{
				//派发扑克
				CopyMemory(cbHandCardData[k],&cbMathCardData[k*MAX_COUNT],MAX_COUNT);
				m_GameLogic.SortCardList(cbHandCardData[k],MAX_COUNT);
			}
			STModePrizeValue stModePrizeValue;
			ZeroMemory(&stModePrizeValue ,sizeof(stModePrizeValue));
			CopyMemory(stModePrizeValue.cbMathCardData , cbHandCardData, GAME_PLAYER*MAX_COUNT);
			CalculateScore(lPlayScore , wBankerUser , cbHandCardData , stModePrizeValue.lSysWinScore , stModePrizeValue.lContralScore);
			stModePrizeValue.bValid = CheckBuffer(stModePrizeValue.lSysWinScore);
			CheckOptimalScore(stModePrizeValue , lOptimalScore , m_vcPrizeValue);
			szStr = "";
			sprintf(szChar , "玩家[%d] 配牌牌型：牛[%d] 实际牌型[%d] 牌[%d][%d][%d][%d][%d]" , cbContralIndex[0] , i ,m_GameLogic.GetCardType(cbMath1,MAX_COUNT) , m_GameLogic.GetCardValue(cbMath1[0]), m_GameLogic.GetCardValue(cbMath1[1]), m_GameLogic.GetCardValue(cbMath1[2]), m_GameLogic.GetCardValue(cbMath1[3]), m_GameLogic.GetCardValue(cbMath1[4]));
			szStr += szChar;
			if(cbContralIndex[1] != INVALID_BYTE)
			{
				sprintf(szChar , " 玩家[%d] 配牌牌型：牛[%d] 实际牌型[%d] 牌[%d][%d][%d][%d][%d]" , cbContralIndex[1] , j,m_GameLogic.GetCardType(cbMath1,MAX_COUNT) , m_GameLogic.GetCardValue(cbMath2[0]), m_GameLogic.GetCardValue(cbMath2[1]), m_GameLogic.GetCardValue(cbMath2[2]), m_GameLogic.GetCardValue(cbMath2[3]), m_GameLogic.GetCardValue(cbMath2[4]));
				szStr += szChar;
			}
			m_pLog->LogWithoutTimeMark(szStr.c_str());
			szStr = "最终牌：";
			for(INT k=0 ; k<GAME_PLAYER*MAX_COUNT; k++)
			{
				sprintf(szChar ,"[%d]",m_GameLogic.GetCardValue(cbMathCardData[k]));
				szStr += szChar;
			}
			m_pLog->LogWithoutTimeMark(szStr.c_str());
			m_pLog->LogWithoutTimeMark("系统输赢：[%I64d] ,控制得分：[%I64d] , %s" , stModePrizeValue.lSysWinScore , stModePrizeValue.lContralScore , stModePrizeValue.bValid?"有效":"无效");
		}
	}
	if(m_vcPrizeValue.size() == 0) return false;
	STModePrizeValue stValue = m_vcPrizeValue[rand()%m_vcPrizeValue.size()];
	CopyMemory(cbMathCardData , stValue.cbMathCardData , GAME_PLAYER*MAX_COUNT);
	m_pLog->LogWithoutTimeMark("最优解：系统输赢：[%I64d] ,控制得分：[%I64d] , %s" , stValue.lSysWinScore , stValue.lContralScore , stValue.bValid?"有效":"无效");
	szStr = "最优解：";
	for(INT k=0 ; k<GAME_PLAYER*MAX_COUNT; k++)
	{
		sprintf(szChar ,"[%d]",m_GameLogic.GetCardValue(cbMathCardData[k]));
		szStr += szChar;
	}
	return true;
}

VOID Contral::CheckOptimalScore(STModePrizeValue &stModePrizeValue , LONGLONG &lOptimalScore , std::vector<STModePrizeValue>& vcSTModePrizeValue)
{
	if(stModePrizeValue.bValid)
	{
		if(stModePrizeValue.lContralScore < lOptimalScore)
		{
			lOptimalScore=stModePrizeValue.lContralScore;
			vcSTModePrizeValue.clear();
			vcSTModePrizeValue.push_back(stModePrizeValue);
		}
		else if(stModePrizeValue.lContralScore == lOptimalScore)
		{
			vcSTModePrizeValue.push_back(stModePrizeValue);
		}
	}
}

void Contral::RandDispath( BYTE cbMathCardData[GAME_PLAYER*MAX_COUNT] , LONGLONG lPlayScore[GAME_PLAYER] , WORD wBankerUser )
{
	BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT]={0};
	m_GameLogic.RandCardList(cbMathCardData , GAME_PLAYER*MAX_COUNT);
	for (WORD k=0;k<GAME_PLAYER;k++)
	{
		//派发扑克
		IServerUserItem *pServerUserItem = m_pTableFrame->GetServerUserItem(k);
		if(pServerUserItem == NULL)
		{
			ZeroMemory(cbMathCardData+k*MAX_COUNT,MAX_COUNT);
		}
		CopyMemory(cbHandCardData[k],&cbMathCardData[k*MAX_COUNT],MAX_COUNT);
		m_GameLogic.SortCardList(cbHandCardData[k],MAX_COUNT);
	}
// 	while(CheckTask(cbMathCardData,cbHandCardData))
// 	{
// 		m_GameLogic.RandCardList(cbMathCardData , GAME_PLAYER*MAX_COUNT);
// 		for (WORD k=0;k<GAME_PLAYER;k++)
// 		{
// 			IServerUserItem *pServerUserItem = m_pTableFrame->GetServerUserItem(k);
// 			if(pServerUserItem == NULL)
// 			{
// 				ZeroMemory(cbMathCardData+k*MAX_COUNT,MAX_COUNT);
// 			}
// 			//派发扑克
// 			CopyMemory(cbHandCardData[k],&cbMathCardData[k*MAX_COUNT],MAX_COUNT);
// 			m_GameLogic.SortCardList(cbHandCardData[k],MAX_COUNT);
// 		}
// 	}
}

bool Contral::CheckTask(BYTE cbMathCardData[GAME_PLAYER*MAX_COUNT],BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT])
{
	for (WORD k=0;k<GAME_PLAYER;k++)
	{
		IServerUserItem *pServerUserItem = m_pTableFrame->GetServerUserItem(k);
		if(pServerUserItem == NULL)
		{
			continue;
		}
		//派发扑克
		CopyMemory(cbHandCardData[k],&cbMathCardData[k*MAX_COUNT],MAX_COUNT);
		m_GameLogic.SortCardList(cbHandCardData[k],MAX_COUNT);

		if (m_GameLogic.GetCardType(cbHandCardData[k],MAX_COUNT) == 10 && pServerUserItem->GetUserScore()->lIngotContribution < 10)
		{
			return true;
		}
	}
	return false;
}

BOOL Contral::CheckSimpleBuffer(BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT] , LONGLONG lPlayScore[GAME_PLAYER] , WORD wBankerUser)
{
	LONGLONG dwSysPrize=0;
	LONGLONG dwContralPrize=0;
	CalculateScore(lPlayScore ,wBankerUser,cbHandCardData, dwSysPrize, dwContralPrize);
	return CheckBuffer(dwSysPrize);
}

WORD Contral::GetRandUser()
{
	WORD wUser[GAME_PLAYER];
	ZeroMemory(wUser, sizeof(wUser));
	int iUserCount =0 ;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_pTableFrame->GetServerUserItem(i) == NULL) continue;
		if (m_pTableFrame->GetServerUserItem(i)->IsAndroidUser())continue;
		wUser[iUserCount] = i;
		iUserCount++;

	}
	ASSERT(iUserCount>0);
	int iSelectUser =  rand() % iUserCount;

	return wUser[iSelectUser];
}

WORD Contral::GetRandAndroid()
{
	WORD wAndroid[GAME_PLAYER];
	ZeroMemory(wAndroid, sizeof(wAndroid));
	int iAndroidCount =0 ;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_pTableFrame->GetServerUserItem(i) == NULL) continue;
		if (!m_pTableFrame->GetServerUserItem(i)->IsAndroidUser())continue;
		wAndroid[iAndroidCount] = i;
		iAndroidCount++;

	}
	ASSERT(iAndroidCount>0);
	int iSelectUser =  rand() % iAndroidCount;

	return wAndroid[iSelectUser];
}

void Contral::ControlDispath( BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT],BYTE cbPlayStatus[GAME_PLAYER] )
{
	//判断是都全部机器人
	bool bIsAllAndroid = true;

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (cbPlayStatus[i] == FALSE) continue;
		if (!m_pTableFrame->GetServerUserItem(i)->IsAndroidUser())
		{
			bIsAllAndroid = false;
			break;
		}	
	}
	//判断是否都是玩家
	bool bIsAllUser = true;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (cbPlayStatus[i] == FALSE) continue;
		if (m_pTableFrame->GetServerUserItem(i)->IsAndroidUser())
		{
			bIsAllUser = false;
			break;
		}	
	}


	if (!bIsAllAndroid && !bIsAllUser)
	{
		if (m_lScorePool < m_lScoreValue[1])		//低于阀值
		{
			WORD wWinUser = m_GameLogic.GetWinUser(cbPlayStatus, cbHandCardData);
			if (m_pTableFrame->GetServerUserItem(wWinUser)->IsAndroidUser())//赢家是机器人
			{
				return;
			}
			else		//赢家是玩家
			{
				if((rand()%100) < m_LoseRate)	//满足必输概率
				{
					WORD wAndroid = GetRandAndroid();
					BYTE tempCard[MAX_COUNT];
					CopyMemory(tempCard, cbHandCardData[wWinUser],sizeof(tempCard));
					//调换牌
					CopyMemory(cbHandCardData[wWinUser], cbHandCardData[wAndroid],sizeof(tempCard));
					CopyMemory(cbHandCardData[wAndroid], tempCard,sizeof(tempCard));
				}
				else return;
						
			}
		}
		else if (m_lScorePool < m_lScoreValue[0])	//低于第一控制值
		{
			return;
		}
		else
		{
			WORD wWinUser = m_GameLogic.GetWinUser(cbPlayStatus, cbHandCardData);
			if (m_pTableFrame->GetServerUserItem(wWinUser)->IsAndroidUser())//赢家是机器人
			{
				if((rand()%100) < m_WinRete)   //满足必赢概率
				{
					WORD wUser = GetRandUser();
					BYTE tempCard[MAX_COUNT];
					CopyMemory(tempCard, cbHandCardData[wWinUser],sizeof(tempCard));
					//调换牌
					CopyMemory(cbHandCardData[wWinUser], cbHandCardData[wUser],sizeof(tempCard));
					CopyMemory(cbHandCardData[wUser], tempCard,sizeof(tempCard));
				}
				else return;
			}
			else		//赢家是玩家
			{
				return;
			}
		}
	}
}

LONGLONG Contral::GetSysWinScore( LONGLONG lWinScore[GAME_PLAYER], WORD wWinUser )
{
	ASSERT(0<=wWinUser<GAME_PLAYER);
	ASSERT(lWinScore[wWinUser]>0);
	LONGLONG lSysWinScore =0; 
	if (m_pTableFrame->GetServerUserItem(wWinUser)->IsAndroidUser()) //赢家是机器人
	{
		for (int i=0; i<GAME_PLAYER; i++)
		{
			if (m_pTableFrame->GetServerUserItem(i) == NULL) continue;
			if (i==wWinUser)continue;
			if (m_pTableFrame->GetServerUserItem(i)->IsAndroidUser()) continue;
			
			lSysWinScore+= -(lWinScore[i]);
		}
	}
	else					//赢家是玩家
	{
		for (int i=0; i<GAME_PLAYER; i++)
		{
			if (m_pTableFrame->GetServerUserItem(i) == NULL) continue;
			if (i==wWinUser)continue;
			if (!m_pTableFrame->GetServerUserItem(i)->IsAndroidUser()) continue;

			lSysWinScore+=lWinScore[i];
		}
	}

	return lSysWinScore;
}

VOID Contral::ChangeBuffer( LONGLONG lScore )
{	
 	m_pLog->Log("抽水前 ：AddScorePool = [%I64d]" , lScore);
	if(lScore > 0)
 	lScore -= lScore*m_cbSysEateRate/100;
 	m_pLog->Log("抽水后 ：AddScorePool = [%I64d]" , lScore);
	m_lScorePool +=lScore;
 	m_pLog->Log("写入后放分池 ：ScorePool[%I64d]", m_lScorePool);
	
}

LONGLONG Contral::GetPoolScore()
{
	return m_lScorePool;
}

