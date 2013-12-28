#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "stdafx.h"
//#include "..\..\模板库\Template.h"
//////////////////////////////////////////////////////////////////////////
//宏定义

#define MAX_COUNT					5									//最大数目

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//扑克类型
#define OX_VALUE0					0									//混合牌型
#define OX_GOLD						11									//金牛
#define OX_DOUBLE_JOKE				12									//双王牛

#define CARD_COUNT					54

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE 							cbFourLogicVolue[1];				//四张列表
	BYTE 							cbThreeLogicVolue[1];				//三张列表
	BYTE 							cbDoubleLogicVolue[2];				//两张列表
	BYTE 							cbSignedLogicVolue[5];				//单张列表
	BYTE							cbFourCardData[MAX_COUNT];			//四张列表
	BYTE							cbThreeCardData[MAX_COUNT];			//三张列表
	BYTE							cbDoubleCardData[MAX_COUNT];		//两张列表
	BYTE							cbSignedCardData[MAX_COUNT];		//单张数目
};

//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
private:
	static BYTE						m_cbCardListData[CARD_COUNT];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSendCard[]=NULL, bool bGet=false);
	//获得玩家给定牌序列的牌型
	BYTE GetListCardType(BYTE cbCardData[], BYTE cbCardCount);
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	//获取倍数
	BYTE GetTimes(BYTE cbCardData[], BYTE cbCardCount);
	//获取牛牛
	bool GetOxCard(BYTE cbCardData[], BYTE cbCardCount);
	//获取整数
	bool IsIntValue(BYTE cbCardData[], BYTE cbCardCount);
	//手牌中是否有王
	int IsHaveJoker(BYTE cbCardData[], BYTE cbCardCount);
	//通比牛牛中获取类型（有大小王的时候获取最大牌型）
	BYTE GetBestCardType(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSendCard[2]=NULL, bool bGet=false);
	//获得有牛牌型的两张牌索引
	void GetIndexOx(int iIndex[]);
	//两个牌组中相同牌
	int GetSameCount(BYTE CardData1[], BYTE CardData2[], BYTE cbCount = 5);
	
	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//混乱扑克
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount , bool bJoke=true);

	void MathCardList( BYTE cbCardBuffer[], BYTE cbBufferCount , BYTE cbMathCard1[] , BYTE cbIndex1, BYTE cbMathCard2[]=NULL ,BYTE cbIndex2=INVALID_BYTE);

	void RandMathCard( BYTE cbCardBuffer[], BYTE cbBufferCount , BYTE cbMathCard1[] , BYTE cbIndex1, BYTE cbMathCard2[]=NULL ,BYTE cbIndex2=INVALID_BYTE);

	void MakeCard(BYTE cbMathCard1[] , BYTE cbType);

	BOOL ValidCardValue(BYTE cbCardBuffer[] , BYTE cbBufferCount);


	//功能函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE cbCardData);
	//对比扑克（通比牛牛）
	bool RatioCompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX);
	//对比扑克
	bool CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount,BOOL FirstOX,BOOL NextOX);
	//赢分情况
	WORD GetWinScore(BYTE cbPlayStatus[GAME_PLAYER] ,BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT] , LONGLONG lTableScore , LONGLONG *lWinScore , WORD wRevenue , LONGLONG *lUserRevenue=NULL);
	//获得赢家座位号
	WORD GetWinUser(BYTE cbPlayStatus[GAME_PLAYER] ,BYTE cbHandCardData[GAME_PLAYER][MAX_COUNT]);

};

//////////////////////////////////////////////////////////////////////////

#endif
