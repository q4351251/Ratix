#include "StdAfx.h"
#include "GameLogic.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//获取类型
BYTE CGameLogic::GetCardType( BYTE cbCardData[], BYTE cbCardCount, BYTE cbOutCard[]/*=NULL*/, bool bGet/*=false*/ )
{
	ASSERT(cbCardCount==MAX_COUNT);

	BYTE bKingCount=0,bJokeCount=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if(GetCardValue(cbCardData[i])>10)
		{
			bKingCount++;
			if(GetCardValue(cbCardData[i])>=0xE)
			{
				bJokeCount++;
			}
		}
	}

	if(bKingCount==MAX_COUNT)
	{
		if(bJokeCount == 2) return OX_DOUBLE_JOKE;
		return OX_GOLD;
	}

	BYTE bTemp[MAX_COUNT];
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				if (bGet)
				{
					cbOutCard[0] = cbCardData[i];
					cbOutCard[1] = cbCardData[j];
				}
				return ((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
			}
		}
	}

	return OX_VALUE0;
}
//获得玩家给定牌序列的牌型
BYTE CGameLogic::GetListCardType( BYTE cbCardData[], BYTE cbCardCount )
{
	return OX_VALUE0;
}

//获取倍数
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes=GetBestCardType(cbCardData,MAX_COUNT);
	if(bTimes<10)return 1;
	else if(bTimes>=10)return 2;
	return 0;
}

//获取牛牛
bool CGameLogic::GetOxCard(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	//设置变量
	BYTE bTemp[MAX_COUNT],bTempData[MAX_COUNT];
	CopyMemory(bTempData,cbCardData,sizeof(bTempData));
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}

	//查找牛牛
	for (BYTE i=0;i<cbCardCount-1;i++)
	{
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			if((bSum-bTemp[i]-bTemp[j])%10==0)
			{
				BYTE bCount=0;
				for (BYTE k=0;k<cbCardCount;k++)
				{
					if(k!=i && k!=j)
					{
						cbCardData[bCount++] = bTempData[k];
					}
				}ASSERT(bCount==3);

				cbCardData[bCount++] = bTempData[i];
				cbCardData[bCount++] = bTempData[j];

				return true;
			}
		}
	}

	return false;
}

//获取整数
bool CGameLogic::IsIntValue(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE sum=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		sum+=GetCardLogicValue(cbCardData[i]);
	}
	ASSERT(sum>0);
	return (sum%10==0);
}

//排列扑克
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//转换数值
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardValue(cbCardData[i]);	

	//排序操作
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//交换位置
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount , bool bJoke)
{
	//CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);
	//混乱准备
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	BYTE cbCount = bJoke?CARD_COUNT:(CARD_COUNT-2);
	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(cbCount-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[cbCount-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//转换数值
	return (bCardValue>10)?(10):bCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX)
{
	if(FirstOX!=NextOX)return (FirstOX>NextOX);

	//比较牛大小
	if(FirstOX==TRUE)
	{
		//获取点数
		BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
		BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

		//点数判断
		if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);
	}

	//排序大小
	BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
	CopyMemory(bNextTemp,cbNextData,cbCardCount);
	SortCardList(bFirstTemp,cbCardCount);
	SortCardList(bNextTemp,cbCardCount);

	//比较数值
	BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
	if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;

	//比较颜色
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);

	return false;
}

WORD CGameLogic::GetWinScore( BYTE cbPlayStatus[GAME_PLAYER] ,BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT] , LONGLONG lTableScore , LONGLONG *lWinScore , WORD wRevenue , LONGLONG *lUserRevenue/*=NULL*/ )
{
	WORD wWinTimes=0;
	/*ZeroMemory(wWinCount,sizeof(wWinCount));*/

	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,cbHandCardData,sizeof(cbUserCardData));

	WORD wWinPlayer = 0;
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if(cbPlayStatus[i] == FALSE)continue;
		wWinPlayer = i;
		break;
	}
	//对比玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(cbPlayStatus[i] == FALSE)continue;

		//对比扑克
		if (RatioCompareCard(cbUserCardData[i],cbUserCardData[wWinPlayer],MAX_COUNT,TRUE,TRUE) )
		{
			wWinPlayer = i;
		}
	}
	//获得倍数
	wWinTimes = GetTimes(cbUserCardData[wWinPlayer], MAX_COUNT);


	//统计得分
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(cbPlayStatus[i] == FALSE)continue;
		if(i==wWinPlayer)
		{
			continue;/*lWinScore[i]=lTableScore*wWinTimes;*/
		}
		else
		{
			lWinScore[i]= -lTableScore*wWinTimes;
		}
	}
	LONGLONG lWinerScore =0;;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(cbPlayStatus[i] == FALSE || i==wWinPlayer)continue;
		lWinerScore+= (-lWinScore[i]);
	}

	lWinScore[wWinPlayer]=lWinerScore;

	//扣税
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(lWinScore[i] > 0)
		{
			if(lUserRevenue != NULL)
			{
				lUserRevenue[i] = lWinScore[i]*wRevenue/100;
			}
			lWinScore[i] -= lWinScore[i]*wRevenue/100;
		}
	}

	return wWinPlayer;
}

void CGameLogic::MathCardList( BYTE cbCardBuffer[], BYTE cbBufferCount , BYTE cbMathCard1[] , BYTE cbIndex1, BYTE cbMathCard2[] ,BYTE cbIndex2)
{
	BYTE cbMachCount=0;
	BYTE cbCount = (cbMathCard2==NULL)?MAX_COUNT:2*MAX_COUNT;

	for (; cbMachCount<cbCount ;)
	{
		//混乱准备
		BYTE cbCardData[CARD_COUNT];
		CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

		//混乱扑克
		BYTE bRandCount=0,bPosition=0;
		BOOL bHas=false;
		do
		{
			bHas=false;
			bPosition=rand()%(CARD_COUNT-bRandCount);

			for(int i=0 ; i<cbBufferCount; i++)
			{
				if(cbMachCount < MAX_COUNT)
				{
					if(i>=(cbIndex1*MAX_COUNT+cbMachCount) && i<(cbIndex1*MAX_COUNT+MAX_COUNT)) continue;
					if(i>=(cbIndex2*MAX_COUNT) && i<(cbIndex2*MAX_COUNT+MAX_COUNT)) continue;
				}
				else
				{
					if(i>=(cbIndex2*MAX_COUNT+cbMachCount%MAX_COUNT) && i<(cbIndex2*MAX_COUNT+MAX_COUNT)) continue;
				}
				if(cbCardBuffer[i] == cbCardData[bPosition])
				{
					bHas=true;
					break;
				}
			}

			if(!bHas)
			{
				if(cbMachCount < MAX_COUNT)
				{
					if(GetCardValue(cbMathCard1[cbMachCount]) == GetCardValue(cbCardData[bPosition]))
					{
						cbCardBuffer[cbIndex1*MAX_COUNT+cbMachCount] = cbCardData[bPosition];
						cbMachCount++;
					}
				}
				else
				{
					if(GetCardValue(cbMathCard2[cbMachCount%MAX_COUNT]) == GetCardValue(cbCardData[bPosition]))
					{
						cbCardBuffer[cbIndex2*MAX_COUNT+cbMachCount%MAX_COUNT] = cbCardData[bPosition];
						cbMachCount++;
					}
				}
			}
			bRandCount++;
			cbCardData[bPosition]=cbCardData[CARD_COUNT-bRandCount];

		} while ((bRandCount<CARD_COUNT)&&(cbMachCount<cbCount));


	}

}

void CGameLogic::RandMathCard( BYTE cbCardBuffer[], BYTE cbBufferCount , BYTE cbMathCard1[] , BYTE cbIndex1, BYTE cbMathCard2[] ,BYTE cbIndex2 )
{
	BYTE cbType1 = GetCardType(cbMathCard1 , MAX_COUNT);
	MakeCard(cbMathCard1 , cbType1);
	CopyMemory(cbCardBuffer+cbIndex1*MAX_COUNT , cbMathCard1 , sizeof(BYTE)*MAX_COUNT);
	while (!ValidCardValue(cbCardBuffer , cbBufferCount))
	{
		MakeCard(cbMathCard1 , cbType1);
		CopyMemory(cbCardBuffer+cbIndex1*MAX_COUNT , cbMathCard1 , sizeof(BYTE)*MAX_COUNT);
	}
	if(cbMathCard2 != NULL)
	{
		BYTE cbType2 = GetCardType(cbMathCard2 , MAX_COUNT);
		MakeCard(cbMathCard2 , cbType2);
		CopyMemory(cbCardBuffer+cbIndex2*MAX_COUNT , cbMathCard2 , sizeof(BYTE)*MAX_COUNT);
		while (!ValidCardValue(cbCardBuffer , cbBufferCount))
		{
			MakeCard(cbMathCard2 , cbType2);
			CopyMemory(cbCardBuffer+cbIndex2*MAX_COUNT , cbMathCard2 , sizeof(BYTE)*MAX_COUNT);
		}
	}
}

void CGameLogic::MakeCard( BYTE cbMathCard1[] , BYTE cbType )
{
	BYTE cbOxCard[3] = {10,20,30};
	BYTE cbOx3Card = cbOxCard[rand()%3];
	BYTE cbOx2Card = cbOxCard[rand()%2];
	if(cbType == OX_DOUBLE_JOKE)
	{
		BYTE cbJokeIndex[2]={0};
		cbJokeIndex[0] = rand()%MAX_COUNT;
		cbJokeIndex[1] = (cbJokeIndex[0]+1+rand()%(MAX_COUNT-1))%MAX_COUNT;
		for (INT i=0 ; i<MAX_COUNT ; i++)
		{
			if(i==cbJokeIndex[0])
			{
				cbMathCard1[i] = 0xE;
			}
			else if(i==cbJokeIndex[1])
			{
				cbMathCard1[i] = 0xF;
			}
			else
			{
				cbMathCard1[i] = rand()%3+11;
			}
		}
	}
	else if(cbType == OX_GOLD)
	{
		BYTE cbJokeIndex=rand()%MAX_COUNT;
		for (INT i=0 ; i<MAX_COUNT ; i++)
		{
			if(i==cbJokeIndex)
			{
				cbMathCard1[i] = (rand()%2)?0xE:0xF;
			}
			else
			{
				cbMathCard1[i] = rand()%3+11;
			}
		}
	}
	else if(cbType > 0)
	{
		BYTE cbCard1 = rand()%(cbOx3Card%0xD+1)+1;
		BYTE cbCard2 = rand()%((cbOx3Card-GetCardLogicValue(cbCard1)+10)%0xD+1)+1;
		BYTE v = 30-GetCardLogicValue(cbCard1)-GetCardLogicValue(cbCard2);
		BYTE cbCard3 = (v%10==0)?(10+rand()%4):v%10;
		BYTE cbCard4 = rand()%0xD+1;
		BYTE v2 = 10-GetCardLogicValue(cbCard4);
		BYTE cbCard5 = (v2+cbType)>10?(cbType-GetCardLogicValue(cbCard4)):(v2+cbType);
		if(cbCard5 == 10)
		{
			cbCard5 = 10+rand()%4;
		}
		cbMathCard1[0] = cbCard1;
		cbMathCard1[1] = cbCard2;
		cbMathCard1[2] = cbCard3;
		cbMathCard1[3] = cbCard4;
		cbMathCard1[4] = cbCard5;
	}
	else
	{
		cbMathCard1[0]=10;
		cbMathCard1[1]=11;
		cbMathCard1[2]=12;
		while (GetCardType(cbMathCard1 , MAX_COUNT) != OX_VALUE0)
		{
			BYTE cbCard1 = rand()%(cbOx3Card%0xD+1)+1;
			BYTE cbCard2 = rand()%((cbOx3Card-cbCard1+10)%0xD+1)+1;
			BYTE cbCard3 = GetCardLogicValue(cbOx3Card-GetCardLogicValue(cbCard1)-GetCardLogicValue(cbCard2))+rand()%3+1;
			BYTE cbCard4 = GetCardLogicValue(20-GetCardLogicValue(cbCard2)-GetCardLogicValue(cbCard3))+rand()%3+1;
			BYTE cbCard5 = GetCardLogicValue(20-GetCardLogicValue(cbCard3)-GetCardLogicValue(cbCard4))+rand()%3+1;
			cbMathCard1[0] = cbCard1;
			cbMathCard1[1] = cbCard2;
			cbMathCard1[2] = cbCard3;
			cbMathCard1[3] = cbCard4;
			cbMathCard1[4] = cbCard5;
		}
	}
}

BOOL CGameLogic::ValidCardValue( BYTE cbCardBuffer[] , BYTE cbBufferCount )
{
	BYTE cbTemp[GAME_PLAYER*MAX_COUNT]={0};
	CopyMemory(cbTemp , cbCardBuffer , cbBufferCount);
	for (int i=0 ; i<GAME_PLAYER*MAX_COUNT; i++)
	{
		if(cbTemp[i] != 0)
		{
			cbTemp[i] = GetCardValue(cbTemp[i]);
		}		
	}
	sort(cbTemp , cbTemp+GAME_PLAYER*MAX_COUNT);
	BYTE cbCount=1;
	for (int k=0 ; k<GAME_PLAYER*MAX_COUNT-1; k++)
	{
		if(cbTemp[k] == 0) continue;
		if(cbTemp[k] != cbTemp[k+1])
		{
			cbCount = 1;
		}
		else
		{
			if(cbTemp[k] == 0xE || cbTemp[k] == 0xF)
			{
				return false;
			}
			cbCount++;
		}
		if(cbCount > 4)
		{
			return false;
		}
	}
	return true;
}
//手牌中是否有王
int CGameLogic::IsHaveJoker( BYTE cbCardData[], BYTE cbCardCount )
{
	BYTE tempCardData[MAX_COUNT];
	CopyMemory(tempCardData, cbCardData, sizeof(tempCardData));
	int iJokerCount =0;
	for(int i=0; i<MAX_COUNT; i++)
	{
		if (GetCardValue(tempCardData[i])>=14)
		{
			iJokerCount ++;
		}
	}
	return iJokerCount;
}
//通比牛牛中获取类型（有大小王的时候获取最大牌型）
BYTE CGameLogic::GetBestCardType( BYTE cbCardData[], BYTE cbCardCount, BYTE cbOutCard[2]/*=NULL*/, bool bGet/*=false*/ )
{
	int iJokerCount = IsHaveJoker(cbCardData, cbCardCount);
	if (iJokerCount== 2)
	{	
		if (bGet)
		{
			int j=0;
			for (BYTE i=0;i<cbCardCount;i++)
			{
				if (GetCardValue(cbCardData[i])>=14 )
				{
					cbOutCard[j] = cbCardData[i];
					j++;
					break;
				}
			}
			for (BYTE i=0;i<cbCardCount;i++)
			{
				if (GetCardValue(cbCardData[i])<14 )
				{
					cbOutCard[j] =cbCardData[i];
					j++;
					break;
				}
			}
		}

		for (int i=0; i<MAX_COUNT;i++)
		{
			if (GetCardValue(cbCardData[i])>=14 )continue;
			if (GetCardValue(cbCardData[i])<11) return 10;
		}
		return OX_DOUBLE_JOKE;
	}
	else if (iJokerCount == 1)
	{
		BYTE bTemp[MAX_COUNT-1];
		BYTE cbTecpNoJokerData[MAX_COUNT-1];
		BYTE bSum=0;
		BYTE iNiu=0;

		for (BYTE i=0,j=0;i<cbCardCount;i++)
		{
			if (GetCardValue(cbCardData[i])>=14 )
			{
				if (bGet)
				{
					cbOutCard[0] = cbCardData[i];
				}
				continue;
			}
			cbTecpNoJokerData[j] = cbCardData[i];
			j++;
		}
		
		for (BYTE i=0;i<MAX_COUNT-1;i++)
		{
			bTemp[i]=GetCardLogicValue(cbTecpNoJokerData[i]);
			bSum+=bTemp[i];
		}

		for (BYTE i=0; i<MAX_COUNT-1; i++)
		{
			if ((bSum-bTemp[i])%10 == 0)
			{
				if (bGet)
				{
					cbOutCard[1] = cbTecpNoJokerData[i];
				}
				return 10;
			}
		}

		for (BYTE i=0;i<MAX_COUNT-2;i++)
		{
			for (BYTE j=i+1;j<MAX_COUNT-1;j++)
			{
				int temp = ((bTemp[i]+bTemp[j])>10)?(bTemp[i]+bTemp[j]-10):(bTemp[i]+bTemp[j]);
				if (temp>iNiu)
				{
					iNiu = temp;
					if (bGet)
					{
						cbOutCard[0] =  cbTecpNoJokerData[i];
						cbOutCard[1] =  cbTecpNoJokerData[j];
					}
				}
			}
		}
		return iNiu;
	}
	else
	{
		return GetCardType(cbCardData, cbCardCount,cbOutCard,bGet);
	}
}



//对比扑克（通比牛牛）
bool CGameLogic::RatioCompareCard( BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX )
{
	if(FirstOX!=NextOX)return (FirstOX>NextOX);
	//比较牛大小
	if(FirstOX==TRUE)
	{
		//获取点数
		BYTE cbNextType=GetBestCardType(cbNextData,cbCardCount);
		BYTE cbFirstType=GetBestCardType(cbFirstData,cbCardCount);

		//点数判断
		if (cbFirstType!=cbNextType) return (cbFirstType>cbNextType);
	}

	if (IsHaveJoker(cbFirstData, cbCardCount)&& !IsHaveJoker(cbNextData, cbCardCount))
	{
		return false;
	}
	else if(!IsHaveJoker(cbFirstData, cbCardCount)&& IsHaveJoker(cbNextData, cbCardCount))
	{
		return true;
	}
	else if(IsHaveJoker(cbFirstData, cbCardCount)&& IsHaveJoker(cbNextData, cbCardCount))
	{
		//排序大小
		BYTE bFirstTemp[MAX_COUNT-1],bNextTemp[MAX_COUNT-1];
		for (BYTE i=0,j=0;i<cbCardCount;i++)
		{
			if (GetCardValue(cbFirstData[i])>=14 )
			{
				continue;
			}
			bFirstTemp[j] = cbFirstData[i];
			j++;
		}

		for (BYTE i=0,j=0;i<cbCardCount;i++)
		{
			if (GetCardValue(cbNextData[i])>=14 )
			{
				continue;
			}
			bNextTemp[j] = cbNextData[i];
			j++;
		}

		SortCardList(bFirstTemp,MAX_COUNT-1);
		SortCardList(bNextTemp,MAX_COUNT-1);

		//比较数值
		BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
		BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
		if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;

		//比较颜色
		return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);
	}

	//排序大小
	BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
	CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
	CopyMemory(bNextTemp,cbNextData,cbCardCount);
	SortCardList(bFirstTemp,cbCardCount);
	SortCardList(bNextTemp,cbCardCount);

	//比较数值
	BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
	BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);
	if(cbNextMaxValue!=cbFirstMaxValue)return cbFirstMaxValue>cbNextMaxValue;

	//比较颜色
	return GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0]);
}

void CGameLogic::GetIndexOx( int iIndex[] )
{
	
}

WORD CGameLogic::GetWinUser( BYTE cbPlayStatus[GAME_PLAYER] ,BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT] )
{

	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,cbHandCardData,sizeof(cbUserCardData));
	WORD wWinPlayer = 0;
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		if(cbPlayStatus[i] == FALSE)continue;
		wWinPlayer = i;
		break;
	}
	//对比玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(cbPlayStatus[i] == FALSE)continue;

		//对比扑克
		if (RatioCompareCard(cbUserCardData[i],cbUserCardData[wWinPlayer],MAX_COUNT,TRUE,TRUE) )
		{
			wWinPlayer = i;
		}
	}
	return wWinPlayer;
}

int CGameLogic::GetSameCount( BYTE CardData1[], BYTE CardData2[], BYTE cbCount /*= 5*/ )
{
	int iCount = 0;
	for (int i=0; i<cbCount; i++)
	{
		for (int j=0; j<cbCount; j++)
		{
			if(CardData1[i] == CardData2[j]) iCount++;
		}
	}
	return iCount;
}



//////////////////////////////////////////////////////////////////////////
