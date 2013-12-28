#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define IDR_MAINFRAME				128
#define KIND_ID						5									//游戏 I D
#define GAME_PLAYER					5									//游戏人数
#define GAME_NAME					TEXT("通比牛牛")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//游戏类型
#define MAXCOUNT					5									//扑克数目
#define MAXMULITY					2									//最大赔率
#define MAX_CHIP_COUNT				5									//兑换筹码的档次

#define BIG_BUFFER						0									//大累计
#define SMALL_BUFFER					1									//小累计
#define BUFFER_COUNT					2									//聚宝盆数量
//游戏状态
#define GS_OX_FREE					GS_FREE								//空闲状态
#define GS_OX_PLAY					(GS_PLAYING+1)						//游戏状态
#define GS_OX_ACCOUNTS				(GS_PLAYING+2)						//结算摊牌状态



// 命令定义
// #define SUB_S_SENCE_FREE			109									//场景消息(空闲)
// #define SUB_S_SENCE_PLAY			110									//场景消息(叫分)
// #define SUB_S_SENCE_END				111									//场景消息(游戏)



//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			1000									//游戏开始
#define SUB_S_GAME_READY			1001									//玩家准备
#define SUB_S_SEND_CARD				1002									//发送扑克
#define SUB_S_EX_CHIP				1003									//兑换筹码完成
#define	SUB_S_BOWL					1004									//聚宝盆
#define SUB_S_OPEN_CARD				1005									//用户摊牌
#define SUB_S_GAME_END				1006
#define SUB_S_GAME_FINISH			1007									//游戏全部结束
#define SUB_S_HURRY_CARD			1008									//催牌
#define SUB_S_GAME_FREE				1009									//游戏空闲
#define SUB_S_XIJIN					1010									//喜金
#define	SUB_S_SEND_BOWL				1011									//聚宝盆信息
#define SUB_S_OFFLINE				1012									//断线消息
#define SUB_S_RECONN				1013									//重连

//断线/重连
struct CMD_S_OffLine_Reconn
{
	WORD								wOperaUser;
};

//聚宝盆/喜金
struct CMD_S_Send_Bowl
{
	DWORD								dwUserID;							//用户 I D
	LONG								lIngot;								//元宝奖励
	BYTE								cbBowlType;							//聚宝盆类型

	LONG								lNewIngot;							//新聚宝盆奖励
};
struct CMD_S_HurryCard
{
	WORD								wCurrentUser;
	BYTE								cbWords;
};
//游戏准备
struct CMD_S_GameReady
{
	BYTE								cbTimeLeave;						//剩余时间
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
};
//游戏状态
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//基础积分
	WORD								wServerType;						//房间类型
	LONGLONG							lMinBet;							//最小下注
	WORD								wServerID;							//房间ID
	LONGLONG							lChipNum[MAX_CHIP_COUNT];			//筹码数量
	LONG								lPercentage[MAX_CHIP_COUNT];		//服务费比例
	
	/*LONGLONG							lDescription[GAME_PLAYER];			//称号积分*/
};

//游戏状态
struct CMD_S_StatusPlay
{

	LONGLONG							lCellScore;							//基础积分
	WORD								wServerType;						//房间类型
	LONGLONG							lMinBet;							//最小下注
	WORD								wServerID;							//房间ID
	LONGLONG							lChipNum[MAX_CHIP_COUNT];			//筹码数量
	LONG								lPercentage[MAX_CHIP_COUNT];		//服务费比例
	
	BYTE								cbOx[GAME_PLAYER];					//摊牌信息
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];			//扑克信息
	BYTE								cbRewardCardData[GAME_PLAYER][MAXCOUNT];	//喜金扑克信息
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态

};
//结算状态
struct CMD_S_StatusEnd
{

	LONGLONG							lCellScore;							//基础积分
	WORD								wServerType;						//房间类型
	LONGLONG							lMinBet;							//最小下注
	WORD								wServerID;							//房间ID
	LONGLONG							lChipNum[MAX_CHIP_COUNT];			//筹码数量
	LONG								lPercentage[MAX_CHIP_COUNT];		//服务费比例

	
	//BYTE								cbOx[GAME_PLAYER];					//摊牌信息
	LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
	BYTE								cbRewardCardData[GAME_PLAYER][MAXCOUNT];//喜金扑克信息
	LONG								lDeskPay;							//扣掉的桌费
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态

};

//游戏开始
struct CMD_S_GameStart
{
	//LONG								lAddScore;							//底注下注
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
	BYTE								cbRewardCardData[GAME_PLAYER][MAXCOUNT];//喜金牌
	BYTE								cbPlayStatus[GAME_PLAYER];			//游戏状态
};

//发送扑克
struct CMD_S_SendCard
{
	
};

//用户摊牌
struct CMD_S_Open_Card
{
	WORD								wPlayerID;							//摊牌用户
	BYTE								bOpen;								//摊牌标志
};

//操作命令
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//操作用户
	WORD							wProvideUser;						//供应用户
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
};
//聚宝盆信息
struct CMD_S_Bowl
{
	LONGLONG						lBowlIngot;							//聚宝盆
	//LONG							lSmallBowlIngot;
	LONG							lLikeGoldIngot;						//喜金
	LONG							lNewLikeIngot[4];					//新喜金发放数目
};

//游戏结束
struct CMD_S_GameEnd
{
	LONG								lGameTax[GAME_PLAYER];				//游戏税收
	LONGLONG							lGameScore[GAME_PLAYER];			//游戏得分
	BYTE								cbCardData[GAME_PLAYER][MAXCOUNT];	//用户扑克
	LONG								lDeskPay;							//扣掉的桌费
};
//用户托管
struct CMD_S_Trustee
{
	bool							bTrustee;							//是否托管
	WORD							wChairID;							//托管用户
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_EX_CHIP				1									//兑换筹码
#define SUB_C_OPEN_CARD				2									//用户摊牌
#define SUB_C_COMPARE_CARD			3									//比牌完成
#define SUB_C_CHEAT					4									//配牌
#define SUB_C_HURRY_CARD			5									//催牌
//催牌
struct CMD_C_HurryCard
{
	BYTE								cbWords;
};

struct CMD_C_Cheat
{
	int								iCheatControl;						//是否进行控制
	BYTE							cbHandCardData[GAME_PLAYER][MAXCOUNT];	
	BYTE							cbRewardCard[GAME_PLAYER][MAXCOUNT];
	LONGLONG						lPoolScore;
};

//用户摊牌
struct CMD_C_OxCard
{
	BYTE								bOX;								//牛牛标志
};

//用户兑换筹码
struct CMD_C_ExchangeChip
{
	WORD							wExchangeUserChairID;				//兑换用户
	LONGLONG						lExchangeCount;						//兑换数目
};

//操作命令
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//操作代码
	BYTE							cbOperateCard;						//操作扑克
};
//用户托管
struct CMD_C_Trustee
{
	bool							bTrustee;							//是否托管	
};
//////////////////////////////////////////////////////////////////////////

#endif