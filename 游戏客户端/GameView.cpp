#include "StdAfx.h"
#include "GameView.h"
#include "DDImage.h"
#include "GameClient.h"
#include "ButtonWidget.h"
#include "Timer.h"
#include "GameTimer.h"
#include "SysFont.h"
#include "Common.h"
#include "FlashWidget.h"
#include "GameClientDlg.h"
#include "MusicInterface.h"
#include "SoundPlayer.h"
#include "Res.h"
#include "TopBullWidget.h"
#include "SexyMatrix.h"
#include "GameLogic.cpp"



#define						SHOW_CHAT_MASSAGE						WM_USER+200
//定时器
#define						IDI_CARD_APPEAR_TIMER					2000		//发牌定时器		

#define						IDI_START_TIMER							2002	
#define						IDI_READY_TIMER							2003
#define						IDI_CHIP_ANI_TIMER						2004
#define						IDI_OPEN_CARD							2005
#define						IDI_COMPARE_CARD_FINISH					2006
#define						IDI_TIMER_BTN_HURRY						2007		//催牌按钮
#define						IDI_CARD_DISAPPEAR_TIMER				2008		//牌消失动画时钟
#define						IDI_SHOW_HURRY_TAG						2009		//催牌标志
#define						IDI_AUTO_CHANGE_DESK					2010		//自动换桌
#define						IDI_NOT_ENOUGH_CHIP						2011		//筹码不足
	
#define						IDI_TIMER_PLAYER_STAU_0					2012
#define						IDI_TIMER_PLAYER_STAU_1					2013
#define						IDI_TIMER_PLAYER_STAU_2					2014
#define						IDI_TIMER_PLAYER_STAU_3					2015
#define						IDI_TIMER_PLAYER_STAU_4					2016

#define						IDI_TIMER_BTN_UP						2017
#define						IDI_TIMER_BTN_DOWN						2018

#define						IDI_REWARDCARD_UP						2019
#define						IDI_REWARDCARD_DOWN						2020


//定时器时间
#define						TIME_READY								10	
#define						TIME_START								5


#define						LIST_UCARD_ID							20000	//催牌
//坐标
//准备
CPoint cpReady[MAX_COUNT] = {
	CPoint(305,352),CPoint(109,480),CPoint(458,576),CPoint(804,480),CPoint(603,362)
};

CPoint cpAddScore[GAME_PLAYER] ={
	CPoint(445,411),CPoint(324,472),CPoint(512,520),CPoint(698,472),CPoint(572,411)
};

CPoint cpCardType[GAME_PLAYER] = {
	CPoint(306,341),CPoint(134,480),CPoint(482,610),CPoint(830,455),CPoint(617,333)
};


CPoint cpExChangeChip[MAX_CHIP_COUNT] ={
	CPoint(253,421),CPoint(375,421),CPoint(496,421),CPoint(616,421),CPoint(735,421)
};

CPoint cpOpenCardTag[GAME_PLAYER] ={
	CPoint(320,350),CPoint(145,465),CPoint(480,630),CPoint(815,460),CPoint(610,345)
};

CPoint cpMoneyAni[GAME_PLAYER] = {
	CPoint(430,292),CPoint(180,362),CPoint(295,520),CPoint(810,488),CPoint(700+20,305)
};

CPoint cpOffLine[GAME_PLAYER] ={
	CPoint(282,305),CPoint(3,415),CPoint(0,0),CPoint(913,412),CPoint(688,308),
};

//聚宝盆底图坐标
CPoint cpIngot(333,0);
//喊话框坐标
CPoint cpHurryCard[GAME_PLAYER] = {CPoint(380,160), CPoint(90,340), CPoint(250,560),CPoint(820,340),CPoint(810,160)};

CPoint cpUcCardPosition(762,488);
//----动画回调
enum 
{
	ANI_DICE_OVER_CALLBACK,
	ANI_CARD_APPEAR,
	ANI_NORMAL = 33,
};

CPoint cpTuoGuanUp(440,684);
CPoint cpTuoGuanDown(440,716);
CPoint cpLikeIngot(299,0);

static PTCHAR   szOrder[]={TEXT("普通会员"),TEXT("一级贵宾"),TEXT("二级贵宾"),TEXT("三级贵宾"),TEXT("四级贵宾"),TEXT("五级贵宾"),TEXT("六级贵宾"),
TEXT("七级贵宾"),TEXT("八级贵宾"),TEXT("九级贵宾"),TEXT("白银贵宾"),TEXT("黄金贵宾"),TEXT("钻石贵宾"),TEXT("顶级贵宾")
};

//称号
static PTCHAR   szDescription[]={TEXT("牛犊子"),TEXT("幼牛"),TEXT("小牛"),TEXT("野牛"),TEXT("壮牛"),TEXT("蛮牛"),TEXT("斗牛"),
TEXT("铁牛"),TEXT("血牛"),TEXT("银牛"),TEXT("金牛"),TEXT("牛魔王"),TEXT("百战神牛")
};

static PTCHAR szXiJin = TEXT("喜金:牛牛牌型即可得30元宝奖励");
static Sexy::Color cDesColor[] = {
	Sexy::Color(255,255,255),Sexy::Color(255,255,0),Sexy::Color(0,255,255),Sexy::Color(128,255,255)
};

#define		ABS(a)			((a)<0?(-(a)):(a))


const char* vn(BYTE nType)
{	
	switch(nType&0x0F)
	{
	case 1:
		return "A";
	case 2:
		return "2";
	case 3:
		return "3";
	case 4:
		return "4";
	case 5:
		return "5";
	case 6:
		return "6";
	case 7:
		return "7";
	case 8:
		return "8";
	case 9:
		return "9";
	case 10:
		return "10";
	case 11:
		return "J";
	case 12:
		return "Q";
	case 13:
		return "K";
	case 14:
		return "小王";
	case 15:
		return "大王";
	}
	return "未知牌";
}

const char* cn(BYTE nType)
{
	switch(nType&0xF0)
	{
	case 0x00:
		return "方块";
	case 0x10:
		return "梅花";
	case 0x20:
		return "红桃";
	case 0x30:
		return "黑桃";
	case 0x40:
		return "";
	}
	return "未知颜色";
}




const int GetVaule(BYTE cCard)
{
	return (cCard & 0x0F)-1;
}

const int GetType(BYTE cCard)
{
	return cCard >> 4;
}

using namespace Sexy;

CGameView::CGameView(void)
{
	m_pGameApp = (CGameClientApp *)AfxGetApp();
	m_pGameDlg = (CGameClientDlg *)m_pGameApp->m_pGameClientDlg;
	
	//初始化字体
	m_pFont10WeiRuanYaHei = new Sexy::SysFont("微软雅黑",10);
	m_pFont9WeiRuanYaHei = new Sexy::SysFont("微软雅黑",9);
	m_pFont9HeiTi =  new Sexy::SysFont("黑体",12);
 	m_pResultNum = new CResultNum;
	//初始化变量
	m_wMeChairID = 0;
	m_lBowl = 0;
	m_lLikeIngot = 0;
	m_bGetMsg=true;
	m_wTimeNum = 0;
	m_lCellScore = 0;						//单位积分
	m_lMinbet =0;							//最小下注
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lChipNum,sizeof(m_lChipNum));
	ZeroMemory(m_lPercentage,sizeof(m_lPercentage));
	ZeroMemory(m_cbHandCard, sizeof(m_cbHandCard));
	ZeroMemory(m_cbRewardCard,sizeof(m_cbRewardCard));
	ZeroMemory(m_cbCardType, sizeof(m_cbCardType));
	ZeroMemory(m_iOtherInfoTrans,sizeof(m_iOtherInfoTrans));
	ZeroMemory(m_bMouseSelect, sizeof(m_bMouseSelect));
	ZeroMemory(m_lNewLikeIngot, sizeof(m_lNewLikeIngot));
	ZeroMemory(m_bShowSameTag, sizeof(m_bShowSameTag));
	m_wServerType =0;
	m_bShowExChangeChipDlg = false;
	m_bShowOxType = false;
	m_cpTimer.SetPoint(441,300);
	m_bShowUcCard = false;
	m_wHurryCardUser = INVALID_CHAIR;
	m_bTrustee = false;
	m_iTransBowlHint = 0;
	m_iTransLikeIngotHing = 0;
	m_bShowBowlHint = false;
	m_bShowLikeIngotHint = false;
	m_bShowRewardCard = false;
	m_iMyFaceIndex = 0;
	m_cpRewardCard.SetPoint(65,-21);

	for (int i=0; i<4; i++)
	{
		m_pLikeIngotImg[i] = new Sexy::DDImage();
	}

	for (int i=0; i<11; i++)
	{
		m_pOxAni[i] = NULL;
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_pPlayer[i] = NULL;
		m_bOpenCard[i] = false;
		
	}   
	m_bAddScore =false;
	

	for (int i=0; i<11; i++)
	{
		m_pOxAni[i] = new AnimLayer(m_pGameApp, this);
	}

	//背景
	m_pTimer = new Sexy::Timer(this);
	m_pGameTimer = new Sexy::GameTimer(this);
	m_pFont10Song = new Sexy::SysFont("宋体",10);
	m_pFont10Hei = new Sexy::SysFont("黑体",9);
	m_iOtherInfofPos[0][0] = 360;
	m_iOtherInfofPos[0][1] = 120;
	m_iOtherInfofPos[0][2] = 45;

	m_iOtherInfofPos[1][0] = 60;
	m_iOtherInfofPos[1][1] = 230;
	m_iOtherInfofPos[1][2] = 110;

	m_iOtherInfofPos[4][0] = 510;
	m_iOtherInfofPos[4][1] = 120;
	m_iOtherInfofPos[4][2] = 45;

	m_iOtherInfofPos[3][0] = 800;
	m_iOtherInfofPos[3][1] = 230;
	m_iOtherInfofPos[3][2] = 110;



	m_x = 0;
	m_y = 0;
	iAciton =0;
	m_index =0;
	iActor = 0;
	InitWidget();
	m_pChip.SetCallBack(this);
	m_GameCard.setCallBack(this);
	m_pTimer->SetTimer(IDI_CARD_APPEAR_TIMER, 20);
	m_pTimer->SetTimer(IDI_CHIP_ANI_TIMER, 20);
	m_pTimer->SetTimer(IDI_CARD_DISAPPEAR_TIMER, 20);

}

void CGameView::InitWidget()
{
	//催牌列表
	int listWidgetColors1[][4] = 
	{
		{57, 56, 0, 0},	   	//Background color//背景色
		{57, 56, 0, 0},    	//Outline color//边框色
		{217,188,146, 255},	/*{255, 253, 166},*/			//Plain text not hilited//文本色(未选择时)
		{255, 255, 255, 255},		//Text color when mouse is over text//文本色(鼠标经过)
		{135, 132, 27, 100},		//Color of the bar displayed when an item is selected//条目被选择后的颜色
		{0, 0, 0, 255}		//Color of the text if it has the bar behind it and mouse is not over it//文本色(被选择后).
	};

	Sexy::SysFont *tempfont= new Sexy::SysFont("宋体",9);

	m_pListUcCard = new Sexy::ListWidget(LIST_UCARD_ID, tempfont,this);
	m_pListUcCard->SetColors(listWidgetColors1, 6);
	AddWidget(m_pListUcCard);
	m_pListUcCard->Resize(cpUcCardPosition.x+19,cpUcCardPosition.y+5,233,185);
	m_pListUcCard->mItemHeight = 19;

	m_pListUcCard->AddLine(L"今天，就靠这把翻身了。",false);
	m_pListUcCard->AddLine(L"撑死胆大的，饿死胆小的！",false);
	m_pListUcCard->AddLine(L"风水轮流转，这局就是我。",false);
	m_pListUcCard->AddLine(L"要跟就跟，麻利点！",false);
	m_pListUcCard->AddLine(L"别磨蹭，快点押注。",false);
	m_pListUcCard->AddLine(L"速度，速度，快开牌。",false);
	m_pListUcCard->AddLine(L"想赚钱，下手就得又快又狠。",false);
	m_pListUcCard->AddLine(L"辛辛苦苦多少年，一把回到解放前！",false);
	m_pListUcCard->AddLine(L"运气太好了，把把是大牌！",false);
	m_pListUcCard->SetVisible(false);

	//兑换筹码按钮1
	m_pBtnExChip[0] = new Sexy::ButtonWidget(BT_EXCHANGECHIP_SPACE1, this);
	m_pBtnExChip[0]->SetBtnImage(m_pGameApp, IMG_BTN_BtnBuyChip);
	m_pBtnExChip[0]->Resize(206,495,IMG_BTN_BtnBuyChip->GetWidth()/4, IMG_BTN_BtnBuyChip->GetHeight());
	m_pBtnExChip[0]->SetVisible(false);
	AddWidget(m_pBtnExChip[0]);
	//兑换筹码按钮2
	m_pBtnExChip[1] = new Sexy::ButtonWidget(BT_EXCHANGECHIP_SPACE2, this);
	m_pBtnExChip[1]->SetBtnImage(m_pGameApp, IMG_BTN_BtnBuyChip);
	m_pBtnExChip[1]->Resize(327,495,IMG_BTN_BtnBuyChip->GetWidth()/4, IMG_BTN_BtnBuyChip->GetHeight());
	m_pBtnExChip[1]->SetVisible(false);
	AddWidget(m_pBtnExChip[1]);
	//兑换筹码按钮3
	m_pBtnExChip[2] = new Sexy::ButtonWidget(BT_EXCHANGECHIP_SPACE3, this);
	m_pBtnExChip[2]->SetBtnImage(m_pGameApp, IMG_BTN_BtnBuyChip);
	m_pBtnExChip[2]->Resize(448,495,IMG_BTN_BtnBuyChip->GetWidth()/4, IMG_BTN_BtnBuyChip->GetHeight());
	m_pBtnExChip[2]->SetVisible(false);
	AddWidget(m_pBtnExChip[2]);
	//兑换筹码按钮4
	m_pBtnExChip[3] = new Sexy::ButtonWidget(BT_EXCHANGECHIP_SPACE4, this);
	m_pBtnExChip[3]->SetBtnImage(m_pGameApp, IMG_BTN_BtnBuyChip);
	m_pBtnExChip[3]->Resize(568,495,IMG_BTN_BtnBuyChip->GetWidth()/4, IMG_BTN_BtnBuyChip->GetHeight());
	m_pBtnExChip[3]->SetVisible(false);
	AddWidget(m_pBtnExChip[3]);
	//兑换筹码按钮5
	m_pBtnExChip[4] = new Sexy::ButtonWidget(BT_EXCHANGECHIP_SPACE5, this);
	m_pBtnExChip[4]->SetBtnImage(m_pGameApp, IMG_BTN_BtnBuyChip);
	m_pBtnExChip[4]->Resize(687,495,IMG_BTN_BtnBuyChip->GetWidth()/4, IMG_BTN_BtnBuyChip->GetHeight());
	m_pBtnExChip[4]->SetVisible(false);
	AddWidget(m_pBtnExChip[4]);
	//快速喊话按钮
	m_pBtnQuickSub = new Sexy::ButtonWidget(BT_CUI_PAI, this);
	m_pBtnQuickSub->SetBtnImage(m_pGameApp, IMG_BTN_BtnQuickSub);
	m_pBtnQuickSub->Resize(817,678,IMG_BTN_BtnQuickSub->GetWidth()/4, IMG_BTN_BtnQuickSub->GetHeight());
	m_pBtnQuickSub->SetVisible(1);
	m_pBtnQuickSub->SetDisabled(true);
	AddWidget(m_pBtnQuickSub);
	//摊牌按钮
	m_pBtnOpenCard = new Sexy::ButtonWidget(BT_OPEN_CARD, this);
	m_pBtnOpenCard->SetBtnImage(m_pGameApp, IMG_BTN_BtnOpenCard);
	m_pBtnOpenCard->Resize(640,590,IMG_BTN_BtnOpenCard->GetWidth()/4, IMG_BTN_BtnOpenCard->GetHeight());
	m_pBtnOpenCard->SetVisible(0);
	AddWidget(m_pBtnOpenCard);


	//提示按钮
	m_pBtnHintCard = new Sexy::ButtonWidget(BT_HINT_CARD, this);
	m_pBtnHintCard->SetBtnImage(m_pGameApp, IMG_BTN_BtnHint);
	m_pBtnHintCard->Resize(640,540,IMG_BTN_BtnHint->GetWidth()/4, IMG_BTN_BtnHint->GetHeight());
	m_pBtnHintCard->SetVisible(0);
	AddWidget(m_pBtnHintCard);

	//重排按钮
	m_pBtnReListCard = new Sexy::ButtonWidget(BT_RELIST_CARD, this);
	m_pBtnReListCard->SetBtnImage(m_pGameApp, IMG_BTN_BtnReList);
	m_pBtnReListCard->Resize(640,540,IMG_BTN_BtnReList->GetWidth()/4, IMG_BTN_BtnReList->GetHeight());
	m_pBtnReListCard->SetVisible(0);
	AddWidget(m_pBtnReListCard);



	m_pPlayer[0] = new Player;
	m_pPlayer[0]->SetDirection(0);
	m_pPlayer[0]->SetPlayerSex();
	//m_pPlayer[0]->SetPlayerPos(34,-73);
	m_pPlayer[0]->SetStatus(ZHENG_CHANG);
 
	m_pPlayer[1] = new Player;
	m_pPlayer[1]->SetDirection(1); 
	m_pPlayer[1]->SetPlayerSex();
	//m_pPlayer[1]->SetPlayerPos(-77,-7);
	m_pPlayer[1]->SetStatus(ZHENG_CHANG);

	m_pPlayer[2] = new Player;
	m_pPlayer[2]->SetDirection(2); 
	m_pPlayer[2]->SetPlayerSex();
	m_pPlayer[2]->SetPlayerPos(166,535);
	m_pPlayer[2]->SetStatus(ZHENG_CHANG);

	m_pPlayer[3] = new Player;
	m_pPlayer[3]->SetDirection(3); 
	m_pPlayer[3]->SetPlayerSex();        
	//m_pPlayer[3]->SetPlayerPos(743,-12); 
	m_pPlayer[3]->SetStatus(ZHENG_CHANG);  
	m_pPlayer[3]->SetReversal(true);

	m_pPlayer[4] = new Player;
	m_pPlayer[4]->SetDirection(4); 
	m_pPlayer[4]->SetPlayerSex();
	//m_pPlayer[4]->SetPlayerPos(440,-71);       
	m_pPlayer[4]->SetStatus(ZHENG_CHANG);   
	
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_pAddMoneyAni[i] = NULL;
		m_pAddMoneyAni[i] = new AnimLayer(m_pGameApp, this);
		m_pAddMoneyAni[i]->Init(IMG_ANI_SubMoney,1);
		m_pAddMoneyAni[i]->SetLoopNum(1);
		m_pAddMoneyAni[i]->SetEndHide(true);
	}
	//自动配桌动画初始化
	m_pAutoDeskAni= NULL;
	m_pAutoDeskAni = new Sexy::AnimLayer(m_pGameApp, this);


	//等待其他玩家动画初始化
	m_pWaitOtherAni= NULL;
	m_pWaitOtherAni = new Sexy::AnimLayer(m_pGameApp, this);
	//喜金动画
	m_pLikeIngotAni = NULL;
	m_pLikeIngotAni = new Sexy::AnimLayer(m_pGameApp, this);

	//托管动画
	m_pTuoGuanAni = NULL;
	m_pTuoGuanAni = new Sexy::AnimLayer(m_pGameApp, this);

	//断线动画
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_pOffLineAni[i] = NULL;
		if(i==2)continue;
		m_pOffLineAni[i] = new Sexy::AnimLayer(m_pGameApp, this);
	}
	//聚宝盆动画
// 	m_pJackocur = NULL;
// 	m_pJackpotwin= NULL;
// 	m_pJackocur = new Sexy::AnimLayer(m_pGameApp, this);
// 	m_pJackpotwin = new Sexy::AnimLayer(m_pGameApp, this);


	
	m_pGameEnd = new GameEnd;
	m_pGameEnd->Resize(0,0,IMG_RATIOX_ShadowBG->GetWidth(),IMG_RATIOX_ShadowBG->GetHeight()+50);
	AddWidget(m_pGameEnd);
	m_pGameEnd->SetVisible(0);
	m_pGameEnd->SetGameView(this);
	
	//我的信息
	m_pMyBasicInfo = new CMyBasicInfo;
	m_pMyBasicInfo->Resize(0,554,214,180);
	AddWidget(m_pMyBasicInfo);
	m_pMyBasicInfo->SetVisible(1);
	m_pMyBasicInfo->SetGameView(this);
	//托管按钮
	m_pBtnTuoGuan = new Sexy::ButtonWidget(BT_TUOGUAN, this);
	m_pBtnTuoGuan->SetBtnImage(m_pGameApp, IMG_BTN_BtnTrustee);
	m_pBtnTuoGuan->Resize(440,cpTuoGuanDown.y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
	m_pBtnTuoGuan->SetDisabled(true);
	m_pBtnTuoGuan->SetVisible(1);
	AddWidget(m_pBtnTuoGuan);

	//取消托管按钮
	m_pBtnCancelTuoGuan = new Sexy::ButtonWidget(BT_CANCEL_TUOGUAN, this);
	m_pBtnCancelTuoGuan->SetBtnImage(m_pGameApp, IMG_BTN_BtnCancelTrustee);
	m_pBtnCancelTuoGuan->Resize(440,cpTuoGuanDown.y,IMG_BTN_BtnCancelTrustee->GetWidth()/4, IMG_BTN_BtnCancelTrustee->GetHeight());
	m_pBtnCancelTuoGuan->SetVisible(0);
	AddWidget(m_pBtnCancelTuoGuan);

	m_pTopBullWidget = new CTopBullWidget;
	m_pTopBullWidget->SetVisible(false);
	m_pTopBullWidget->Resize(0,0,1015, IMG_TOP_BULL_BG->GetHeight());
	AddWidget(m_pTopBullWidget);
	

	m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_0,2000);
	m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_1,3000);
	m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_2,4000);
	m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_3,5000);
	m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_4,6000);

	MakeLikeIngotImg();
}

CGameView::~CGameView(void)
{
	RemoveAllAni();
	RemoveAllWidgets(true);
	delete m_pFont10Hei;
	delete m_pFont10Song;
	SAFE_DELETE(m_pFont9WeiRuanYaHei);
	SAFE_DELETE(m_pFont9HeiTi);
	for (int i =0; i<4; i++)
	{
		SAFE_DELETE(m_pLikeIngotImg[i]);
	}
	/*SAFE_DELETE(m_pChip);*/
}

void CGameView::Draw( Sexy::Graphics* g )
{	
	if (m_pGameDlg->GetSafeHwnd() == NULL)
		return;
	// 背景
	DrawBg(g);
	//画喜金栏
	DrawReward(g);

	//printf("Draw(%d)\n", GetTickCount());
	//准备
	DrawReady(g);

	DrawTest(g);

	
	//画筹码
	m_pChip.Draw(g);
	//桌面筹码
	DrawDeskChip(g);
	//加注信息
	DrawAddScore(g);
	//牌
	m_GameCard.Draw(g);
	//画玩家
	DrawPlayer(g);
	//牌型
	DrawOxType(g);
	//催牌
	DrawUcCard(g);
	//别的玩家的信息
	DrawOtherInfo(g);
	//DrawOpenCard
	DrawOpenCard(g);
	//画聚宝盆
	//DrawBuffer(g);
	//兑换筹码
	DrawExChangeChipDlg(g);
	//画动画
	DrawAni(g);
	//喜金栏提示
	DrawRewardHint(g);

	DeferOverlay(0);
}

//画背景
void CGameView::DrawBg( Sexy::Graphics* g )
{
	g->DrawImage(IMG_RATIOX_BACK, 0,0);
	g->DrawImage(IMG_RATIOX_MainMsg,810, 552);
	g->DrawImage(IMG_RATIOX_BtnBack,423,716);

// 	g->SetColor(Sexy::Color(255,255,130));
// 	g->SetFont(m_pFont9WeiRuanYaHei);	
// //  	_bstr_t m_description = szXiJin;
// //  	wchar_t* pwcharDes = (wchar_t*)m_description;
// 	SexyString str;
// 	str = Sexy::StrFormat(_S("喜金:牛牛牌型即可得%ld元宝奖励"),m_lLikeIngot);
//  	g->WriteString(str,820, 610,184,0);
 	
}


//画数字的方法 theJustification 对齐方式 -1为左对齐 0为居中对齐(x就为中心坐标了),1为右对齐(x就为最右边的坐标)
void CGameView::DrawNum1( Sexy::Graphics* g, Sexy::Image *img, LONGLONG iNum, int x, int y, int theJustification /*= 0*/ )
{
	int i;
	if(iNum >= 0)
	{		
		LONGLONG iResult = iNum;
		int iWidth = /*img->GetWidth()/10;*/img->GetCelWidth()-3;
		int iHeight = img->GetHeight();

		int iPos[20];//不能超过9位数
		int iIndex=0;//几位数
		memset(iPos, 0x00, sizeof(iPos));

		for(i=19; i>0; i--)
		{
			iPos[i] = (int)ABS(iResult/pow((float)10,i));		
			if(iPos[i] > 0)
			{
				iResult = iResult%((LONGLONG)pow((float)10,i));
			}
		}
		iPos[0] = ABS(iResult%10);
		for(i=19; i>=0; i--)
		{
			if(iPos[i] != 0)
			{
				iIndex = i+1;
				break;
			}
		}

		int iLeft, iTop;
		iTop = y;
		if (theJustification < 0)//左对齐
		{
			iLeft = x;
		}
		else if (theJustification == 0)//居中
		{
			iLeft = x - (iIndex*iWidth)/2;
		}
		else//右对齐
		{
			iLeft = x - iIndex*iWidth;
		}
		if (iNum == 0)
		{
			iIndex = 1;
		}
		for(i=0; i<iIndex; i++)
		{
			g->DrawImageCel(img,iLeft+(iWidth)*i,iTop,iPos[iIndex-i-1]);
		}
	}
}

void CGameView::Update()
{
	Widget::Update();
	m_pTimer->Update();
	m_pGameTimer->Update();
	m_GameCard.Update();
	m_pChip.Update();

	for(int i=0; i<GAME_PLAYER; i++)
	{
		if(i==2) continue;
		if (m_bMouseSelect[i])
		{
			m_iOtherInfofPos[i][1]-=2;
			if (m_iOtherInfofPos[i][1]<=m_iOtherInfofPos[i][2]) m_iOtherInfofPos[i][1]=m_iOtherInfofPos[i][2];
			m_iOtherInfoTrans[i]+=10;
			if (m_iOtherInfoTrans[i]>=255) m_iOtherInfoTrans[i]=255;
		}
		else
		{
			m_iOtherInfofPos[i][1]+=2;
			if (m_iOtherInfofPos[i][1]>=m_iOtherInfofPos[i][2]+60) m_iOtherInfofPos[i][1]=m_iOtherInfofPos[i][2]+60;
			m_iOtherInfoTrans[i]-=10;
			if (m_iOtherInfoTrans[i]<=0) m_iOtherInfoTrans[i]=0;
		}
	}
	//聚宝盆提示
// 	if(m_bShowBowlHint)
// 	{
// 		m_iTransBowlHint +=6;
// 		m_iTransBowlHint = (m_iTransBowlHint>=255)?255:m_iTransBowlHint;
// 	}
// 	else
// 	{
// 		m_iTransBowlHint -=6;
// 		m_iTransBowlHint = (m_iTransBowlHint<=0)?0:m_iTransBowlHint;
// 	}
	//喜金提示
 	if(m_bShowLikeIngotHint)
 	{
 		m_iTransLikeIngotHing +=6;
 		m_iTransLikeIngotHing = (m_iTransLikeIngotHing>=255)?255:m_iTransLikeIngotHing;
 	}
 	else
 	{
 		m_iTransLikeIngotHing -=6;
 		m_iTransLikeIngotHing = (m_iTransLikeIngotHing<=0)?0:m_iTransLikeIngotHing;
 	}

	if (m_bGetMsg)
	{
		m_pGameApp->GetNetMessage();
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (m_pPlayer[i] != NULL)
		{
			m_pPlayer[i]->Update();
		}    
	}   

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_pPlayer[i] != NULL)
		{
			m_pAddMoneyAni[i]->Update();
		} 
	}

// 	m_pWaitOtherAni->Update();
// 	m_pAutoDeskAni->Update();
	MarkDirty();

	if (m_pAutoDeskAni != NULL)
	{
		m_pAutoDeskAni->Update();
	}
	if (m_pWaitOtherAni != NULL)
	{
		m_pWaitOtherAni->Update();
	}
	if (m_pLikeIngotAni != NULL)
	{
		m_pLikeIngotAni->Update();
	}
	if (m_pTuoGuanAni != NULL)
	{
		m_pTuoGuanAni->Update();
	}
// 	if (m_pJackocur != NULL)
// 	{
// 		m_pJackocur->Update();
// 	}
// 	if (m_pJackpotwin != NULL)
// 	{
// 		m_pJackpotwin->Update();
// 	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(i==2)continue;
		if (m_pOffLineAni[i] != NULL)
		{
			m_pOffLineAni[i]->Update();
		}
	}

	m_pGameApp->m_pSoundPlayer->UpdateMusic();

}

void CGameView::MouseMove( int x, int y )
{
	m_ptMouse.mX = x;
	m_ptMouse.mY = y;
	if (x>=0&&x<=119&&y>=240&&y<=527) m_bMouseSelect[1] = true;
	else if	(x>=201&&x<=410&&y>=87&&y<=344) m_bMouseSelect[0] = true;
	else if	(x>=597&&x<=787&&y>=64&&y<=359) m_bMouseSelect[4] = true;
	else if	(x>=883&&x<=1012&&y>=232&&y<=520) m_bMouseSelect[3] = true;
	else
	{
		ZeroMemory(m_bMouseSelect,sizeof(m_bMouseSelect));
	}
	if(x>=311&&x<=703&&y>=0&&y<=40) m_bShowLikeIngotHint = true;
	else if(x>=390&&x<=613&&y>=40&&y<=100)
	{
		if(m_bShowRewardCard)
			m_bShowLikeIngotHint = true;
		else
			m_bShowLikeIngotHint = false;
	}
	else m_bShowLikeIngotHint = false;

// 	if(x>=333&&x<=693&&y>=0&&y<=40) m_bShowBowlHint = true;
// 	else m_bShowBowlHint = false;


}

void CGameView::MouseDrag( int x, int y )
{
	m_ptMouse.mX = x;
	m_ptMouse.mY = y;
}

void CGameView::MouseLeave()
{
	m_ptMouse.mX = -1;
	m_ptMouse.mY = -1;

}

void CGameView::MouseEnter()
{

}

void CGameView::MouseDown( int x, int y, int theBtnNum, int theClickCount )
{
	if (theBtnNum == 1)
	{
		/*InitDiceAni0(4);*/

	}
	else if(theBtnNum == 0)
	{
		/*InitDiceAni1(6);*/
	}
	else if(theBtnNum == 2)
	{
		
	}

/*	m_pGameEnd->SetVisible(1);*/

// 	m_pGameApp->mWidgetManager->BringToFront(m_pGameEnd);
// 	m_pGameApp->mWidgetManager->SetFocus(m_pGameEnd);
	
 	printf("鼠标点击:[%d][%d]\n", x, y);
}

void CGameView::OnTimer( int id )
{
	switch (id)
	{
	case IDI_TIMER_PLAYER_STAU_0:
		{
			int act = rand()%4;
			if (act == 0)
			{
				m_pPlayer[0]->SetStatus(DAI_JI2);
			}
			else
			{
				m_pPlayer[0]->SetStatus(DAI_JI);
			}
			m_pTimer->KillTimer(IDI_TIMER_PLAYER_STAU_0);
			m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_0,6000);
		}
		break;
	case IDI_TIMER_PLAYER_STAU_1:
		{
			int act = rand()%4;
			if (act == 0)
			{
				m_pPlayer[1]->SetStatus(DAI_JI2);
			}
			else
			{
				m_pPlayer[1]->SetStatus(DAI_JI);
			}	
			m_pTimer->KillTimer(IDI_TIMER_PLAYER_STAU_1);
			m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_1,6000);
		}
		break;
	case IDI_TIMER_PLAYER_STAU_2:
		{

			m_pPlayer[2]->SetStatus(DAI_JI);
			m_pTimer->KillTimer(IDI_TIMER_PLAYER_STAU_2);
			m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_2,6000);
		}
		break;
	case IDI_TIMER_PLAYER_STAU_3:
		{
			int act = rand()%4;
			if (act == 0)
			{
				m_pPlayer[3]->SetStatus(DAI_JI2);
			}
			else
			{
				m_pPlayer[3]->SetStatus(DAI_JI);
			}
			m_pTimer->KillTimer(IDI_TIMER_PLAYER_STAU_3);
			m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_3,6000);
		}
		break;
	case IDI_TIMER_PLAYER_STAU_4:
		{
			int act = rand()%4;
			if (act == 0)
			{
				m_pPlayer[4]->SetStatus(DAI_JI2);
			}
			else
			{
				m_pPlayer[4]->SetStatus(DAI_JI);
			}

			m_pTimer->KillTimer(IDI_TIMER_PLAYER_STAU_4);
			m_pTimer->SetTimer(IDI_TIMER_PLAYER_STAU_4,6000);
		}
		break;
	case IDI_AUTO_CHANGE_DESK:
		{
			m_pTimer->KillTimer(IDI_AUTO_CHANGE_DESK);
			m_pGameDlg->SendChangeTable();
		}
		break;
	case IDI_SHOW_HURRY_TAG:
		{
			m_pTimer->KillTimer(IDI_SHOW_HURRY_TAG);
			m_wHurryCardUser = INVALID_CHAIR;
		}
		break;
	case IDI_TIMER_BTN_HURRY:
		{
			m_pTimer->KillTimer(IDI_TIMER_BTN_HURRY);
			/*m_wHurryCardUser = INVALID_CHAIR;*/
			m_pBtnQuickSub->SetDisabled(false);
			break;
		}
	case IDI_CARD_APPEAR_TIMER:
		{
			/*m_pTimer->SetTimer(IDI_CARD_APPEAR_TIMER,30);*/
			m_GameCard.AniTimer();
			m_pResultNum->AniTimer();
			break;
		}
	case IDI_CARD_DISAPPEAR_TIMER:
		{
			m_GameCard.CardDisAppearFrame();
			break;
		}
// 	case IDI_CHIP_ANI_TIMER:
// 		{
// 			m_pChip.AniTimer();
// 			break;
// 		}
	}
	
}

void CGameView::OnTimer( int id, WORD wChairID, UINT nElapse )
{
	switch (id)
	{
	case IDI_REWARDCARD_UP:
		{
			m_cpRewardCard.y -= 8;
			m_cpRewardCard.y = m_cpRewardCard.y<-21?-21:m_cpRewardCard.y;
		}
		break;
	case IDI_REWARDCARD_DOWN:
		{
			m_cpRewardCard.y += 8;
			m_cpRewardCard.y = m_cpRewardCard.y>40?40:m_cpRewardCard.y;
		}
		break;
	case IDI_TIMER_BTN_DOWN:
		{
			//拿wChairID当按钮标识
			int y = (8-nElapse)*5;
			int iYPos = cpTuoGuanUp.y+y;
			if (iYPos > cpTuoGuanDown.y)
			{
				iYPos = cpTuoGuanDown.y;
			}
			if(wChairID == 0) //托管
			{

				m_pBtnTuoGuan->Resize(440,iYPos,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
			}
			if (wChairID == 1)//取消托管
			{
				m_pBtnCancelTuoGuan->Resize(440,iYPos,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
				if (nElapse ==0)
				{
					m_pBtnCancelTuoGuan->SetVisible(false);
				}
			}
		}
		break;
	case IDI_TIMER_BTN_UP:
		{
			//拿wChairID当按钮标识
			int y = (8-nElapse)*5;
			int iYPos = cpTuoGuanDown.y -y;
			if (iYPos < cpTuoGuanUp.y)
			{
				iYPos = cpTuoGuanUp.y;
			}
			if(wChairID == 0) //托管
			{
				m_pBtnTuoGuan->Resize(440,iYPos,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
			}
			if (wChairID == 1)//取消托管
			{
				m_pBtnCancelTuoGuan->Resize(440,iYPos,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
			}
		}
		break;
	case IDI_NOT_ENOUGH_CHIP:
		{
			m_wTimeNum = nElapse;
			if (nElapse <= 5)
			{

			}
			if (nElapse <= 0)
			{	
				OperaTimer(IDI_READY_TIMER,0,0,FALSE);
				m_pGameDlg->PostMessage(WM_CLOSE);
			}
		}
		break;
	case IDI_COMPARE_CARD_FINISH:
		{
 			for (int i=IDI_TIMER_PLAYER_STAU_0; i<IDI_TIMER_PLAYER_STAU_0+5; i++)
 			{
 				m_pTimer->SetTimer(i,(i-IDI_TIMER_PLAYER_STAU_0)*1000+2000);
 			}
			m_pGameDlg->SendData(SUB_C_COMPARE_CARD);
		}
		break;
	case IDI_READY_TIMER:
		{
			//兑换筹码（准备）
			m_wTimeNum = nElapse;
			if (m_bTrustee)
			{
				m_pGameEnd->ButtonDepress(BT_CONTINUE);
			}
			if (nElapse <= 5)
			{
					
			}
			if (nElapse <= 0)
			{	
				OperaTimer(IDI_READY_TIMER,0,0,FALSE);
				m_pGameDlg->TimeOutCloseWidget();
			}

			break;
		}
	case IDI_START_TIMER:
		{
			//准备开始
			m_wTimeNum = nElapse;
			if (nElapse <= 5)
			{
				
			}
			if (nElapse <= 0)
			{	
				OperaTimer(IDI_START_TIMER,0,0,FALSE);
			}

			break;
		}
	case  IDI_OPEN_CARD:
		{
			//摊牌
			m_wTimeNum = nElapse;
			if (nElapse <= 5)
			{

			}
			if (nElapse == 0)
			{	
				OperaTimer(IDI_OPEN_CARD,0,INVALID_CHAIR,false);
				//摊牌消息
				OnBtnOpenCard();
// 				CMD_C_OxCard OpenCard;
// 				OpenCard.bOX = TRUE;
// 				m_pGameDlg->SendData(SUB_C_OPEN_CARD,&OpenCard,sizeof(CMD_C_OxCard));
			}
			break;
		}
	}
}

void CGameView::ButtonDepress( int theId )
{
	switch(theId)
	{
	case BT_EXCHANGECHIP_SPACE1:
	case BT_EXCHANGECHIP_SPACE2:
	case BT_EXCHANGECHIP_SPACE3:
	case BT_EXCHANGECHIP_SPACE4:
	case BT_EXCHANGECHIP_SPACE5:
		{
			OnExChip(theId);
		}
		break;
	case BT_OPEN_CARD:				//摊牌
		{
			OnBtnOpenCard();
		}
		break;
	case BT_HINT_CARD:				//提示
		{
			OnBtnHintCard();
		}
		break;
	case BT_RELIST_CARD:			//重排
		{
			OnBtnReListCard();
		}
		break;
	case BT_ADD_CHIP:				
		{
			if (m_pBtnTuoGuan->mDisabled == false)
			{
				m_pBtnTuoGuan->SetDisabled(true);
				MoveButton(0,0);
			}
			ShowExChip(true);
		}
		break;
	case BT_CUI_PAI:				//催牌
		{
			ShowUcCardDlg(!m_bShowUcCard);
		}
		break;
	case BT_TUOGUAN:
		{
			m_bTrustee = true;
			ShowExChip(false);
			m_pBtnTuoGuan->SetDisabled(true);
			MoveButton(0,0);
			MoveButton(1,1);
			m_pBtnCancelTuoGuan->SetVisible(true);
			if (m_bTrustee)
			{
				//播放托管动画
				m_pTuoGuanAni->Init(IMG_ANI_TuoGuanAni);
				m_pTuoGuanAni->SetLoopNum(-100);
				m_pTuoGuanAni->ShowAnimate(Sexy::Point(659,482),Sexy::Point(659,482),130);
			}
		}
		break;
	case BT_CANCEL_TUOGUAN:
		{
			if (m_bTrustee == false) break;
				
			m_bTrustee = false;
			if (m_pGameEnd->mVisible == true)
			{
				//m_pBtnTuoGuan->SetVisible(true);
			}
			else
			{
				m_pTuoGuanAni->StopAnimate();
				/*m_pBtnTuoGuan->SetDisabled(true);*/
				MoveButton(1,0);
			}
			//m_pBtnCancelTuoGuan->SetVisible(false);
		}
		break;
	}

}


void CGameView::KeyDown( Sexy::KeyCode theKey )
{
	if (theKey == KEYCODE_RETURN || theKey == KEYCODE_SPACE)
	{
		if (m_bTrustee)
		{
			return;
		}
		if(m_pBtnOpenCard->mVisible == true)
		{
			OnBtnOpenCard();
		}
		else if (m_pGameEnd->mVisible == true)
		{
			m_pGameEnd->ButtonDepress(BT_CONTINUE);
		}

	}
// 	else if(theKey == 'Q')
// 	{
// 		m_pTuoGuanAni->Init(IMG_ANI_TuoGuanAni);
// 		m_pTuoGuanAni->SetLoopNum(-100);
// 		m_pTuoGuanAni->ShowAnimate(Sexy::Point(m_x,m_y),Sexy::Point(m_x,m_y),130);
// 	}
// 	
// 	if (theKey == 'W')
// 	{
// 		m_y--;
// 	}
// 	else if (theKey == 'S')
// 	{
// 		m_y++;
// 	}
// 	else if (theKey == 'A')
// 	{
// 		m_x--;
// 	}
// 	else if (theKey == 'D')
// 	{
// 		m_x++;
// 	}
// 	else if (theKey == KEYCODE_DOWN)
// 	{
// 
// 		m_y+=10;
// 	}
// 	else if (theKey == KEYCODE_UP)
// 	{
// 
// 		m_y-=10;
// 	}
// 	else if (theKey == KEYCODE_RIGHT)
// 	{
// 		m_x+=10;
// 	}
// 	else if (theKey == KEYCODE_LEFT)
// 	{
// 		m_x-=10;
// 	}
// 	else if(theKey == KEYCODE_CONTROL)
// 	{
// 		iAciton --;
// 		if (iAciton <0)
// 		{
// 			iAciton =0;
// 		}
// 	}
// 	else if (theKey == KEYCODE_SHIFT)
// 	{
// 		iAciton ++;
// 		if (iAciton >4)
// 		{
// 			iAciton = 4;
// 		}
// 		for (int i=0; i<GAME_PLAYER; i++)
// 		{
// 			m_pPlayer[i]->SetStatus(HAPPY);
// 		}
// 		delete m_pOffLineAni;
// 		m_pOffLineAni = new Sexy::AnimLayer(m_pGameApp, this);
// 		for (int i=0; i<GAME_PLAYER; i++)
// 		{
// 			if (i == 2) continue;
// 
// 			m_pOffLineAni[i]->Init(IMG_ANI_OffLineAni);
// 			m_pOffLineAni[i]->SetLoopNum(-100);
// 			m_pOffLineAni[i]->ShowAnimate(Sexy::Point(cpOffLine[i].x,cpOffLine[i].y),Sexy::Point(cpOffLine[i].x,cpOffLine[i].y),250);
// 
// 		}

// 	}
// 	else if (theKey == 'Q')
//  	{
// 		for (int i=0; i<GAME_PLAYER; i++)
// 		{
// 			m_pPlayer[i]->SetStatus((PlayerAct)iAciton);
// 		}
		/*m_cpRewardCard.y = 40;*/
  		//for (int i=0; i<GAME_PLAYER; i++)
  		//{
  		//	m_pAddMoneyAni[i]->ShowAnimate(Sexy::Point(cpMoneyAni[i].x-10,cpMoneyAni[i].y-120),Sexy::Point(cpMoneyAni[i].x-10,cpMoneyAni[i].y-120),50);
  		//	/*lScore[i] = 1000;*/
  		//	/*m_cbPalyStatus[i] = 1;*/
  		//}
		//播放喜金动画
// 		m_pLikeIngotAni->Init(IMG_ANI_LikeIngotAni);
// 		m_pLikeIngotAni->SetLoopNum(1);
// 		m_pLikeIngotAni->SetEndHide(true);
//		m_pLikeIngotAni->ShowAnimate(Sexy::Point(350+10,160+30),Sexy::Point(350+10,160+30),50);
		//播放获得聚宝盆动画
// 		m_pJackpotwin->Init(IMG_ANI_jackpotwin);
// 		m_pJackpotwin->SetLoopNum(1);
// 		m_pJackpotwin->SetEndHide(true);
// 		m_pJackpotwin->ShowAnimate(Sexy::Point(399, 258),Sexy::Point(399, 258),100);

 		//播放财神路过动画
//   		m_pJackocur->Init(IMG_ANI_jackocur);
//   		m_pJackocur->SetLoopNum(10);
//   		m_pJackocur->SetEndHide(true);
//   		m_pJackocur->ShowAnimate(Sexy::Point(m_x, m_y),Sexy::Point(m_x, m_y),100);

/*	}*/
	//cpCardType[4].SetPoint(m_x, m_y);
	/*m_GameCard.SetPos(m_x, m_y);*/
// 	m_iOtherInfofPos[m_index][0] = m_x;
// 	m_iOtherInfofPos[m_index][1] = m_y;
// 	m_pMyBasicInfo->SetPosition(m_x, m_y);
 	//printf("键盘%d [%d][%d]\n",(int)theKey, m_x, m_y);
	//m_pBtnAddChip->Resize(120+m_x,600+m_y,IMG_BTN_BtnAddChip->GetWidth()/4, IMG_BTN_BtnAddChip->GetHeight());
	//m_pBtnQuickSub->Resize(m_x,m_y,IMG_BTN_BtnQuickSub->GetWidth()/4, IMG_BTN_BtnQuickSub->GetHeight());
//	m_pBtnTuoGuan->Resize(m_x,m_y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
// 	m_pListUcCard->SetVisible(true);
// 	m_pListUcCard->Resize(762+m_x,480+m_y,214,178);
// 	m_pBtnReListCard->Resize(640,m_y,IMG_BTN_BtnHint->GetWidth()/4, IMG_BTN_BtnHint->GetHeight());
// 	BYTE key[11] = {
// 		KEYCODE_UP,KEYCODE_UP,KEYCODE_DOWN,KEYCODE_DOWN,KEYCODE_LEFT,KEYCODE_RIGHT,KEYCODE_LEFT,KEYCODE_RIGHT,'B','A',KEYCODE_RETURN
// 	};
// 	static int iCount = 0;
// 	if (theKey == key[iCount])
// 	{
// 		iCount++;
// 	}
// 	else
// 	{
// 		iCount = 0;
// 	}
// 	if (iCount == 11)
// 	{
// 		iCount = 0;
// 		char buf[20];
// 		char sz[200];
// 		sprintf_s(sz,200,"喜报：恭喜潘天然玩家，在矿山大冒险新手房中，获得%d欢乐豆！", 111);
// 		m_pTopBullWidget->AddString(sz);
// 	}	
// 	

}

void CGameView::Resize( int theX, int theY, int theWidth, int theHeight )
{
	Widget::Resize(theX, theY, theWidth, theHeight);
}

//更新列表控件(标示cbMark: 0为更新 1为添加 2为删除)
void CGameView::OnGameMessage(WPARAM wParam, LPARAM lParam)					//窗口消息
{

}

bool CGameView::OnGameMessage( WORD wSubCmdID, const void * pBuffer, WORD wDataSize )
{
	switch(wSubCmdID)
	{
	case SUB_S_RECONN:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_OffLine_Reconn));
			if (wDataSize!=sizeof(CMD_S_OffLine_Reconn)) return false;
			CMD_S_OffLine_Reconn * pReconn = (CMD_S_OffLine_Reconn *)pBuffer;
			if (pReconn->wOperaUser>=GAME_PLAYER && pReconn->wOperaUser <0)return false;
			if(m_pGameDlg->GetUserData(pReconn->wOperaUser) == NULL) return true;		
			//取消断线动画
			WORD wChair = m_pGameDlg->SwitchViewChairID(pReconn->wOperaUser);
			if(wChair == 2) return true;
			m_pOffLineAni[wChair]->StopAnimate();
			return true;
		}
	case SUB_S_OFFLINE:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_OffLine_Reconn));
			if (wDataSize!=sizeof(CMD_S_OffLine_Reconn)) return false;
			CMD_S_OffLine_Reconn * pOffLine = (CMD_S_OffLine_Reconn *)pBuffer;

			if (pOffLine->wOperaUser>=GAME_PLAYER && pOffLine->wOperaUser <0)return false;
			if(m_pGameDlg->GetUserData(pOffLine->wOperaUser) == NULL) return true;		
			//播放断线动画
			WORD wChair = m_pGameDlg->SwitchViewChairID(pOffLine->wOperaUser);
			if(wChair == 2) return true;
			m_pOffLineAni[wChair]->Init(IMG_ANI_OffLineAni);
			m_pOffLineAni[wChair]->SetLoopNum(-100);
			m_pOffLineAni[wChair]->ShowAnimate(Sexy::Point(cpOffLine[wChair].x,cpOffLine[wChair].y),Sexy::Point(cpOffLine[wChair].x,cpOffLine[wChair].y),250);
			return true;
		}
	case SUB_S_BOWL:					//获得喜金和聚宝盆信息
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_Bowl));
			if (wDataSize!=sizeof(CMD_S_Bowl)) return false;
			
			CMD_S_Bowl * pBowl = (CMD_S_Bowl*)pBuffer;

			m_lBowl = pBowl->lBowlIngot;
			m_lLikeIngot = pBowl->lLikeGoldIngot;
			
			CopyMemory(m_lNewLikeIngot, pBowl->lNewLikeIngot, sizeof(m_lNewLikeIngot));

			MakeLikeIngotImg();
			return true;
		}
	case SUB_S_SEND_BOWL:				//获得聚宝盆/喜金
		{
			OnSubBowl(pBuffer, wDataSize);
		}
	case SUB_S_XIJIN:					//获得喜金
		{
			return true;
		}
	case SUB_S_GAME_FREE:				
		{
			return	OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_OPEN_CARD:				//摊牌
		{
			return	OnSubOpenCard(pBuffer, wDataSize);
		}
	case SUB_S_HURRY_CARD:				//催牌
		{
			return	OnSubHurryCard(pBuffer, wDataSize);
		}
	case SUB_S_GAME_FINISH:				//游戏局结束
		{
			return OnSubGameAllFinish(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:				//游戏结算
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_EX_CHIP:
		{
			OperaTimer(IDI_READY_TIMER,0,INVALID_CHAIR,FALSE);
			//发送消息
			if (m_pGameDlg->GetUserData(m_wMeChairID)->cbUserStatus != US_READY)
			{
				m_pGameDlg->SendUserReady(NULL,0);
			}
			WORD wUserCount = 0;		//玩家人数
			for (int i=0; i<GAME_PLAYER; i++)
			{
				if (m_pGameDlg->GetUserData(i) != NULL)
				{
					wUserCount++;
				}
			}
			if (wUserCount<2)
			{
				OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
				ShowAutoDeskAni();
				m_pTimer->SetTimer(IDI_AUTO_CHANGE_DESK, 5000);
			}
			else
			{
				//TODO:等待其它玩家
				OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
				m_pTimer->KillTimer(IDI_AUTO_CHANGE_DESK);
			}

			m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(true);
			ControlCardData();
			//玩家准备动作  
			//m_GameClientView.m_GameClientViewDX.m_GamePlayer[SwitchChairID(m_wMeChairID)].SetStatus(ZHUN_BEI);

			return true;
		}
	case SUB_S_GAME_START:				//游戏开始
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_GAME_READY:
		{
			return OnSubGameReady(pBuffer, wDataSize);
		}
	case GS_OX_FREE:					//空闲场景
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			/*m_pBtnAddChip->SetVisible(1);*/
			const tagUserData *pUserData = m_pGameDlg->GetUserData(m_pGameDlg->GetMeChairID());
			if (pUserData == NULL)
			{
				ASSERT(pUserData == NULL);
				//_log("DZPK_Client", "自己UserData为空");
				return false;
			}
			/*_log("RatioxLog","%s","空闲场景！");*/
			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			m_lCellScore = pStatusFree->lCellScore;
			m_lMinbet = pStatusFree->lMinBet;
			m_wServerType = pStatusFree->wServerType;

			CopyMemory(m_lChipNum, pStatusFree->lChipNum, sizeof(m_lChipNum));
			CopyMemory(m_lPercentage, pStatusFree->lPercentage, sizeof(m_lPercentage));
			
			if(!m_pGameDlg->IsLookonMode())	
			{
				m_wMeChairID = m_pGameDlg->GetMeChairID();
				m_pMyBasicInfo->SetMyChair(m_wMeChairID);
			}
			
			m_pGameDlg->SetGameStatus(GS_OX_FREE);

			OperaTimer(IDI_READY_TIMER,TIME_READY,INVALID_CHAIR, TRUE);
			/*m_pGameTimer->SetTimer(IDI_READY_TIMER,1000,INVALID_CHAIR, TIME_READY);*/
			if (pUserData->lBodyChip < 3*m_lMinbet)
			{
				ShowExChip(true);
			}
			else
			{
				OperaTimer(IDI_READY_TIMER,0,INVALID_CHAIR,FALSE);
				WORD wUserCount = 0;		//玩家人数
				for (int i=0; i<GAME_PLAYER; i++)
				{
					if (m_pGameDlg->GetUserData(i) != NULL)
					{
						wUserCount++;
					}
				}
				if (wUserCount<2)
				{
					OperaTimer(IDI_READY_TIMER,0,INVALID_CHAIR,FALSE);
					ShowAutoDeskAni();
					m_pTimer->SetTimer(IDI_AUTO_CHANGE_DESK, 5000);
				}
				else
				{
					OperaTimer(IDI_READY_TIMER,0,INVALID_CHAIR,FALSE);
					m_pTimer->KillTimer(IDI_AUTO_CHANGE_DESK);
				}
				m_pGameDlg->SendUserReady(NULL,0);
				m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(true);
			}

			return true;
		}
	case GS_OX_PLAY:					//游戏场景
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			const tagUserData *pUserData = m_pGameDlg->GetUserData(m_pGameDlg->GetMeChairID());
			if (pUserData == NULL)
			{
				ASSERT(pUserData == NULL);
				//_log("DZPK_Client", "自己UserData为空");
				return false;
			}


			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			m_lCellScore = pStatusPlay->lCellScore;
			m_lMinbet = pStatusPlay->lMinBet;
			m_wServerType = pStatusPlay->wServerType;
			CopyMemory(m_lChipNum, pStatusPlay->lChipNum, sizeof(m_lChipNum));
			CopyMemory(m_lPercentage, pStatusPlay->lPercentage, sizeof(m_lPercentage));
			CopyMemory(m_cbHandCard,pStatusPlay->cbCardData, sizeof(m_cbHandCard));
			CopyMemory(m_cbPalyStatus, pStatusPlay->cbPlayStatus, sizeof(m_cbPalyStatus));
			for (int i=0; i<GAME_PLAYER; i++)
			{
				if (pStatusPlay->cbOx[i] == 1)
				{
					m_bOpenCard[i] = true;
				}
				else
				{
					m_bOpenCard[i] = false;
				}
			}
			CopyMemory(m_cbRewardCard, pStatusPlay->cbRewardCardData, sizeof(m_cbRewardCard));

			for (int i=0; i<GAME_PLAYER; i++)
			{
				if (m_cbPalyStatus[i] == FALSE) continue;
				WORD wChair = m_pGameDlg->SwitchViewChairID(i);
				m_pChip.AddChips(m_lMinbet, wChair);
			}
			m_pGameDlg->SetGameStatus(GS_OX_PLAY);



			return true;
		}
	case GS_OX_ACCOUNTS:				//摊牌结算场景
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_S_StatusEnd));
			if (wDataSize!=sizeof(CMD_S_StatusEnd)) return false;

			const tagUserData *pUserData = m_pGameDlg->GetUserData(m_pGameDlg->GetMeChairID());
			if (pUserData == NULL)
			{
				ASSERT(pUserData == NULL);
				//_log("DZPK_Client", "自己UserData为空");
				return false;
			}
			//_log("RatioxLog","%s","结算场景！");
			//消息处理
			CMD_S_StatusEnd * pStatusEnd=(CMD_S_StatusEnd *)pBuffer;
			m_lCellScore = pStatusEnd->lCellScore;
			m_lMinbet = pStatusEnd->lMinBet;
			m_wServerType = pStatusEnd->wServerType;

			CopyMemory(m_lChipNum, pStatusEnd->lChipNum, sizeof(m_lChipNum));
			CopyMemory(m_lPercentage, pStatusEnd->lPercentage, sizeof(m_lPercentage));
			CopyMemory(m_cbPalyStatus, pStatusEnd->cbPlayStatus, sizeof(m_cbPalyStatus));

			if(!m_pGameDlg->IsLookonMode())	
			{
				m_wMeChairID = m_pGameDlg->GetMeChairID();
				m_pMyBasicInfo->SetMyChair(m_wMeChairID);
			}
			
			m_pGameDlg->SetGameStatus(GS_OX_ACCOUNTS);
				
			ShowUcCardDlg(false);
			m_pBtnQuickSub->SetDisabled(true);
			m_bAddScore = false;
			ZeroMemory(m_cbHandCard, sizeof(m_cbHandCard));
			CopyMemory(m_cbHandCard, pStatusEnd->cbCardData, sizeof(m_cbHandCard));
			//牛牛信息
			// 	BYTE cbTempOx[GAME_PLAYER];
			// 	ZeroMemory(cbTempOx, sizeof(cbTempOx));

			for (int i=0; i<GAME_PLAYER; i++)
			{
				if (m_cbPalyStatus[i] == FALSE)continue;
				/*if (i==m_wMeChairID) continue;*/
				WORD wChair = m_pGameDlg->SwitchViewChairID(i);
				m_GameCard.InitCardData(wChair,pStatusEnd->cbCardData[i], MAX_COUNT);

				BYTE cbOutCard[2];
				if ( m_pGameLogic->GetBestCardType(m_cbHandCard[i],MAX_COUNT,cbOutCard,true) != OX_VALUE0)
				{
					WORD wChair = m_pGameDlg->SwitchViewChairID(i);
					m_GameCard.SetOutCard(wChair,cbOutCard);
				}
			}
			BYTE cbTempOx[GAME_PLAYER];
			for (int i=0; i<GAME_PLAYER; i++)
			{
				cbTempOx[i] = 11;
			}
			for (int i=0; i<GAME_PLAYER; i++)
			{	
				if(m_cbPalyStatus[i] == FALSE)continue;
				WORD wChair = m_pGameDlg->SwitchViewChairID(i);
				m_cbCardType[i] = m_pGameLogic->GetBestCardType(m_cbHandCard[i],MAX_COUNT);
				cbTempOx[wChair] = m_cbCardType[i];
			}
			m_pGameEnd->SetOxData(cbTempOx);
			LONGLONG lViewScore[GAME_PLAYER];
			BYTE cbViewStatus[GAME_PLAYER];
			ZeroMemory(lViewScore, sizeof(lViewScore));
			ZeroMemory(cbViewStatus, sizeof(cbViewStatus));
			for(int i=0; i<GAME_PLAYER; i++)
			{
				if (m_cbPalyStatus[i] == FALSE) continue;
				WORD wChair = m_pGameDlg->SwitchViewChairID(i);
				lViewScore[wChair] = pStatusEnd->lGameScore[i];
				cbViewStatus[wChair] = m_cbPalyStatus[i];
			}
			m_pResultNum->SetResultNum(lViewScore, cbViewStatus);
			ShowMoneyAni(pStatusEnd->lGameScore);
			m_bShowOxType = true;
		


			return true;
		}
	}
	return true;
}

void CGameView::AddBull( LPCTSTR pszString )
{
	/*_log("RatioxLog", "喜报%s",pszString);*/
	m_pTopBullWidget->AddString(pszString);
}

void CGameView::DrawTextString( Sexy::Graphics* g, const SexyString& theString, int theX, int theY, int iWidth,int itheJustification, Sexy::Color crText, Sexy::Color crFrame )
{
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	g->SetColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		g->WriteString(theString, theX+nXExcursion[i], theY+nYExcursion[i], iWidth,itheJustification);
	}
	g->SetColor(crText);
	g->WriteString(theString, theX, theY, iWidth,itheJustification);

}

void CGameView::DrawPlayer( Sexy::Graphics* g )
{

	//人物测试
// 	for (int i=0;i<GAME_PLAYER;i++)
// 	{
// 		WORD wChair =i;
// 		if (m_pPlayer[wChair] != NULL)
// 		{
// 			//调整位置
// 			if (wChair == -3)
// 			{			
// 				m_pPlayer[wChair]->SetPlayerPos(m_x,m_y);
// 				m_pPlayer[wChair]->SetStatus(ZHENG_CHANG);
// 			}
// 			m_pPlayer[wChair]->Draw(g);
// 		}
// 	}

	//人物动画
 	for (int i=0;i<GAME_PLAYER;i++)
 	{
 		tagUserData const *pUserData = m_pGameDlg->GetUserData(i);
 		if (pUserData!=NULL)
 		{
 			WORD wChair =m_pGameDlg->SwitchViewChairID(i);
 			if (m_pPlayer[wChair] != NULL)
 			{
 				m_pPlayer[wChair]->Draw(g);
 			}
 		}
 	}
}


	

void CGameView::RemoveAllAni()
{
// 	if (m_pDiceAni0 != NULL)
// 	{
// 		delete m_pDiceAni0;
// 	}
// 	if (m_pDiceAni1 != NULL)
// 	{
// 		delete m_pDiceAni1;
// 	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		SAFE_DELETE(m_pPlayer[i]);
		SAFE_DELETE(m_pAddMoneyAni[i]);
		SAFE_DELETE(m_pOffLineAni[i]);
	}

	SAFE_DELETE(m_pWaitOtherAni);
	SAFE_DELETE(m_pLikeIngotAni);
	SAFE_DELETE(m_pAutoDeskAni);
	SAFE_DELETE(m_pTuoGuanAni);
// 	SAFE_DELETE(m_pJackpotwin);
// 	SAFE_DELETE(m_pJackocur);

}

void CGameView::OnAniFinish( int id )
{
	
	if (id == ANI_DICE_OVER_CALLBACK)
	{
		
	}
	if (ANI_CARD_APPEAR <= id && id<= ANI_CARD_APPEAR+30)
	{

	}
	if (id == ANI_CARD_APPEAR + 31)
	{

	}
}



void CGameView::ShowWaitAni()
{
	m_pWaitOtherAni->Init(IMG_ANI_WaitOhter);
	m_pWaitOtherAni->SetLoopNum(-100);
	m_pWaitOtherAni->ShowAnimate(Sexy::Point(m_cpTimer.x-95,m_cpTimer.y+95),Sexy::Point(m_cpTimer.x-95,m_cpTimer.y+95),80);
}


void CGameView::ShowAutoDeskAni( bool bShow/*=true*/ )
{
	if (bShow)
	{
		m_pAutoDeskAni->Init(IMG_ANI_AutoDesk);
		m_pAutoDeskAni->SetLoopNum(-100);
		m_pAutoDeskAni->ShowAnimate(Sexy::Point(441-95,320+85),Sexy::Point(441-95,320+85),80);
	}
	else
	{
		m_pAutoDeskAni->StopAnimate();
	}

}
//画动画
void CGameView::DrawAni( Sexy::Graphics* g )
{
// 	if (m_pDiceAni0 != NULL)
// 	{
// 		m_pDiceAni0->Draw(g);
// 	}
// 	if (m_pDiceAni1 != NULL)
// 	{
// 		m_pDiceAni1->Draw(g);
// 	}
	if (m_pWaitOtherAni != NULL)
	{
		m_pWaitOtherAni->Draw(g);
	}
	if (m_pAutoDeskAni != NULL)
	{
		m_pAutoDeskAni->Draw(g);
	}
	if (m_pLikeIngotAni != NULL)
	{
		m_pLikeIngotAni->Draw(g);
	}
	if (m_pTuoGuanAni != NULL)
	{
		m_pTuoGuanAni->Draw(g);
	}
// 	if (m_pJackocur != NULL)
// 	{
// 		m_pJackocur->Draw(g);
// 	}
// 	if (m_pJackpotwin != NULL)
// 	{
// 		m_pJackpotwin->Draw(g);
// 	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(m_pAddMoneyAni[i] != NULL)
			m_pAddMoneyAni[i]->Draw(g);
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(i ==2)continue;
		if (m_pOffLineAni[i] != NULL)
		{
			m_pOffLineAni[i]->Draw(g);
		}
	}


}
//画信息（自己信息，房间相关信息）
void CGameView::DrawBaseInfo( Sexy::Graphics* g )
{
	const tagUserData *pUserData = m_pGameDlg->GetUserData(m_wMeChairID);
	if (pUserData == NULL) return; 

	SexyString str;
	//昵称
 	g->SetFont(m_pFont10WeiRuanYaHei);
	g->SetColor(Sexy::Color(255,255,0));
 	_bstr_t bstr = pUserData->szNickName;
 	wchar_t pname[NAME_LEN];
 	StrCpyW(pname, (wchar_t *)bstr);
	g->WriteString(pname, 85, 672, 100,0);
 	/*DrawTextString(g,pname, 85,644,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); */
	//欢乐豆（灵珠）
	/*g->SetFont(m_pFont10WeiRuanYaHei);*/
	if (m_pGameDlg->m_wServerType == GAME_GENRE_GOLD)
	{
		str = Sexy::StrFormat(_S("%d"),pUserData->lBodySilverScore);
	}
	else
	{
		str = Sexy::StrFormat(_S("%d"),pUserData->lBodyScore);
	}
	g->WriteString(str, 85, 690, 100,0);
 	/*DrawTextString(g,str, 84,662,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); */
// 	//等级
	_bstr_t m_grade = szOrder[GetUserOrder(pUserData->lExperience)];
	wchar_t* pwcharDJ = (wchar_t*)m_grade;
	g->WriteString(pwcharDJ, 85, 656, 100,0);
// 	DrawTextString(g,pwcharDJ, 84,628,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); 
// 	//元宝
 	/*g->SetFont(m_pFont10WeiRuanYaHei);*/
 	str = Sexy::StrFormat(_S("%d"),pUserData->lGameGold);
	g->WriteString(pwcharDJ, 83, 707, 100,0);
 	/*DrawTextString(g,str, 83,679,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); */
// 	

 	/*g->SetFont(m_pFont10WeiRuanYaHei);*/
 	str = Sexy::StrFormat(_S("%d"),pUserData->lBodyChip);
	g->WriteString(str, 94, 585, 100,0);
 	/*DrawTextString(g,str, 94,557,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); */

	/*g->SetFont(m_pFont10WeiRuanYaHei);*/
	if(m_pGameDlg->m_wServerType == GAME_GENRE_SCORE_CHIP)
	{
		str = Sexy::StrFormat(_S("1筹码=%d灵珠"),m_lCellScore);
	}
	else
	{
		str = Sexy::StrFormat(_S("1筹码=%d欢乐豆"),m_lCellScore);
	}
	g->WriteString(str, 17, 619, 100,0);
	//DrawTextString(g,str, 17,591,100,0,Sexy::Color(255,255,0), Sexy::Color(0,0,0)); 
}
//兑换筹码
void CGameView::OnExChip( UINT nIDEvent )
{
	ASSERT(m_pGameDlg->m_lCellScore>0);
	//隐藏对话框
	ShowExChip(false);
	CMD_C_ExchangeChip cExChip;
	cExChip.wExchangeUserChairID = m_pGameDlg->GetMeChairID();
	switch (nIDEvent)
	{
	case BT_EXCHANGECHIP_SPACE1:
		{
			cExChip.lExchangeCount = m_lChipNum[0];
			break;
		}
	case BT_EXCHANGECHIP_SPACE2:
		{
			cExChip.lExchangeCount = m_lChipNum[1];
			break;
		}
	case BT_EXCHANGECHIP_SPACE3:
		{
			cExChip.lExchangeCount = m_lChipNum[2];
			break;
		}
	case BT_EXCHANGECHIP_SPACE4:
		{
			cExChip.lExchangeCount = m_lChipNum[3];
			break;
		}
	case BT_EXCHANGECHIP_SPACE5:
		{
			cExChip.lExchangeCount = m_lChipNum[4];
			break;
		}
	}
	m_pGameDlg->SendData(SUB_C_EX_CHIP, &cExChip, sizeof(cExChip));	
}

void CGameView::ShowExChip( bool bShow /*= false*/ )
{
	m_bShowExChangeChipDlg = bShow;	
	m_cpTimer.SetPoint(441,300);
	for (int i=0; i<MAX_CHIP_COUNT; i++)
	{
		m_pBtnExChip[i]->SetVisible(bShow);
	}
	if (bShow)
	{
		m_pGameEnd->SetVisible(false);
		//m_pBtnTuoGuan->SetVisible(false);
		m_pResultNum->SetNowShow(false);
		m_cpTimer.SetPoint(441,140);
		for (int i=0; i<MAX_CHIP_COUNT; i++)
		{
			m_pBtnExChip[i]->SetDisabled(true);
		}
		LONGLONG lNeedScore[MAX_CHIP_COUNT];
		for (int i=0; i<MAX_CHIP_COUNT; i++)
		{
			lNeedScore[i] = m_lChipNum[i]*m_lCellScore + m_lChipNum[i]*m_lPercentage[i]/100;
		}
		const tagUserData *pUserData = m_pGameDlg->GetUserData(m_wMeChairID);
		if (pUserData == NULL) return; 
		int index = -1;
		for (int i=0; i<MAX_CHIP_COUNT; i++)
		{
			if (m_wServerType == GAME_GENRE_GOLD) //欢乐豆
			{
				if (pUserData->lBodySilverScore>=  lNeedScore[i])
				{
					index = i;
				}
				else
				{
					break;
				}
			}
			else if (m_wServerType == GAME_GENRE_SCORE_CHIP)
			{
				if (pUserData->lBodyScore>=  lNeedScore[i])
				{
					index = i;
				}
				else
				{
					break;
				}
			}

		}
		for (int i=0; i<=index; i++)
		{
			m_pBtnExChip[i]->SetDisabled(false);
		}
	}
}

WORD CGameView::GetUserOrder( LONG lExperience )
{
	LONG lOrder[13] = {
		21600,		100800,		928800,		3376800,	8344800,
		14738400,	29440800,	47368800,	71416800,	141472800,
		246808800,	394624800,	59210800
	};
	for(int i=0; i<13; i++)
	{
		if(lExperience < lOrder[i])
		{
			return i;
		}
	}

	return 13;
}

void CGameView::ResetGameFrame()
{
	m_bGetMsg = true;
	ZeroMemory(m_cbHandCard, sizeof(m_cbHandCard));
	ZeroMemory(m_cbCardType, sizeof(m_cbCardType));

	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_bOpenCard[i] = false;
		
	} 
	m_bAddScore =false;
	m_bShowOxType = false;
	ShowUcCardDlg(false);
	m_pBtnReListCard->SetDisabled(false);
	m_wHurryCardUser = INVALID_CHAIR;
}

void CGameView::DrawReady( Sexy::Graphics* g )
{
	tagUserData const *pUserData;
	for (int ChairID=0;ChairID<GAME_PLAYER;ChairID++)
	{
		pUserData = m_pGameDlg->GetUserData(ChairID);
		if (pUserData==NULL) continue;
		WORD wChairID = m_pGameDlg->SwitchViewChairID(ChairID);

		if (pUserData->cbUserStatus == US_READY)
		{
			g->DrawImage(IMG_RATIOX_Ready,cpReady[wChairID].x,cpReady[wChairID].y);
		}
	}
}

void CGameView::DrawTest( Sexy::Graphics* g )
{
	/*g->DrawImage(IMG_RATIOX_Ready,m_x,m_y);*/
	/*g->DrawImage(IMG_RATIOX_SmallChip0,m_x, m_y);*/


	//牌面
// /*	iY -=4;*/
// 	SexyMatrix3 theTmpMatrix;
// 	theTmpMatrix.ZeroMatrix();
// 	theTmpMatrix.m[0][0] =1;
// 	theTmpMatrix.m[0][1] =0.1;
// 	theTmpMatrix.m[0][2] =0;
// 
// 	theTmpMatrix.m[1][0] = 0;
// 	theTmpMatrix.m[1][1] = 0.85;
// 	theTmpMatrix.m[1][2] = 0;
// 
// 	theTmpMatrix.m[2][0] = 0;
// 	theTmpMatrix.m[2][1] = 0;
// 	theTmpMatrix.m[2][2] = 1;
// 
// 	g->DrawImageMatrix(IMG_RATIOX_CardBackNormal,theTmpMatrix,510,620);
// 	g->SetColorizeImages(false);
}
//游戏准备开始
bool CGameView::OnSubGameReady( const void * pBuffer, WORD wDataSize )
{
	ASSERT(sizeof(CMD_S_GameReady) == wDataSize);
	if(sizeof(CMD_S_GameReady) != wDataSize) return false;
	CMD_S_GameReady *pGameReady = (CMD_S_GameReady*)pBuffer;
	OperaTimer(IDI_READY_TIMER,0,INVALID_CHAIR,FALSE);
	OperaTimer(IDI_START_TIMER,pGameReady->cbTimeLeave,INVALID_CHAIR,TRUE);
	//m_pGameTimer->SetTimer(IDI_START_TIMER, 1000,INVALID_CHAIR,pGameReady->cbTimeLeave);
	
	return true;
}

bool CGameView::OnSubGameStart( const void * pBuffer, WORD wDataSize )
{
	ASSERT(sizeof(CMD_S_GameStart) == wDataSize);
	if(sizeof(CMD_S_GameStart) != wDataSize) return false;
	m_pGameDlg->SetGameStatus(GS_OX_PLAY);
	CMD_S_GameStart *pGameStart = (CMD_S_GameStart*)pBuffer;
	m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(true);
	ShowExChip(false);
// 	m_pGameTimer->KillTimer(IDI_START_TIMER);
// 	m_pGameTimer->KillTimer(IDI_READY_TIMER);
	OperaTimer(IDI_START_TIMER,0,INVALID_CHAIR,FALSE);
	m_wTimeNum = 0;
	CopyMemory(m_cbPalyStatus, pGameStart->cbPlayStatus, sizeof(m_cbPalyStatus));
	CopyMemory(m_cbHandCard, pGameStart->cbCardData, sizeof(m_cbHandCard));
	ZeroMemory(m_cbRewardCard,sizeof(m_cbRewardCard));
	CopyMemory(m_cbRewardCard, pGameStart->cbRewardCardData, sizeof(m_cbRewardCard));

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE) continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		m_pChip.AddChips(m_lMinbet, wChair);
	}
	
	if (m_bTrustee )
	{
		/*m_pBtnCancelTuoGuan->SetVisible(true);*/
	}
	else
	{
		/*m_pBtnTuoGuan->SetVisible(true);*/
	}
	m_bAddScore = true;

	m_bShowRewardCard = true;
	m_pGameTimer->SetTimer(IDI_REWARDCARD_DOWN, 80,0, 10);

	
	return true;
}
//画桌面筹码
void CGameView::DrawDeskChip( Sexy::Graphics* g )
{
	for (int i=0 ; i<GAME_PLAYER; i++)
	{	
		const tagUserData *pUserData = m_pGameDlg->GetUserData(i);
		if (pUserData == NULL) continue; 
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		LONGLONG tempbodychip = pUserData->lBodyChip;
		m_pChip.DrawDeskChip1(g, wChair,tempbodychip);
	}

}
//压底注动画结束
void CGameView::OnYaDiFinish()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE)continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		m_GameCard.InitCardData(wChair,m_cbHandCard[i], MAX_COUNT);
	}
	m_GameCard.InitSendCardAni(0);
}
//时钟操作
void CGameView::OperaTimer( UINT nTimerID, UINT nElapse ,WORD wChair, BOOL bStart/*=true*/ )
{
	if (m_pGameDlg->GetUserData(m_wMeChairID) != NULL)
	{
		if (m_pGameDlg->GetUserData(m_wMeChairID)->cbUserStatus == US_READY)
		{
			m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(true);
		}
	}

	m_pWaitOtherAni->StopAnimate();
	ShowAutoDeskAni(false);
	m_pGameTimer->KillTimer(IDI_READY_TIMER);
	m_pGameTimer->KillTimer(IDI_START_TIMER);
	m_pGameTimer->KillTimer(IDI_OPEN_CARD);
	m_pGameTimer->KillTimer(IDI_NOT_ENOUGH_CHIP);
	m_wTimeNum = 0;
	m_cpTimer.SetPoint(441,320);

	if (bStart)
	{
		if(nTimerID == IDI_START_TIMER)
		{
			ShowWaitAni();
		}
		if (nTimerID == IDI_OPEN_CARD)
		{
			m_cpTimer.SetPoint(441,420);
		}
		m_wTimeNum = nElapse;
		m_pGameTimer->SetTimer(nTimerID,1000,wChair,nElapse);
	}
}

void CGameView::OnMoveFinish( int id )
{
	ASSERT(id>=0 && id<GAME_PLAYER);
	if (id == GAME_PLAYER-1) //发牌结束
	{
		OnSendCardFinish();
	}
	else
	{
		m_GameCard.InitSendCardAni(id+1);
	}
}

void CGameView::OnSendCardFinish()
{
	m_pBtnOpenCard->SetVisible(true);
	m_pBtnReListCard->SetVisible(false);
	m_pBtnHintCard->SetVisible(true);

	m_pBtnQuickSub->SetDisabled(false);
	m_pTimer->SetTimer(IDI_TIMER_BTN_HURRY,5000);
	
	for (int i=0; i<MAX_COUNT; i++)
	{
		for (int j=0; j<MAX_COUNT; j++)
		{
			if (m_cbRewardCard[m_wMeChairID][i] == m_cbHandCard[m_wMeChairID][j]) m_bShowSameTag[i] = true;
		}
	}
	
	OperaTimer(IDI_OPEN_CARD, 10,INVALID_CHAIR,true);

	if (m_bTrustee)
	{
		ButtonDepress(BT_OPEN_CARD);
	}

}
//提示按钮
void CGameView::OnBtnHintCard()
{
	BYTE cbOutCard[2];
	if ( m_pGameLogic->GetBestCardType(m_cbHandCard[m_wMeChairID],MAX_COUNT,cbOutCard,true) != OX_VALUE0)
	{
		m_GameCard.SetOutCard(2,cbOutCard);
		m_pBtnReListCard->SetVisible(true);
		m_pBtnHintCard->SetVisible(false);
	}
	else
	{
		m_pBtnHintCard->SetVisible(false);
		m_pBtnReListCard->SetVisible(true);
		m_pBtnReListCard->SetDisabled(true);
	}

}

void CGameView::OnBtnOpenCard()
{
	OperaTimer(0,0,0,FALSE);
	OnBtnHintCard();
	m_pBtnHintCard->SetVisible(false);
	m_pBtnReListCard->SetVisible(false);
	m_pBtnOpenCard->SetVisible(false);
	CMD_C_OxCard OpenCard;
	OpenCard.bOX = TRUE;
	m_pGameDlg->SendData(SUB_C_OPEN_CARD, &OpenCard, sizeof(OpenCard));
}

bool CGameView::OnSubGameEnd( const void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize == sizeof(CMD_S_GameEnd));
	if(wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	m_pGameDlg->SetGameStatus(GS_OX_ACCOUNTS);
	CMD_S_GameEnd * pGameEnd = (CMD_S_GameEnd *)pBuffer;
	
	for (int i=IDI_TIMER_PLAYER_STAU_0; i<IDI_TIMER_PLAYER_STAU_0+5; i++)
	{
		m_pTimer->KillTimer(i);
	}

	m_pTimer->KillTimer(IDI_TIMER_BTN_HURRY);
	ShowUcCardDlg(false);
	m_pBtnQuickSub->SetDisabled(true);
	m_bAddScore = false;
	ZeroMemory(m_bOpenCard, sizeof(m_bOpenCard));
	ZeroMemory(m_cbHandCard, sizeof(m_cbHandCard));
	CopyMemory(m_cbHandCard, pGameEnd->cbCardData, sizeof(m_cbHandCard));
	//牛牛信息
// 	BYTE cbTempOx[GAME_PLAYER];
// 	ZeroMemory(cbTempOx, sizeof(cbTempOx));

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE)continue;
		if (i==m_wMeChairID) continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		m_GameCard.InitCardData(wChair,pGameEnd->cbCardData[i], MAX_COUNT);

		BYTE cbOutCard[2];
		if ( m_pGameLogic->GetBestCardType(m_cbHandCard[i],MAX_COUNT,cbOutCard,true) != OX_VALUE0)
		{
			WORD wChair = m_pGameDlg->SwitchViewChairID(i);
			m_GameCard.SetOutCard(wChair,cbOutCard);
		}
	}
	
	BYTE cbTempOx[GAME_PLAYER];
	for (int i=0; i<GAME_PLAYER; i++)
	{
		cbTempOx[i] = 11;
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{	
		if(m_cbPalyStatus[i] == FALSE)continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		m_cbCardType[i] = m_pGameLogic->GetBestCardType(m_cbHandCard[i],MAX_COUNT);
		cbTempOx[wChair] = m_cbCardType[i];
	}
	m_pGameEnd->SetOxData(cbTempOx);

	LONGLONG lViewScore[GAME_PLAYER];
	BYTE cbViewStatus[GAME_PLAYER];
	ZeroMemory(lViewScore, sizeof(lViewScore));
	ZeroMemory(cbViewStatus, sizeof(cbViewStatus));
	for(int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE) continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		lViewScore[wChair] = pGameEnd->lGameScore[i];
		cbViewStatus[wChair] = m_cbPalyStatus[i];
	}
 	m_pResultNum->SetResultNum(lViewScore, cbViewStatus);
 	ShowMoneyAni(pGameEnd->lGameScore);
  	for (int i=0; i<GAME_PLAYER; i++)
  	{
  		if (m_cbPalyStatus[i] == FALSE) continue;
  		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
  		if (pGameEnd->lGameScore[i] >=0)
  		{
  			m_pPlayer[wChair]->SetStatus(HAPPY);
  		}
  		else
  		{
  			m_pPlayer[wChair]->SetStatus(JU_SANG);
  		}
  		
  	}

	m_bShowOxType = true;
	m_pGameTimer->SetTimer(IDI_COMPARE_CARD_FINISH,3000,0, 1);


	return true;
}
//重排
void CGameView::OnBtnReListCard()
{
	m_GameCard.RemoveMyAllCard();
	m_GameCard.InitCardData(2,m_cbHandCard[m_wMeChairID], MAX_COUNT);
	m_pBtnReListCard->SetVisible(false);
	m_pBtnHintCard->SetVisible(true);
}
//画押注数额
void CGameView::DrawAddScore( Sexy::Graphics *g )
{
	if (m_bAddScore == false) return;
	for (int i=0; i<GAME_PLAYER; i++) 
	{
		if (m_cbPalyStatus[i] == FALSE) continue;	
		if(m_pGameDlg->GetUserData(i) == NULL) continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		if (wChair ==2)
		{
			DrawNum(g,IMG_NUM_AddScore,m_lMinbet, cpAddScore[wChair].x, cpAddScore[wChair].y,0);
		}
		else
		{
			g->SetScale(0.7,0.7,1,1);
			DrawNum(g,IMG_NUM_AddScore,m_lMinbet, cpAddScore[wChair].x*10/7, cpAddScore[wChair].y*10/7,0);
			g->SetScale(1,1,1,1);
		}

	}
}

void CGameView::DrawOverlay( Sexy::Graphics* g, int thePriority )
{
	//自己的信息背景
// 	g->DrawImage(IMG_RATIOX_UserInfo,0,634);
// 	// 	//所持筹码背景
// 	g->DrawImage(IMG_RATIOX_UserChip,15,568);
// 	// 	//房间比例
// 	g->DrawImage(IMG_RATIOX_CellScoreBack,15,596);
// 
// 	//画信息（自己信息，房间相关信息）
// 	DrawBaseInfo(g);

	

 	m_pResultNum->Draw(g);
	//闹钟
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if ( m_wTimeNum > 0)
		{
			g->DrawImage(IMG_RATIOX_TimeBack,m_cpTimer.x,m_cpTimer.y);
			DrawNum(g,IMG_NUM_TimeNum,m_wTimeNum/10,m_cpTimer.x+50,m_cpTimer.y+30);
			DrawNum(g,IMG_NUM_TimeNum,m_wTimeNum%10,m_cpTimer.x+90,m_cpTimer.y+30);

		}
	}
	/*BringToFront(m_pBtnAddChip);*/
	//m_pResultNum->SetPosition(m_x, m_y);
}
//游戏全部结束
bool CGameView::OnSubGameAllFinish( const void * pBuffer, WORD wDataSize )
{
	m_bShowOxType = false;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(i==2) continue;
		if(m_pOffLineAni[i] !=NULL)
		{
			delete m_pOffLineAni[i];
			m_pOffLineAni[i] = new Sexy::AnimLayer(m_pGameApp, this);
		}
	}
	m_GameCard.SetCardDisAppear(true);
	if (m_pGameDlg->GetUserData(m_pGameDlg->GetMeChairID())->lBodyChip < m_lMinbet*3)
	{
		OperaTimer(IDI_NOT_ENOUGH_CHIP,TIME_READY,INVALID_CHAIR, TRUE);
		if (IDOK == ShowInformation(TEXT("您身上的筹码已不能继续游戏，即将为您退出游戏！"),m_wTimeNum,MB_OK))
		{
			m_pGameDlg->PostMessage(WM_CLOSE);
			return true;
		}
		//m_pGameDlg->ShowHintMsg();
	}
	/*m_bShowOxType = false;*/
 	//m_pResultNum->SetNotShow(false);
	m_GameCard.SetResultCardData();
	m_pGameEnd->SetGameEndResultCard(m_GameCard.m_vecResultCard);
	m_pGameEnd->SetVisible(true);
	m_pBtnTuoGuan->SetDisabled(false);
	MoveButton(0,1);
	OperaTimer(IDI_READY_TIMER,TIME_READY,INVALID_CHAIR, TRUE);
	m_cpTimer.SetPoint(441,339);
	m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(false);
	if (m_bTrustee)
	{
		m_pGameEnd->ButtonDepress(BT_CONTINUE);
	}
	m_bAddScore = false;
	m_bShowRewardCard = false;
	ZeroMemory(m_bShowSameTag, sizeof(m_bShowSameTag));
	m_pGameTimer->SetTimer(IDI_REWARDCARD_UP, 80,0,10);

	/*m_GameCard.RemoveMyAllCard();*/
	/*m_GameCard.SetCardDisAppear(true);*/
	/*m_GameCard.Reset();*/
	m_pChip.Reset();
	ResetGameFrame();
	return true;
}
//画比牌阶段个人的牌型
void CGameView::DrawOxType( Sexy::Graphics *g )
{
	if (m_bShowOxType)
	{
		for (int i= 0; i<GAME_PLAYER; i++)
		{
			if (m_cbPalyStatus[i] == FALSE) continue;
			WORD wChair = m_pGameDlg->SwitchViewChairID(i);
			g->DrawImageCel(IMG_RATIOX_BigCardType,cpCardType[wChair].x,cpCardType[wChair].y,m_cbCardType[i]>=10?10:m_cbCardType[i],0);
		}
	}
}
//测试配牌-------------------------------------------------------------------
void CGameView::ControlCardData()
{
 	//发送数据
 	int IsCheat=0;
 	CMD_C_Cheat CheatCard;
 	//读取配置
 	TCHAR szFileName[MAX_PATH];
 	TCHAR szPath[MAX_PATH];
 	GetCurrentDirectory(sizeof(szPath),szPath);
 	_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\RatiOxCheat.ini"),szPath);
 	IsCheat= (int)GetPrivateProfileInt(TEXT("CheatControl"), TEXT("bCheat"),0, szFileName);
 
 	CheatCard.iCheatControl = IsCheat;
 
 	if (IsCheat)
 	{
 		//读取扑克
 		for (int i=0;i<GAME_PLAYER;i++)
 		{
 			CString szText;
 			szText.Format(TEXT("Player%d"), m_pGameDlg->SwitchViewChairID(i));
 			for (int j=0;j<MAX_COUNT;j++)
 			{
 				CString szText0;
 				szText0.Format(TEXT("HandCard%d"),j);
 				CheatCard.cbHandCardData[i][j]= (BYTE)GetPrivateProfileInt( szText, szText0,0, szFileName);
 			}
 		}
 
 		ZeroMemory(CheatCard.cbRewardCard, sizeof(CheatCard.cbRewardCard));
 		for (int j=0;j<MAX_COUNT;j++)
 		{
 			CString szText0;
 			szText0.Format(TEXT("HandCard%d"),j);
 			CheatCard.cbRewardCard[m_wMeChairID][j]= (BYTE)GetPrivateProfileInt( TEXT("RewardCard"), szText0,0, szFileName);
 		}
 		CheatCard.lPoolScore = GetPrivateProfileInt( TEXT("ScorePool"),TEXT("ScorePool") ,0, szFileName);
 
 		m_pGameDlg->SendData(SUB_C_CHEAT,&CheatCard,sizeof(CheatCard));
 	}
}
//画兑换筹码框
void CGameView::DrawExChangeChipDlg( Sexy::Graphics *g )
{
	//兑换筹码框背景
	if (m_bShowExChangeChipDlg)
	{
		g->DrawImage(IMG_RATIOX_ExChipBack,167,250);
		LONGLONG lNeedScore[MAX_CHIP_COUNT];
		for (int i=0; i<MAX_CHIP_COUNT; i++)
		{
			lNeedScore[i] = m_lChipNum[i]*m_lCellScore + m_lChipNum[i]*m_lPercentage[i]/100;
			DrawNum1(g,IMG_NUM_ExChangeNum,m_lChipNum[i],cpExChangeChip[i].x, cpExChangeChip[i].y,0);
			DrawNum1(g,IMG_NUM_ExChangeNum,lNeedScore[i],cpExChangeChip[i].x, cpExChangeChip[i].y+37,0);
		}
	}
}

void CGameView::ShowCardAni( BYTE cbCardType, WORD wViewChair )
{
	
}
//画催牌
void CGameView::DrawUcCard( Sexy::Graphics *g )
{
	
	//g->SetFont(m_pFont10WeiRuanYaHei);
	//催牌框底图
	if (m_bShowUcCard)
	{
		g->DrawImage(IMG_RATIOX_UcCardBack,cpUcCardPosition.x,cpUcCardPosition.y);
	}

	if (m_wHurryCardUser != INVALID_CHAIR)
	{
		WORD wUser = m_pGameDlg->SwitchViewChairID(m_wHurryCardUser);
		g->DrawImage(IMG_RATIOX_HurryTag,cpHurryCard[wUser].x, cpHurryCard[wUser].y);

// 		CString	m_StrSkill[18];
// 		m_StrSkill[0] = "不敢出手，你怕啦！";
// 		m_StrSkill[1] = "快点啦，牌都在你手上下蛋啦！";
// 		m_StrSkill[2] = "时代在变，为何你的速度总不变";
// 		m_StrSkill[3] = "时间就是金钱，我的朋友。";
// 		m_StrSkill[4] = "喂！快点出啊，别磨磨蹭蹭的";
// 		m_StrSkill[5] = "喂喂喂，太阳落山啦。";
// 		m_StrSkill[6] = "无尽的等待，真让人闹心。";
// 		m_StrSkill[7] = "想啥呢，快出牌呀！";
// 		m_StrSkill[8] = "再不出牌，我扣120啦！";

// 		g->SetColor(Sexy::Color(51,25,0));  
// 		g->SetFont(m_pFont10WeiRuanYaHei);
// 		_bstr_t t = m_StrSkill[m_wHurryWordID];
// 		wchar_t* pwchar = (wchar_t*)t;
		//WriteString(g, pwchar, ptAction[wUser].x+42, ptAction[wUser].y+54, 124, 0);
//		WriteWordWrapped(g, Sexy::Rect(ptAction[wUser].x+42,ptAction[wUser].y+34,120,40), pwchar, 20, 0); //会换行
	}

}
//显示催牌框
void CGameView::ShowUcCardDlg( bool bShow /*= false*/ )
{
	m_bShowUcCard = bShow;
	m_pListUcCard->SetVisible(bShow);
	if(bShow)
		::PostMessage(m_pGameDlg->GetSafeHwnd(),SHOW_CHAT_MASSAGE,0,0);
	else
		::PostMessage(m_pGameDlg->GetSafeHwnd(),SHOW_CHAT_MASSAGE,1,1);
	/*m_pGameDlg->ShowChat(bShow);*/
// 	_log("RatioxLog","隐藏/显示 聊天框开始");
// 	if (m_bShowUcCard)
// 	{
// 		/*if(m_GameClientView.m_ChatMessage.IsWindowVisible())*/
// 		m_pGameDlg->m_GameClientView.m_ChatMessage.ShowWindow(SW_HIDE);
// 	}
// 	else
// 	{
// 		/*if(!m_GameClientView.m_ChatMessage.IsWindowVisible())*/
// 		m_pGameDlg->m_GameClientView.m_ChatMessage.ShowWindow(SW_SHOW);
// 	}
// 	_log("RatioxLog","隐藏/显示 聊天框结束");
}

void CGameView::ListClicked( int theId, int theIdx, int theClickCount )
{
	

	OnUcCard(theIdx);
	ShowUcCardDlg(false);
	m_pBtnQuickSub->SetDisabled(true);
	/*m_pBtCuiPai->SetDisabled(true);*/
	/*m_pTimer->SetTimer(ID_TIMER_UCCARDBTN_ENBLE, 10000);*/
}

void CGameView::OnUcCard( int iIndex )
{
	CMD_C_HurryCard HurryCard;
	HurryCard.cbWords = (BYTE)iIndex;
	/*_log("RatioxLog","发送催牌消息\n");*/
	if (m_pGameDlg->GetGameStatus() != GS_OX_PLAY)
	{
		/*_log("RatioxLog","发送催牌消息失败--GameStatus【%d】\n ",m_pGameDlg->GetGameStatus());*/
		return ;
	}

	m_pGameDlg->SendData(SUB_C_HURRY_CARD, &HurryCard, sizeof(CMD_C_HurryCard));
	/*_log("RatioxLog","发送催牌消息成功--索引值【%d】\n", iIndex);*/
}
//催牌消息处理
bool CGameView::OnSubHurryCard( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_HurryCard));
	if (wDataSize!=sizeof(CMD_S_HurryCard)) return false;
	//变量定义
	CMD_S_HurryCard * pHurryCard=(CMD_S_HurryCard *)pBuffer;
	//_log("RatioxLog","接受催牌消息\n");
	m_wHurryCardUser = pHurryCard->wCurrentUser;
	m_wHurryWordID = pHurryCard->cbWords;
	//_log("RatioxLog","我的座位号【%d】座位号【%d】索引值【%d】\n",m_wMeChairID,m_wHurryCardUser,m_wHurryWordID);
/*	WORD wViewUcardUser = m_pGameDlg->SwitchViewChairID(pPhrase->wPhraseUser);*/
	/*播放声音*/
	//_log("RatioxLog","播放催牌消息声音\n");
	char szDir[1024] = {0};
 	if (m_wHurryCardUser == m_wMeChairID)
 	{
		//_log("RatioxLog","我自己的催牌消息声音\n",);
 		if (m_pGameDlg->GetUserData(m_wMeChairID)->wGender == 1) //男
 		{
 			sprintf(szDir,"Res/RatiOx/Sound/man/man%d.mp3",m_wHurryWordID+1);
 		}
 		else													//女
 		{
 			sprintf(szDir,"Res/RatiOx/Sound/women/women%d.mp3",m_wHurryWordID+1);
 		}
 	}
 	else
 	{
//  		if (m_pPlayer[wViewUcardUser]->m_iPlayerIndex == 0 || m_pPlayer[wViewUcardUser]->m_iPlayerIndex == 1)
//  		{
//  			sprintf(szDir,"Res/RatiOx/Sound/man/man%d.mp3",m_wHurryWordID);
//  		} 
//  		else
//  		{
//  			sprintf(szDir,"Res/RatiOx/Sound/women/women%d.mp3",m_wHurryWordID);
//  		}
 	}
 
 	m_pGameApp->m_pSoundPlayer->PlaySound(szDir);

	m_pTimer->SetTimer(IDI_SHOW_HURRY_TAG, 4000);

	return true;
}

void CGameView::DrawOtherInfo( Sexy::Graphics *g )
{

	for(int i=0; i<GAME_PLAYER;i++)
	{
		const tagUserData *pUserData = m_pGameDlg->GetUserData(i);
		if (pUserData == NULL) continue; 
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		if (wChair==2) continue;
 		if (m_iOtherInfoTrans[wChair]<=0)continue;
 		g->SetColor(Sexy::Color(255, 255, 255, m_iOtherInfoTrans[wChair]));//设置透明度
		g->SetColorizeImages(true);
		g->DrawImage(IMG_RATIOX_OtherInfo,m_iOtherInfofPos[wChair][0],m_iOtherInfofPos[wChair][1]);
		//昵称
		g->SetFont(m_pFont9WeiRuanYaHei);
		g->SetColor(Sexy::Color(186,167,58));
		_bstr_t bstr = pUserData->szUserAddress;
		wchar_t pname[NAME_LEN];
		StrCpyW(pname, (wchar_t *)bstr);
		g->WriteString(pname, m_iOtherInfofPos[wChair][0]+32, m_iOtherInfofPos[wChair][1]+54, 100,0);
		//等级
		_bstr_t m_grade = szOrder[GetUserOrder(pUserData->lExperience)];
		wchar_t* pwcharDJ = (wchar_t*)m_grade;
		g->WriteString(pwcharDJ, m_iOtherInfofPos[wChair][0]+31, m_iOtherInfofPos[wChair][1]+21 ,100,0);
		//称号
		WORD wIndex = GetDescription((pUserData->lWinCount+pUserData->lLostCount));
		g->SetColor(cDesColor[GetDesColor(wIndex)]);
		_bstr_t m_description= szDescription[wIndex];
		wchar_t* pwcharDes = (wchar_t*)m_description;
		// 	g->WriteString(pwcharDJ, 0+82, 70+20, 100,0);
		g->WriteString(pwcharDes, m_iOtherInfofPos[wChair][0]+32, m_iOtherInfofPos[wChair][1]+38, 100,0);

// 		SexyString str = 
// 		g->DrawString()
	}
	g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
}
//获得称号
WORD CGameView::GetDescription( LONGLONG lBuffer )
{
	LONG lOrder[12] = {
		100,		200,		400,		800,	2000,
		4000,	6000,	8000,	20000,	40000,
		60000,	100000
	};
	for(int i=0; i<12; i++)
	{
		if(lBuffer < lOrder[i])
		{
			return i;
		}
	}

	return 12;
}

WORD CGameView::GetDesColor( WORD wIndex )
{
	if (wIndex <5)
	{
		return 0;
	}
	else if(wIndex <9)
	{
		return 1;
	}
	else if (wIndex <12)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}
//画摊牌状态
void CGameView::DrawOpenCard( Sexy::Graphics *g )
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE)continue;
		if (m_pGameDlg->GetUserData(i) == NULL) continue;
		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		if(m_bOpenCard[i])
		{
			g->DrawImage(IMG_RATIOX_OpenCardTag, cpOpenCardTag[wChair].x, cpOpenCardTag[wChair].y);
		}
	}
}

bool CGameView::OnSubOpenCard( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Open_Card));
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;

	CMD_S_Open_Card * pOpenCard = (CMD_S_Open_Card*)pBuffer;
	if (pOpenCard->bOpen ==1)
	{
		m_bOpenCard[pOpenCard->wPlayerID] = true;
	}

	return true;
}

void CGameView::ShowMoneyAni( LONGLONG lScore[] )
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPalyStatus[i] == FALSE) continue;
		if (m_pGameDlg->GetUserData(i) == NULL) continue;

		WORD wChair = m_pGameDlg->SwitchViewChairID(i);
		if (lScore[i] >= 0)
		{
			m_pAddMoneyAni[i]->Init(IMG_ANI_AddMoney);
			m_pAddMoneyAni[i]->ShowAnimate(Sexy::Point(cpMoneyAni[wChair].x-10,cpMoneyAni[wChair].y-120),Sexy::Point(cpMoneyAni[wChair].x-10,cpMoneyAni[wChair].y-120),40);

		}
		else
		{
			m_pAddMoneyAni[i]->Init(IMG_ANI_SubMoney);
			m_pAddMoneyAni[i]->ShowAnimate(Sexy::Point(cpMoneyAni[wChair].x-15,cpMoneyAni[wChair].y-150),Sexy::Point(cpMoneyAni[wChair].x-15,cpMoneyAni[wChair].y-150),35);
		}
	}
}

bool CGameView::OnSubGameFree( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	/*AfxMessageBox("");*/
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;
	//OperaTimer(IDI_START_TIMER, 0 ,0, false);
	//TODO:显示游戏等待或者配桌
	WORD wUserCount = 0;		//玩家人数
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_pGameDlg->GetUserData(i) != NULL)
		{
			wUserCount++;
		}
	}
	if (wUserCount < 2)
	{
		/*_log("RatioxLog","玩家人数小于2");*/
		if (m_pGameDlg->GetUserData(m_wMeChairID) == NULL) return true;
		if (US_READY == m_pGameDlg->GetUserData(m_wMeChairID)->cbUserStatus)
		{		
			/*_log("RatioxLog","本机玩家准备好了");*/
			OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
			ShowAutoDeskAni();
			m_pTimer->SetTimer(IDI_AUTO_CHANGE_DESK, 5000);
		}
		else
		{
			/*_log("RatioxLog","本机玩家没准备好");*/
		}
	}
	else
	{
		/*_log("RatioxLog","玩家人数大于2");*/
		if (m_pGameDlg->GetUserData(m_wMeChairID) == NULL) return true;
		//TODO:等待其它玩家
		if (US_READY == m_pGameDlg->GetUserData(m_wMeChairID)->cbUserStatus)
		{
			/*_log("RatioxLog","本机玩家准备好l了");*/
			//取消自动换桌
			OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
			m_pTimer->KillTimer(IDI_AUTO_CHANGE_DESK);
		}
		else
		{
			/*_log("RatioxLog","本机玩家没准备好");*/
		}

	}
	return true;

}

void CGameView::OnUserEnter( tagUserData * pUserData, WORD wViewChairID, bool bLookonUser )
{
	m_pPlayer[wViewChairID]->SetPlayerSex(1);
	m_pPlayer[wViewChairID]->SetStatus(ZHENG_CHANG);  
	/*_log("RatioxLog", "用户进入");*/
}
//画聚宝盆信息
void CGameView::DrawBuffer( Sexy::Graphics *g )
{
	g->SetColor(Sexy::Color(255, 255, 255,m_iTransBowlHint));
	g->SetColorizeImages(true);
	g->DrawImage(IMG_RATIOX_BowlHint,380,40);
	g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明

	g->DrawImage(IMG_RATIOX_BufferBK,cpIngot.x,0);
	int iNum = m_lBowl%10;
	DrawNum(g,IMG_NUM_IngotNum,iNum, cpIngot.x+329,12);
	iNum = m_lBowl/10%10;
	DrawNum(g,IMG_NUM_IngotNum,iNum, cpIngot.x+307,12);
	iNum =m_lBowl/100%10;
	DrawNum(g,IMG_NUM_IngotNum,iNum, cpIngot.x+285,12);
	iNum =m_lBowl/1000%10;
	DrawNum(g,IMG_NUM_IngotNum,iNum, cpIngot.x+263,12);
	iNum = m_lBowl/10000%10;
	DrawNum(g,IMG_NUM_IngotNum,iNum, cpIngot.x+241,12);

}

bool CGameView::OnSubBowl( const void * pBuffer, WORD wDataSize )
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Send_Bowl));
	if (wDataSize!=sizeof(CMD_S_Send_Bowl)) return false;
	CMD_S_Send_Bowl* pSendBowl = (CMD_S_Send_Bowl*)pBuffer;

	if (pSendBowl->cbBowlType == INVALID_BYTE)
	{
		//播放喜金动画
		m_pLikeIngotAni->Init(IMG_ANI_LikeIngotAni);
		m_pLikeIngotAni->SetLoopNum(1);
		m_pLikeIngotAni->SetEndHide(true);
		m_pLikeIngotAni->ShowAnimate(Sexy::Point(350+10,160+30),Sexy::Point(350+10,160+30),50);
	}
	else
	{
// 		if(m_wMeChairID < GAME_PLAYER)
// 		{
// 			if(pSendBowl->dwUserID == m_pGameDlg->GetUserData(m_wMeChairID)->dwUserID)
// 			{
// 				//播放获得聚宝盆动画
// 				m_pJackpotwin->Init(IMG_ANI_jackpotwin);
// 				m_pJackpotwin->SetLoopNum(1);
// 				m_pJackpotwin->SetEndHide(true);
// 				m_pJackpotwin->ShowAnimate(Sexy::Point(399, 258),Sexy::Point(399, 258),100);
// 			}
// 			else
// 			{
// 				//播放财神路过动画
// 				m_pJackocur->Init(IMG_ANI_jackocur);
// 				m_pJackocur->SetLoopNum(10);
// 				m_pJackocur->SetEndHide(true);
// 				m_pJackocur->ShowAnimate(Sexy::Point(310, 370),Sexy::Point(310, 370),100);
// 			}
// 		}
// 		m_lBowl = pSendBowl->lNewIngot;
	}


	return true;
}

void CGameView::MoveButton( int iBtnTag, int iUp )
{
	ASSERT(iBtnTag==0 || iBtnTag ==1);
	if (iBtnTag !=0 && iBtnTag != 1) return;

	if (iUp == 1)
	{
		if (iBtnTag == 0)	//托管
		{
			m_pBtnTuoGuan->Resize(cpTuoGuanDown.x,cpTuoGuanDown.y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
		}
		else
		{
			m_pBtnCancelTuoGuan->Resize(cpTuoGuanDown.x,cpTuoGuanDown.y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
		}
		m_pGameTimer->SetTimer(IDI_TIMER_BTN_UP,50,iBtnTag, 8);
	}
	else if(iUp == 0)
	{
		if (iBtnTag == 0)	//托管
		{
			m_pBtnTuoGuan->Resize(cpTuoGuanUp.x,cpTuoGuanUp.y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
		}
		else
		{
			m_pBtnCancelTuoGuan->Resize(cpTuoGuanUp.x,cpTuoGuanUp.y,IMG_BTN_BtnTrustee->GetWidth()/4, IMG_BTN_BtnTrustee->GetHeight());
		}
		m_pGameTimer->SetTimer(IDI_TIMER_BTN_DOWN,50,iBtnTag, 8);
	}
}

void CGameView::AfterReady()
{
	WORD wUserCount = 0;		//玩家人数
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_pGameDlg->GetUserData(i) != NULL)
		{
			wUserCount++;
		}
	}
	if (wUserCount<2)
	{
		OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
		ShowAutoDeskAni();
		m_pTimer->SetTimer(IDI_AUTO_CHANGE_DESK, 5000);
	}
	else
	{
		//TODO:等待其它玩家
		OperaTimer(IDI_START_TIMER, 0,0 , FALSE);
		m_pTimer->KillTimer(IDI_AUTO_CHANGE_DESK);
	}
}
//画喜金牌
void CGameView::DrawReward( Sexy::Graphics *g )
{
	
	g->DrawImage(IMG_RATIOX_LikeIngotBkDown,cpLikeIngot.x+m_cpRewardCard.x,cpLikeIngot.y+m_cpRewardCard.y);
	int x = cpLikeIngot.x+m_cpRewardCard.x;
	int y = cpLikeIngot.y+m_cpRewardCard.y;
	//画喜金牌

	for (int i=0; i<MAX_COUNT; i++)
	{
		g->DrawImageCel(IMG_RATIOX_ResultCard,x+47+i*40, y+5, GetVaule(m_cbRewardCard[m_wMeChairID][i]),GetType(m_cbRewardCard[m_wMeChairID][i])%4);
		if (m_bShowSameTag[i])
		{
			g->DrawImage(IMG_RATIOX_SameCardTag,x+44+i*40, y+2);
		}
	}


	g->DrawImage(IMG_RATIOX_LikeIngotBkUp, cpLikeIngot.x,cpLikeIngot.y);
	if (m_wServerType == GAME_GENRE_SCORE_CHIP)
	{
		g->DrawImage(IMG_RATIOX_TwoSameTxt, cpLikeIngot.x+49,cpLikeIngot.y+9);
	}
	else
	{
		g->DrawImage(IMG_RATIOX_ThreeSameTxt, cpLikeIngot.x+49,cpLikeIngot.y+9);
	}


// 	g->SetColor(Sexy::Color(255,255,0));
// 	g->SetFont(m_pFont9WeiRuanYaHei);
// 	for (int i=0; i<MAX_COUNT; i++)
// 	{
// 		if(m_cbRewardCard[m_wMeChairID][i] == 0) continue;
// 		for (int j=0; j<GAME_PLAYER; j++)
// 		{
// 			if (m_cbRewardCard[m_wMeChairID][i] == m_cbHandCard[m_wMeChairID][j])
// 			{
// 				g->SetColor(Sexy::Color(255,0,0));
// 				break;
// 			}
// 		}
// 
// 		SexyString str;
// 		_bstr_t bstr = cn(m_cbRewardCard[m_wMeChairID][i]);
// 		wchar_t pname[NAME_LEN];
// 		StrCpyW(pname, (wchar_t *)bstr);
// 		bstr = vn(m_cbRewardCard[m_wMeChairID][i]);
// 		StrCatW(pname,(wchar_t *)bstr);
// 		/*str = Sexy::StrFormat(_S("%s%s"), ,);*/
// 		g->DrawString(pname,20+i*45, 30);
// 		g->SetColor(Sexy::Color(255,255,0));
// 	}
}

void CGameView::DrawRewardHint( Sexy::Graphics *g )
{
	//g->DrawImage(m_pLikeIngotImg[0], m_x, m_y);

// 	g->SetColor(Sexy::Color(255, 255, 255,0));
// 	g->SetColorizeImages(true);

	g->SetColor(Sexy::Color(255, 255, 255,m_iTransLikeIngotHing));
	g->SetColorizeImages(true);
	g->SetFont(m_pFont10WeiRuanYaHei);
	
	SexyString str;
	if (!m_bShowRewardCard)
	{

		if (m_wServerType == GAME_GENRE_SCORE_CHIP)//灵珠
		{
			g->DrawImage(IMG_RATIOX_LikeIngotHint2, cpLikeIngot.x+42,cpLikeIngot.y+41);
			
			//g->DrawImage(m_pLikeIngotImg[0],cpLikeIngot.x+42+103,cpLikeIngot.y+41+0);
			//g->DrawImage(m_pLikeIngotImg[1],cpLikeIngot.x+42+246,cpLikeIngot.y+41+0);
			//g->DrawImage(m_pLikeIngotImg[2],cpLikeIngot.x+42+103,cpLikeIngot.y+41+17);
			//g->DrawImage(m_pLikeIngotImg[3],cpLikeIngot.x+42+246,cpLikeIngot.y+41+17);

 			if(m_iTransLikeIngotHing>50)
 			{
				g->SetColor(Sexy::Color(186,167,58));
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[0]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+41+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[1]);
 				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+41+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[2]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+41+31,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[3]);
 				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+41+31,28,0);
 			}


		}
		else						//欢乐豆
		{
			g->DrawImage(IMG_RATIOX_LikeIngotHint1, cpLikeIngot.x+42,cpLikeIngot.y+41);
			/*g->SetColor(Sexy::Color(186,167,58));*/
// 			g->DrawImage(m_pLikeIngotImg[1],cpLikeIngot.x+42+103,cpLikeIngot.y+41+0);
// 			g->DrawImage(m_pLikeIngotImg[2],cpLikeIngot.x+42+246,cpLikeIngot.y+41+0);
// 			g->DrawImage(m_pLikeIngotImg[3],cpLikeIngot.x+42+103,cpLikeIngot.y+41+17);
 			if(m_iTransLikeIngotHing>50)
 			{
 				g->SetColor(Sexy::Color(186,167,58));
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[1]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+41+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[2]);
 				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+41+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[3]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+41+31,28,0);
 			}
		}		
	}
	else
	{
		if (m_wServerType == GAME_GENRE_SCORE_CHIP)//灵珠
		{
			g->DrawImage(IMG_RATIOX_LikeIngotHint2, cpLikeIngot.x+42,cpLikeIngot.y+102);
			g->SetColor(Sexy::Color(186,167,58));
			//g->DrawImage(m_pLikeIngotImg[0],cpLikeIngot.x+42+103,cpLikeIngot.y+102+0);
			//g->DrawImage(m_pLikeIngotImg[1],cpLikeIngot.x+42+246,cpLikeIngot.y+102+0);
			//g->DrawImage(m_pLikeIngotImg[2],cpLikeIngot.x+42+103,cpLikeIngot.y+102+17);
			//g->DrawImage(m_pLikeIngotImg[3],cpLikeIngot.x+42+246,cpLikeIngot.y+102+17);

 			if(m_iTransLikeIngotHing>50)
 			{
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[0]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+102+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[1]);
 				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+102+15,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[2]);
 				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+102+31,28,0);
 				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[3]);
 				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+102+31,28,0);
 			}
		}
		else						//欢乐豆
		{
			g->DrawImage(IMG_RATIOX_LikeIngotHint1, cpLikeIngot.x+42,cpLikeIngot.y+102);
// 			g->DrawImage(m_pLikeIngotImg[1],cpLikeIngot.x+42+103,cpLikeIngot.y+102+0);
// 			g->DrawImage(m_pLikeIngotImg[2],cpLikeIngot.x+42+246,cpLikeIngot.y+102+0);
// 			g->DrawImage(m_pLikeIngotImg[3],cpLikeIngot.x+42+103,cpLikeIngot.y+102+17);

			if(m_iTransLikeIngotHing>50)
			{
				g->SetColor(Sexy::Color(186,167,58));
				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[1]);
				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+102+15,28,0);
				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[2]);
				g->WriteString(str,cpLikeIngot.x+42+233,cpLikeIngot.y+102+15,28,0);
				str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[3]);
				g->WriteString(str,cpLikeIngot.x+42+90,cpLikeIngot.y+102+31,28,0);
			}
		}		
	}

	g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
}

void CGameView::MakeLikeIngotImg()
{
	SexyString str;
	for (int i=0; i<4; i++)
	{
		str = Sexy::StrFormat(_S("%d"), m_lNewLikeIngot[i]);
		int iWidth = m_pFont10WeiRuanYaHei->StringWidth(str);
		int iHeight = m_pFont10WeiRuanYaHei->GetHeight()+5;
		m_pLikeIngotImg[i]->Create(iWidth, iHeight);
		Sexy::Rect rect(0,0,iWidth, iHeight);
		//m_pLikeIngotImg[i]->NormalFillRect(rect,Sexy::Color(255,255,255));
		m_pLikeIngotImg[i]->SetImageMode(true, false);
	
		Sexy::Graphics imgG(m_pLikeIngotImg[i]);
		imgG.ClearRect(0, 0, iWidth, iHeight);
		imgG.SetFont(m_pFont10WeiRuanYaHei);
		imgG.SetColor(Sexy::Color(186,167,58));
		imgG.DrawString(str, 0, m_pFont10WeiRuanYaHei->GetAscent());
		/*imgG.SetColor(Sexy::Color(255,255,255));*/
	}
}





