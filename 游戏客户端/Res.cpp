#include "Res.h"
#include "ResourceManager.h"

using namespace Sexy;

#pragma warning(disable:4311 4312)

static bool gNeedRecalcVariableToIdMap = false;

bool Sexy::ExtractResourcesByName(ResourceManager *theManager, const char *theName)
{
	if (strcmp(theName,"Init")==0) return ExtractInitResources(theManager);
	return false;
}

Sexy::ResourceId Sexy::GetIdByStringId(const char *theStringId)
{
	typedef std::map<std::string,int> MyMap;
	static MyMap aMap;
	if(aMap.empty())
	{
		for(int i=0; i<RESOURCE_ID_MAX; i++)
			aMap[GetStringIdById(i)] = i;
	}

	MyMap::iterator anItr = aMap.find(theStringId);
	if (anItr == aMap.end())
		return RESOURCE_ID_MAX;
	else
		return (ResourceId) anItr->second;
}

// Init Resources
Image* Sexy::IMG_ANI_AddMoney;
Image* Sexy::IMG_ANI_AutoDesk;
Image* Sexy::IMG_ANI_LikeIngotAni;
Image* Sexy::IMG_ANI_OffLineAni;
Image* Sexy::IMG_ANI_PLAYER00_Status0;
Image* Sexy::IMG_ANI_PLAYER00_Status1;
Image* Sexy::IMG_ANI_PLAYER00_Status2;
Image* Sexy::IMG_ANI_PLAYER00_Status3;
Image* Sexy::IMG_ANI_PLAYER01_Status0;
Image* Sexy::IMG_ANI_PLAYER01_Status1;
Image* Sexy::IMG_ANI_PLAYER01_Status2;
Image* Sexy::IMG_ANI_PLAYER01_Status3;
Image* Sexy::IMG_ANI_PLAYER02_Status0;
Image* Sexy::IMG_ANI_PLAYER02_Status1;
Image* Sexy::IMG_ANI_PLAYER02_Status2;
Image* Sexy::IMG_ANI_PLAYER02_Status3;
Image* Sexy::IMG_ANI_PLAYER03_Status0;
Image* Sexy::IMG_ANI_PLAYER03_Status1;
Image* Sexy::IMG_ANI_PLAYER03_Status2;
Image* Sexy::IMG_ANI_PLAYER03_Status3;
Image* Sexy::IMG_ANI_PLAYER04_Status0;
Image* Sexy::IMG_ANI_PLAYER04_Status1;
Image* Sexy::IMG_ANI_PLAYER04_Status2;
Image* Sexy::IMG_ANI_PLAYER04_Status3;
Image* Sexy::IMG_ANI_PLAYER05_Status0;
Image* Sexy::IMG_ANI_PLAYER05_Status1;
Image* Sexy::IMG_ANI_PLAYER05_Status2;
Image* Sexy::IMG_ANI_PLAYER05_Status3;
Image* Sexy::IMG_ANI_PLAYER0_Status0;
Image* Sexy::IMG_ANI_PLAYER0_Status1;
Image* Sexy::IMG_ANI_PLAYER0_Status2;
Image* Sexy::IMG_ANI_PLAYER0_Status3;
Image* Sexy::IMG_ANI_PLAYER1_Status0;
Image* Sexy::IMG_ANI_PLAYER1_Status1;
Image* Sexy::IMG_ANI_PLAYER1_Status2;
Image* Sexy::IMG_ANI_PLAYER1_Status3;
Image* Sexy::IMG_ANI_PLAYER2_Status0;
Image* Sexy::IMG_ANI_PLAYER2_Status1;
Image* Sexy::IMG_ANI_PLAYER2_Status2;
Image* Sexy::IMG_ANI_PLAYER2_Status3;
Image* Sexy::IMG_ANI_PLAYER3_Status0;
Image* Sexy::IMG_ANI_PLAYER3_Status1;
Image* Sexy::IMG_ANI_PLAYER3_Status2;
Image* Sexy::IMG_ANI_PLAYER3_Status3;
Image* Sexy::IMG_ANI_PLAYER4_Status0;
Image* Sexy::IMG_ANI_PLAYER4_Status1;
Image* Sexy::IMG_ANI_PLAYER4_Status2;
Image* Sexy::IMG_ANI_PLAYER4_Status3;
Image* Sexy::IMG_ANI_PLAYER5_Status0;
Image* Sexy::IMG_ANI_PLAYER5_Status1;
Image* Sexy::IMG_ANI_PLAYER5_Status2;
Image* Sexy::IMG_ANI_PLAYER5_Status3;
Image* Sexy::IMG_ANI_SubMoney;
Image* Sexy::IMG_ANI_TuoGuanAni;
Image* Sexy::IMG_ANI_WaitOhter;
Image* Sexy::IMG_BTN_BtnAddChip;
Image* Sexy::IMG_BTN_BtnBuyChip;
Image* Sexy::IMG_BTN_BtnCancelTrustee;
Image* Sexy::IMG_BTN_BtnChangeDesk;
Image* Sexy::IMG_BTN_BtnContinue;
Image* Sexy::IMG_BTN_BtnExit;
Image* Sexy::IMG_BTN_BtnHint;
Image* Sexy::IMG_BTN_BtnOpenCard;
Image* Sexy::IMG_BTN_BtnQuickSub;
Image* Sexy::IMG_BTN_BtnReList;
Image* Sexy::IMG_BTN_BtnTrustee;
Image* Sexy::IMG_MYANI_MyStatus0;
Image* Sexy::IMG_MYANI_MyStatus1;
Image* Sexy::IMG_MYANI_MyStatus2;
Image* Sexy::IMG_MYANI_MyStatus3;
Image* Sexy::IMG_NUM_AddScore;
Image* Sexy::IMG_NUM_ExChangeNum;
Image* Sexy::IMG_NUM_FailNum;
Image* Sexy::IMG_NUM_IngotNum;
Image* Sexy::IMG_NUM_SignAdd;
Image* Sexy::IMG_NUM_SignMinus;
Image* Sexy::IMG_NUM_TimeNum;
Image* Sexy::IMG_NUM_WinNum;
Image* Sexy::IMG_RATIOX_BACK;
Image* Sexy::IMG_RATIOX_BigCardType;
Image* Sexy::IMG_RATIOX_BigChip0;
Image* Sexy::IMG_RATIOX_BigChip1;
Image* Sexy::IMG_RATIOX_BigChip2;
Image* Sexy::IMG_RATIOX_BigChip3;
Image* Sexy::IMG_RATIOX_BigShadow;
Image* Sexy::IMG_RATIOX_BowlHint;
Image* Sexy::IMG_RATIOX_BtnBack;
Image* Sexy::IMG_RATIOX_BufferBK;
Image* Sexy::IMG_RATIOX_Card0;
Image* Sexy::IMG_RATIOX_Card1;
Image* Sexy::IMG_RATIOX_Card3;
Image* Sexy::IMG_RATIOX_Card4;
Image* Sexy::IMG_RATIOX_CardBack0;
Image* Sexy::IMG_RATIOX_CardBack1;
Image* Sexy::IMG_RATIOX_CardBack3;
Image* Sexy::IMG_RATIOX_CardBack4;
Image* Sexy::IMG_RATIOX_CardBackNormal;
Image* Sexy::IMG_RATIOX_CardNormal;
Image* Sexy::IMG_RATIOX_CellScoreBack;
Image* Sexy::IMG_RATIOX_EndBtnBack;
Image* Sexy::IMG_RATIOX_ExChipBack;
Image* Sexy::IMG_RATIOX_HurryTag;
Image* Sexy::IMG_RATIOX_LikeIngotBkDown;
Image* Sexy::IMG_RATIOX_LikeIngotBkUp;
Image* Sexy::IMG_RATIOX_LikeIngotHint1;
Image* Sexy::IMG_RATIOX_LikeIngotHint2;
Image* Sexy::IMG_RATIOX_MainMsg;
Image* Sexy::IMG_RATIOX_OpenCardTag;
Image* Sexy::IMG_RATIOX_OtherInfo;
Image* Sexy::IMG_RATIOX_Ready;
Image* Sexy::IMG_RATIOX_ResultCard;
Image* Sexy::IMG_RATIOX_ResultCardType;
Image* Sexy::IMG_RATIOX_SameCardTag;
Image* Sexy::IMG_RATIOX_ShadowBG;
Image* Sexy::IMG_RATIOX_SmallChip0;
Image* Sexy::IMG_RATIOX_SmallChip1;
Image* Sexy::IMG_RATIOX_SmallChip2;
Image* Sexy::IMG_RATIOX_SmallChip3;
Image* Sexy::IMG_RATIOX_SmallShadow;
Image* Sexy::IMG_RATIOX_ThreeSameTxt;
Image* Sexy::IMG_RATIOX_TimeBack;
Image* Sexy::IMG_RATIOX_TwoSameTxt;
Image* Sexy::IMG_RATIOX_UcCardBack;
Image* Sexy::IMG_RATIOX_UserChip;
Image* Sexy::IMG_RATIOX_UserInfo;
Image* Sexy::IMG_RATIOX_UserInfoLingZhu;
Image* Sexy::IMG_TOP_BULL_BG;
Image* Sexy::IMG_TOP_BULL_CLOSE;

bool Sexy::ExtractInitResources(ResourceManager *theManager)
{
	gNeedRecalcVariableToIdMap = true;

	ResourceManager &aMgr = *theManager;
	try
	{
		IMG_ANI_AddMoney = aMgr.GetImageThrow("IMG_ANI_AddMoney");
		IMG_ANI_AutoDesk = aMgr.GetImageThrow("IMG_ANI_AutoDesk");
		IMG_ANI_LikeIngotAni = aMgr.GetImageThrow("IMG_ANI_LikeIngotAni");
		IMG_ANI_OffLineAni = aMgr.GetImageThrow("IMG_ANI_OffLineAni");
		IMG_ANI_PLAYER00_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER00_Status0");
		IMG_ANI_PLAYER00_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER00_Status1");
		IMG_ANI_PLAYER00_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER00_Status2");
		IMG_ANI_PLAYER00_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER00_Status3");
		IMG_ANI_PLAYER01_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER01_Status0");
		IMG_ANI_PLAYER01_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER01_Status1");
		IMG_ANI_PLAYER01_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER01_Status2");
		IMG_ANI_PLAYER01_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER01_Status3");
		IMG_ANI_PLAYER02_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER02_Status0");
		IMG_ANI_PLAYER02_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER02_Status1");
		IMG_ANI_PLAYER02_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER02_Status2");
		IMG_ANI_PLAYER02_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER02_Status3");
		IMG_ANI_PLAYER03_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER03_Status0");
		IMG_ANI_PLAYER03_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER03_Status1");
		IMG_ANI_PLAYER03_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER03_Status2");
		IMG_ANI_PLAYER03_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER03_Status3");
		IMG_ANI_PLAYER04_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER04_Status0");
		IMG_ANI_PLAYER04_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER04_Status1");
		IMG_ANI_PLAYER04_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER04_Status2");
		IMG_ANI_PLAYER04_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER04_Status3");
		IMG_ANI_PLAYER05_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER05_Status0");
		IMG_ANI_PLAYER05_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER05_Status1");
		IMG_ANI_PLAYER05_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER05_Status2");
		IMG_ANI_PLAYER05_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER05_Status3");
		IMG_ANI_PLAYER0_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER0_Status0");
		IMG_ANI_PLAYER0_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER0_Status1");
		IMG_ANI_PLAYER0_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER0_Status2");
		IMG_ANI_PLAYER0_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER0_Status3");
		IMG_ANI_PLAYER1_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER1_Status0");
		IMG_ANI_PLAYER1_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER1_Status1");
		IMG_ANI_PLAYER1_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER1_Status2");
		IMG_ANI_PLAYER1_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER1_Status3");
		IMG_ANI_PLAYER2_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER2_Status0");
		IMG_ANI_PLAYER2_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER2_Status1");
		IMG_ANI_PLAYER2_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER2_Status2");
		IMG_ANI_PLAYER2_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER2_Status3");
		IMG_ANI_PLAYER3_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER3_Status0");
		IMG_ANI_PLAYER3_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER3_Status1");
		IMG_ANI_PLAYER3_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER3_Status2");
		IMG_ANI_PLAYER3_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER3_Status3");
		IMG_ANI_PLAYER4_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER4_Status0");
		IMG_ANI_PLAYER4_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER4_Status1");
		IMG_ANI_PLAYER4_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER4_Status2");
		IMG_ANI_PLAYER4_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER4_Status3");
		IMG_ANI_PLAYER5_Status0 = aMgr.GetImageThrow("IMG_ANI_PLAYER5_Status0");
		IMG_ANI_PLAYER5_Status1 = aMgr.GetImageThrow("IMG_ANI_PLAYER5_Status1");
		IMG_ANI_PLAYER5_Status2 = aMgr.GetImageThrow("IMG_ANI_PLAYER5_Status2");
		IMG_ANI_PLAYER5_Status3 = aMgr.GetImageThrow("IMG_ANI_PLAYER5_Status3");
		IMG_ANI_SubMoney = aMgr.GetImageThrow("IMG_ANI_SubMoney");
		IMG_ANI_TuoGuanAni = aMgr.GetImageThrow("IMG_ANI_TuoGuanAni");
		IMG_ANI_WaitOhter = aMgr.GetImageThrow("IMG_ANI_WaitOhter");
		IMG_BTN_BtnAddChip = aMgr.GetImageThrow("IMG_BTN_BtnAddChip");
		IMG_BTN_BtnBuyChip = aMgr.GetImageThrow("IMG_BTN_BtnBuyChip");
		IMG_BTN_BtnCancelTrustee = aMgr.GetImageThrow("IMG_BTN_BtnCancelTrustee");
		IMG_BTN_BtnChangeDesk = aMgr.GetImageThrow("IMG_BTN_BtnChangeDesk");
		IMG_BTN_BtnContinue = aMgr.GetImageThrow("IMG_BTN_BtnContinue");
		IMG_BTN_BtnExit = aMgr.GetImageThrow("IMG_BTN_BtnExit");
		IMG_BTN_BtnHint = aMgr.GetImageThrow("IMG_BTN_BtnHint");
		IMG_BTN_BtnOpenCard = aMgr.GetImageThrow("IMG_BTN_BtnOpenCard");
		IMG_BTN_BtnQuickSub = aMgr.GetImageThrow("IMG_BTN_BtnQuickSub");
		IMG_BTN_BtnReList = aMgr.GetImageThrow("IMG_BTN_BtnReList");
		IMG_BTN_BtnTrustee = aMgr.GetImageThrow("IMG_BTN_BtnTrustee");
		IMG_MYANI_MyStatus0 = aMgr.GetImageThrow("IMG_MYANI_MyStatus0");
		IMG_MYANI_MyStatus1 = aMgr.GetImageThrow("IMG_MYANI_MyStatus1");
		IMG_MYANI_MyStatus2 = aMgr.GetImageThrow("IMG_MYANI_MyStatus2");
		IMG_MYANI_MyStatus3 = aMgr.GetImageThrow("IMG_MYANI_MyStatus3");
		IMG_NUM_AddScore = aMgr.GetImageThrow("IMG_NUM_AddScore");
		IMG_NUM_ExChangeNum = aMgr.GetImageThrow("IMG_NUM_ExChangeNum");
		IMG_NUM_FailNum = aMgr.GetImageThrow("IMG_NUM_FailNum");
		IMG_NUM_IngotNum = aMgr.GetImageThrow("IMG_NUM_IngotNum");
		IMG_NUM_SignAdd = aMgr.GetImageThrow("IMG_NUM_SignAdd");
		IMG_NUM_SignMinus = aMgr.GetImageThrow("IMG_NUM_SignMinus");
		IMG_NUM_TimeNum = aMgr.GetImageThrow("IMG_NUM_TimeNum");
		IMG_NUM_WinNum = aMgr.GetImageThrow("IMG_NUM_WinNum");
		IMG_RATIOX_BACK = aMgr.GetImageThrow("IMG_RATIOX_BACK");
		IMG_RATIOX_BigCardType = aMgr.GetImageThrow("IMG_RATIOX_BigCardType");
		IMG_RATIOX_BigChip0 = aMgr.GetImageThrow("IMG_RATIOX_BigChip0");
		IMG_RATIOX_BigChip1 = aMgr.GetImageThrow("IMG_RATIOX_BigChip1");
		IMG_RATIOX_BigChip2 = aMgr.GetImageThrow("IMG_RATIOX_BigChip2");
		IMG_RATIOX_BigChip3 = aMgr.GetImageThrow("IMG_RATIOX_BigChip3");
		IMG_RATIOX_BigShadow = aMgr.GetImageThrow("IMG_RATIOX_BigShadow");
		IMG_RATIOX_BowlHint = aMgr.GetImageThrow("IMG_RATIOX_BowlHint");
		IMG_RATIOX_BtnBack = aMgr.GetImageThrow("IMG_RATIOX_BtnBack");
		IMG_RATIOX_BufferBK = aMgr.GetImageThrow("IMG_RATIOX_BufferBK");
		IMG_RATIOX_Card0 = aMgr.GetImageThrow("IMG_RATIOX_Card0");
		IMG_RATIOX_Card1 = aMgr.GetImageThrow("IMG_RATIOX_Card1");
		IMG_RATIOX_Card3 = aMgr.GetImageThrow("IMG_RATIOX_Card3");
		IMG_RATIOX_Card4 = aMgr.GetImageThrow("IMG_RATIOX_Card4");
		IMG_RATIOX_CardBack0 = aMgr.GetImageThrow("IMG_RATIOX_CardBack0");
		IMG_RATIOX_CardBack1 = aMgr.GetImageThrow("IMG_RATIOX_CardBack1");
		IMG_RATIOX_CardBack3 = aMgr.GetImageThrow("IMG_RATIOX_CardBack3");
		IMG_RATIOX_CardBack4 = aMgr.GetImageThrow("IMG_RATIOX_CardBack4");
		IMG_RATIOX_CardBackNormal = aMgr.GetImageThrow("IMG_RATIOX_CardBackNormal");
		IMG_RATIOX_CardNormal = aMgr.GetImageThrow("IMG_RATIOX_CardNormal");
		IMG_RATIOX_CellScoreBack = aMgr.GetImageThrow("IMG_RATIOX_CellScoreBack");
		IMG_RATIOX_EndBtnBack = aMgr.GetImageThrow("IMG_RATIOX_EndBtnBack");
		IMG_RATIOX_ExChipBack = aMgr.GetImageThrow("IMG_RATIOX_ExChipBack");
		IMG_RATIOX_HurryTag = aMgr.GetImageThrow("IMG_RATIOX_HurryTag");
		IMG_RATIOX_LikeIngotBkDown = aMgr.GetImageThrow("IMG_RATIOX_LikeIngotBkDown");
		IMG_RATIOX_LikeIngotBkUp = aMgr.GetImageThrow("IMG_RATIOX_LikeIngotBkUp");
		IMG_RATIOX_LikeIngotHint1 = aMgr.GetImageThrow("IMG_RATIOX_LikeIngotHint1");
		IMG_RATIOX_LikeIngotHint2 = aMgr.GetImageThrow("IMG_RATIOX_LikeIngotHint2");
		IMG_RATIOX_MainMsg = aMgr.GetImageThrow("IMG_RATIOX_MainMsg");
		IMG_RATIOX_OpenCardTag = aMgr.GetImageThrow("IMG_RATIOX_OpenCardTag");
		IMG_RATIOX_OtherInfo = aMgr.GetImageThrow("IMG_RATIOX_OtherInfo");
		IMG_RATIOX_Ready = aMgr.GetImageThrow("IMG_RATIOX_Ready");
		IMG_RATIOX_ResultCard = aMgr.GetImageThrow("IMG_RATIOX_ResultCard");
		IMG_RATIOX_ResultCardType = aMgr.GetImageThrow("IMG_RATIOX_ResultCardType");
		IMG_RATIOX_SameCardTag = aMgr.GetImageThrow("IMG_RATIOX_SameCardTag");
		IMG_RATIOX_ShadowBG = aMgr.GetImageThrow("IMG_RATIOX_ShadowBG");
		IMG_RATIOX_SmallChip0 = aMgr.GetImageThrow("IMG_RATIOX_SmallChip0");
		IMG_RATIOX_SmallChip1 = aMgr.GetImageThrow("IMG_RATIOX_SmallChip1");
		IMG_RATIOX_SmallChip2 = aMgr.GetImageThrow("IMG_RATIOX_SmallChip2");
		IMG_RATIOX_SmallChip3 = aMgr.GetImageThrow("IMG_RATIOX_SmallChip3");
		IMG_RATIOX_SmallShadow = aMgr.GetImageThrow("IMG_RATIOX_SmallShadow");
		IMG_RATIOX_ThreeSameTxt = aMgr.GetImageThrow("IMG_RATIOX_ThreeSameTxt");
		IMG_RATIOX_TimeBack = aMgr.GetImageThrow("IMG_RATIOX_TimeBack");
		IMG_RATIOX_TwoSameTxt = aMgr.GetImageThrow("IMG_RATIOX_TwoSameTxt");
		IMG_RATIOX_UcCardBack = aMgr.GetImageThrow("IMG_RATIOX_UcCardBack");
		IMG_RATIOX_UserChip = aMgr.GetImageThrow("IMG_RATIOX_UserChip");
		IMG_RATIOX_UserInfo = aMgr.GetImageThrow("IMG_RATIOX_UserInfo");
		IMG_RATIOX_UserInfoLingZhu = aMgr.GetImageThrow("IMG_RATIOX_UserInfoLingZhu");
		IMG_TOP_BULL_BG = aMgr.GetImageThrow("IMG_TOP_BULL_BG");
		IMG_TOP_BULL_CLOSE = aMgr.GetImageThrow("IMG_TOP_BULL_CLOSE");
	}
	catch(ResourceManagerException&)
	{
		return false;
	}
	return true;
}

static void* gResources[] =
{
	&IMG_BTN_BtnCancelTrustee,
	&IMG_BTN_BtnChangeDesk,
	&IMG_BTN_BtnContinue,
	&IMG_BTN_BtnExit,
	&IMG_BTN_BtnOpenCard,
	&IMG_BTN_BtnTrustee,
	&IMG_BTN_BtnHint,
	&IMG_BTN_BtnReList,
	&IMG_BTN_BtnBuyChip,
	&IMG_BTN_BtnAddChip,
	&IMG_BTN_BtnQuickSub,
	&IMG_ANI_PLAYER0_Status0,
	&IMG_ANI_PLAYER0_Status1,
	&IMG_ANI_PLAYER0_Status2,
	&IMG_ANI_PLAYER0_Status3,
	&IMG_ANI_PLAYER1_Status0,
	&IMG_ANI_PLAYER1_Status1,
	&IMG_ANI_PLAYER1_Status2,
	&IMG_ANI_PLAYER1_Status3,
	&IMG_ANI_PLAYER2_Status0,
	&IMG_ANI_PLAYER2_Status1,
	&IMG_ANI_PLAYER2_Status2,
	&IMG_ANI_PLAYER2_Status3,
	&IMG_ANI_PLAYER3_Status0,
	&IMG_ANI_PLAYER3_Status1,
	&IMG_ANI_PLAYER3_Status2,
	&IMG_ANI_PLAYER3_Status3,
	&IMG_ANI_PLAYER4_Status0,
	&IMG_ANI_PLAYER4_Status1,
	&IMG_ANI_PLAYER4_Status2,
	&IMG_ANI_PLAYER4_Status3,
	&IMG_ANI_PLAYER5_Status0,
	&IMG_ANI_PLAYER5_Status1,
	&IMG_ANI_PLAYER5_Status2,
	&IMG_ANI_PLAYER5_Status3,
	&IMG_MYANI_MyStatus0,
	&IMG_MYANI_MyStatus1,
	&IMG_MYANI_MyStatus2,
	&IMG_MYANI_MyStatus3,
	&IMG_RATIOX_BACK,
	&IMG_RATIOX_UserInfo,
	&IMG_RATIOX_TimeBack,
	&IMG_RATIOX_UserChip,
	&IMG_RATIOX_CellScoreBack,
	&IMG_RATIOX_ExChipBack,
	&IMG_RATIOX_Ready,
	&IMG_RATIOX_BigShadow,
	&IMG_RATIOX_SmallShadow,
	&IMG_RATIOX_ShadowBG,
	&IMG_RATIOX_EndBtnBack,
	&IMG_RATIOX_BigChip0,
	&IMG_RATIOX_BigChip1,
	&IMG_RATIOX_BigChip2,
	&IMG_RATIOX_BigChip3,
	&IMG_RATIOX_SmallChip0,
	&IMG_RATIOX_SmallChip1,
	&IMG_RATIOX_SmallChip2,
	&IMG_RATIOX_SmallChip3,
	&IMG_RATIOX_CardBackNormal,
	&IMG_RATIOX_CardBack0,
	&IMG_RATIOX_CardBack1,
	&IMG_RATIOX_CardBack3,
	&IMG_RATIOX_CardBack4,
	&IMG_RATIOX_CardNormal,
	&IMG_RATIOX_Card0,
	&IMG_RATIOX_Card1,
	&IMG_RATIOX_Card3,
	&IMG_RATIOX_Card4,
	&IMG_RATIOX_ResultCard,
	&IMG_RATIOX_BigCardType,
	&IMG_RATIOX_UcCardBack,
	&IMG_RATIOX_MainMsg,
	&IMG_RATIOX_OtherInfo,
	&IMG_RATIOX_HurryTag,
	&IMG_RATIOX_OpenCardTag,
	&IMG_RATIOX_BufferBK,
	&IMG_RATIOX_UserInfoLingZhu,
	&IMG_RATIOX_BtnBack,
	&IMG_RATIOX_BowlHint,
	&IMG_RATIOX_LikeIngotHint1,
	&IMG_RATIOX_LikeIngotHint2,
	&IMG_RATIOX_ThreeSameTxt,
	&IMG_RATIOX_TwoSameTxt,
	&IMG_RATIOX_SameCardTag,
	&IMG_RATIOX_LikeIngotBkUp,
	&IMG_RATIOX_LikeIngotBkDown,
	&IMG_RATIOX_ResultCardType,
	&IMG_ANI_AddMoney,
	&IMG_ANI_SubMoney,
	&IMG_ANI_WaitOhter,
	&IMG_ANI_AutoDesk,
	&IMG_ANI_LikeIngotAni,
	&IMG_ANI_OffLineAni,
	&IMG_ANI_TuoGuanAni,
	&IMG_NUM_TimeNum,
	&IMG_NUM_WinNum,
	&IMG_NUM_FailNum,
	&IMG_NUM_SignAdd,
	&IMG_NUM_SignMinus,
	&IMG_NUM_AddScore,
	&IMG_NUM_ExChangeNum,
	&IMG_NUM_IngotNum,
	&IMG_TOP_BULL_BG,
	&IMG_TOP_BULL_CLOSE,
	&IMG_ANI_PLAYER00_Status0,
	&IMG_ANI_PLAYER00_Status1,
	&IMG_ANI_PLAYER00_Status2,
	&IMG_ANI_PLAYER00_Status3,
	&IMG_ANI_PLAYER01_Status0,
	&IMG_ANI_PLAYER01_Status1,
	&IMG_ANI_PLAYER01_Status2,
	&IMG_ANI_PLAYER01_Status3,
	&IMG_ANI_PLAYER02_Status0,
	&IMG_ANI_PLAYER02_Status1,
	&IMG_ANI_PLAYER02_Status2,
	&IMG_ANI_PLAYER02_Status3,
	&IMG_ANI_PLAYER03_Status0,
	&IMG_ANI_PLAYER03_Status1,
	&IMG_ANI_PLAYER03_Status2,
	&IMG_ANI_PLAYER03_Status3,
	&IMG_ANI_PLAYER04_Status0,
	&IMG_ANI_PLAYER04_Status1,
	&IMG_ANI_PLAYER04_Status2,
	&IMG_ANI_PLAYER04_Status3,
	&IMG_ANI_PLAYER05_Status0,
	&IMG_ANI_PLAYER05_Status1,
	&IMG_ANI_PLAYER05_Status2,
	&IMG_ANI_PLAYER05_Status3,
	NULL
};

Image* Sexy::LoadImageById(ResourceManager *theManager, int theId)
{
	return (*((Image**)gResources[theId]) = theManager->LoadImage(GetStringIdById(theId)));
}

void Sexy::ReplaceImageById(ResourceManager *theManager, int theId, Image *theImage)
{
	theManager->ReplaceImage(GetStringIdById(theId),theImage);
	*(Image**)gResources[theId] = theImage;
}

Image* Sexy::GetImageById(int theId)
{
	return *(Image**)gResources[theId];
}

Font* Sexy::GetFontById(int theId)
{
	return *(Font**)gResources[theId];
}

int Sexy::GetSoundById(int theId)
{
	return *(int*)gResources[theId];
}

Image*& Sexy::GetImageRefById(int theId)
{
	return *(Image**)gResources[theId];
}

Font*& Sexy::GetFontRefById(int theId)
{
	return *(Font**)gResources[theId];
}

int& Sexy::GetSoundRefById(int theId)
{
	return *(int*)gResources[theId];
}

static Sexy::ResourceId GetIdByVariable(const void *theVariable)
{
	typedef std::map<int,int> MyMap;
	static MyMap aMap;
	if(gNeedRecalcVariableToIdMap)
	{
		gNeedRecalcVariableToIdMap = false;
		aMap.clear();
		for(int i=0; i<RESOURCE_ID_MAX; i++)
			aMap[*(int*)gResources[i]] = i;
	}

	MyMap::iterator anItr = aMap.find((int)theVariable);
	if (anItr == aMap.end())
		return RESOURCE_ID_MAX;
	else
		return (ResourceId) anItr->second;
}

Sexy::ResourceId Sexy::GetIdByImage(Image *theImage)
{
	return GetIdByVariable(theImage);
}

Sexy::ResourceId Sexy::GetIdByFont(Font *theFont)
{
	return GetIdByVariable(theFont);
}

Sexy::ResourceId Sexy::GetIdBySound(int theSound)
{
	return GetIdByVariable((void*)theSound);
}

const char* Sexy::GetStringIdById(int theId)
{
	switch(theId)
	{
		case IMG_BTN_BtnCancelTrustee_ID: return "IMG_BTN_BtnCancelTrustee";
		case IMG_BTN_BtnChangeDesk_ID: return "IMG_BTN_BtnChangeDesk";
		case IMG_BTN_BtnContinue_ID: return "IMG_BTN_BtnContinue";
		case IMG_BTN_BtnExit_ID: return "IMG_BTN_BtnExit";
		case IMG_BTN_BtnOpenCard_ID: return "IMG_BTN_BtnOpenCard";
		case IMG_BTN_BtnTrustee_ID: return "IMG_BTN_BtnTrustee";
		case IMG_BTN_BtnHint_ID: return "IMG_BTN_BtnHint";
		case IMG_BTN_BtnReList_ID: return "IMG_BTN_BtnReList";
		case IMG_BTN_BtnBuyChip_ID: return "IMG_BTN_BtnBuyChip";
		case IMG_BTN_BtnAddChip_ID: return "IMG_BTN_BtnAddChip";
		case IMG_BTN_BtnQuickSub_ID: return "IMG_BTN_BtnQuickSub";
		case IMG_ANI_PLAYER0_Status0_ID: return "IMG_ANI_PLAYER0_Status0";
		case IMG_ANI_PLAYER0_Status1_ID: return "IMG_ANI_PLAYER0_Status1";
		case IMG_ANI_PLAYER0_Status2_ID: return "IMG_ANI_PLAYER0_Status2";
		case IMG_ANI_PLAYER0_Status3_ID: return "IMG_ANI_PLAYER0_Status3";
		case IMG_ANI_PLAYER1_Status0_ID: return "IMG_ANI_PLAYER1_Status0";
		case IMG_ANI_PLAYER1_Status1_ID: return "IMG_ANI_PLAYER1_Status1";
		case IMG_ANI_PLAYER1_Status2_ID: return "IMG_ANI_PLAYER1_Status2";
		case IMG_ANI_PLAYER1_Status3_ID: return "IMG_ANI_PLAYER1_Status3";
		case IMG_ANI_PLAYER2_Status0_ID: return "IMG_ANI_PLAYER2_Status0";
		case IMG_ANI_PLAYER2_Status1_ID: return "IMG_ANI_PLAYER2_Status1";
		case IMG_ANI_PLAYER2_Status2_ID: return "IMG_ANI_PLAYER2_Status2";
		case IMG_ANI_PLAYER2_Status3_ID: return "IMG_ANI_PLAYER2_Status3";
		case IMG_ANI_PLAYER3_Status0_ID: return "IMG_ANI_PLAYER3_Status0";
		case IMG_ANI_PLAYER3_Status1_ID: return "IMG_ANI_PLAYER3_Status1";
		case IMG_ANI_PLAYER3_Status2_ID: return "IMG_ANI_PLAYER3_Status2";
		case IMG_ANI_PLAYER3_Status3_ID: return "IMG_ANI_PLAYER3_Status3";
		case IMG_ANI_PLAYER4_Status0_ID: return "IMG_ANI_PLAYER4_Status0";
		case IMG_ANI_PLAYER4_Status1_ID: return "IMG_ANI_PLAYER4_Status1";
		case IMG_ANI_PLAYER4_Status2_ID: return "IMG_ANI_PLAYER4_Status2";
		case IMG_ANI_PLAYER4_Status3_ID: return "IMG_ANI_PLAYER4_Status3";
		case IMG_ANI_PLAYER5_Status0_ID: return "IMG_ANI_PLAYER5_Status0";
		case IMG_ANI_PLAYER5_Status1_ID: return "IMG_ANI_PLAYER5_Status1";
		case IMG_ANI_PLAYER5_Status2_ID: return "IMG_ANI_PLAYER5_Status2";
		case IMG_ANI_PLAYER5_Status3_ID: return "IMG_ANI_PLAYER5_Status3";
		case IMG_MYANI_MyStatus0_ID: return "IMG_MYANI_MyStatus0";
		case IMG_MYANI_MyStatus1_ID: return "IMG_MYANI_MyStatus1";
		case IMG_MYANI_MyStatus2_ID: return "IMG_MYANI_MyStatus2";
		case IMG_MYANI_MyStatus3_ID: return "IMG_MYANI_MyStatus3";
		case IMG_RATIOX_BACK_ID: return "IMG_RATIOX_BACK";
		case IMG_RATIOX_UserInfo_ID: return "IMG_RATIOX_UserInfo";
		case IMG_RATIOX_TimeBack_ID: return "IMG_RATIOX_TimeBack";
		case IMG_RATIOX_UserChip_ID: return "IMG_RATIOX_UserChip";
		case IMG_RATIOX_CellScoreBack_ID: return "IMG_RATIOX_CellScoreBack";
		case IMG_RATIOX_ExChipBack_ID: return "IMG_RATIOX_ExChipBack";
		case IMG_RATIOX_Ready_ID: return "IMG_RATIOX_Ready";
		case IMG_RATIOX_BigShadow_ID: return "IMG_RATIOX_BigShadow";
		case IMG_RATIOX_SmallShadow_ID: return "IMG_RATIOX_SmallShadow";
		case IMG_RATIOX_ShadowBG_ID: return "IMG_RATIOX_ShadowBG";
		case IMG_RATIOX_EndBtnBack_ID: return "IMG_RATIOX_EndBtnBack";
		case IMG_RATIOX_BigChip0_ID: return "IMG_RATIOX_BigChip0";
		case IMG_RATIOX_BigChip1_ID: return "IMG_RATIOX_BigChip1";
		case IMG_RATIOX_BigChip2_ID: return "IMG_RATIOX_BigChip2";
		case IMG_RATIOX_BigChip3_ID: return "IMG_RATIOX_BigChip3";
		case IMG_RATIOX_SmallChip0_ID: return "IMG_RATIOX_SmallChip0";
		case IMG_RATIOX_SmallChip1_ID: return "IMG_RATIOX_SmallChip1";
		case IMG_RATIOX_SmallChip2_ID: return "IMG_RATIOX_SmallChip2";
		case IMG_RATIOX_SmallChip3_ID: return "IMG_RATIOX_SmallChip3";
		case IMG_RATIOX_CardBackNormal_ID: return "IMG_RATIOX_CardBackNormal";
		case IMG_RATIOX_CardBack0_ID: return "IMG_RATIOX_CardBack0";
		case IMG_RATIOX_CardBack1_ID: return "IMG_RATIOX_CardBack1";
		case IMG_RATIOX_CardBack3_ID: return "IMG_RATIOX_CardBack3";
		case IMG_RATIOX_CardBack4_ID: return "IMG_RATIOX_CardBack4";
		case IMG_RATIOX_CardNormal_ID: return "IMG_RATIOX_CardNormal";
		case IMG_RATIOX_Card0_ID: return "IMG_RATIOX_Card0";
		case IMG_RATIOX_Card1_ID: return "IMG_RATIOX_Card1";
		case IMG_RATIOX_Card3_ID: return "IMG_RATIOX_Card3";
		case IMG_RATIOX_Card4_ID: return "IMG_RATIOX_Card4";
		case IMG_RATIOX_ResultCard_ID: return "IMG_RATIOX_ResultCard";
		case IMG_RATIOX_BigCardType_ID: return "IMG_RATIOX_BigCardType";
		case IMG_RATIOX_UcCardBack_ID: return "IMG_RATIOX_UcCardBack";
		case IMG_RATIOX_MainMsg_ID: return "IMG_RATIOX_MainMsg";
		case IMG_RATIOX_OtherInfo_ID: return "IMG_RATIOX_OtherInfo";
		case IMG_RATIOX_HurryTag_ID: return "IMG_RATIOX_HurryTag";
		case IMG_RATIOX_OpenCardTag_ID: return "IMG_RATIOX_OpenCardTag";
		case IMG_RATIOX_BufferBK_ID: return "IMG_RATIOX_BufferBK";
		case IMG_RATIOX_UserInfoLingZhu_ID: return "IMG_RATIOX_UserInfoLingZhu";
		case IMG_RATIOX_BtnBack_ID: return "IMG_RATIOX_BtnBack";
		case IMG_RATIOX_BowlHint_ID: return "IMG_RATIOX_BowlHint";
		case IMG_RATIOX_LikeIngotHint1_ID: return "IMG_RATIOX_LikeIngotHint1";
		case IMG_RATIOX_LikeIngotHint2_ID: return "IMG_RATIOX_LikeIngotHint2";
		case IMG_RATIOX_ThreeSameTxt_ID: return "IMG_RATIOX_ThreeSameTxt";
		case IMG_RATIOX_TwoSameTxt_ID: return "IMG_RATIOX_TwoSameTxt";
		case IMG_RATIOX_SameCardTag_ID: return "IMG_RATIOX_SameCardTag";
		case IMG_RATIOX_LikeIngotBkUp_ID: return "IMG_RATIOX_LikeIngotBkUp";
		case IMG_RATIOX_LikeIngotBkDown_ID: return "IMG_RATIOX_LikeIngotBkDown";
		case IMG_RATIOX_ResultCardType_ID: return "IMG_RATIOX_ResultCardType";
		case IMG_ANI_AddMoney_ID: return "IMG_ANI_AddMoney";
		case IMG_ANI_SubMoney_ID: return "IMG_ANI_SubMoney";
		case IMG_ANI_WaitOhter_ID: return "IMG_ANI_WaitOhter";
		case IMG_ANI_AutoDesk_ID: return "IMG_ANI_AutoDesk";
		case IMG_ANI_LikeIngotAni_ID: return "IMG_ANI_LikeIngotAni";
		case IMG_ANI_OffLineAni_ID: return "IMG_ANI_OffLineAni";
		case IMG_ANI_TuoGuanAni_ID: return "IMG_ANI_TuoGuanAni";
		case IMG_NUM_TimeNum_ID: return "IMG_NUM_TimeNum";
		case IMG_NUM_WinNum_ID: return "IMG_NUM_WinNum";
		case IMG_NUM_FailNum_ID: return "IMG_NUM_FailNum";
		case IMG_NUM_SignAdd_ID: return "IMG_NUM_SignAdd";
		case IMG_NUM_SignMinus_ID: return "IMG_NUM_SignMinus";
		case IMG_NUM_AddScore_ID: return "IMG_NUM_AddScore";
		case IMG_NUM_ExChangeNum_ID: return "IMG_NUM_ExChangeNum";
		case IMG_NUM_IngotNum_ID: return "IMG_NUM_IngotNum";
		case IMG_TOP_BULL_BG_ID: return "IMG_TOP_BULL_BG";
		case IMG_TOP_BULL_CLOSE_ID: return "IMG_TOP_BULL_CLOSE";
		case IMG_ANI_PLAYER00_Status0_ID: return "IMG_ANI_PLAYER00_Status0";
		case IMG_ANI_PLAYER00_Status1_ID: return "IMG_ANI_PLAYER00_Status1";
		case IMG_ANI_PLAYER00_Status2_ID: return "IMG_ANI_PLAYER00_Status2";
		case IMG_ANI_PLAYER00_Status3_ID: return "IMG_ANI_PLAYER00_Status3";
		case IMG_ANI_PLAYER01_Status0_ID: return "IMG_ANI_PLAYER01_Status0";
		case IMG_ANI_PLAYER01_Status1_ID: return "IMG_ANI_PLAYER01_Status1";
		case IMG_ANI_PLAYER01_Status2_ID: return "IMG_ANI_PLAYER01_Status2";
		case IMG_ANI_PLAYER01_Status3_ID: return "IMG_ANI_PLAYER01_Status3";
		case IMG_ANI_PLAYER02_Status0_ID: return "IMG_ANI_PLAYER02_Status0";
		case IMG_ANI_PLAYER02_Status1_ID: return "IMG_ANI_PLAYER02_Status1";
		case IMG_ANI_PLAYER02_Status2_ID: return "IMG_ANI_PLAYER02_Status2";
		case IMG_ANI_PLAYER02_Status3_ID: return "IMG_ANI_PLAYER02_Status3";
		case IMG_ANI_PLAYER03_Status0_ID: return "IMG_ANI_PLAYER03_Status0";
		case IMG_ANI_PLAYER03_Status1_ID: return "IMG_ANI_PLAYER03_Status1";
		case IMG_ANI_PLAYER03_Status2_ID: return "IMG_ANI_PLAYER03_Status2";
		case IMG_ANI_PLAYER03_Status3_ID: return "IMG_ANI_PLAYER03_Status3";
		case IMG_ANI_PLAYER04_Status0_ID: return "IMG_ANI_PLAYER04_Status0";
		case IMG_ANI_PLAYER04_Status1_ID: return "IMG_ANI_PLAYER04_Status1";
		case IMG_ANI_PLAYER04_Status2_ID: return "IMG_ANI_PLAYER04_Status2";
		case IMG_ANI_PLAYER04_Status3_ID: return "IMG_ANI_PLAYER04_Status3";
		case IMG_ANI_PLAYER05_Status0_ID: return "IMG_ANI_PLAYER05_Status0";
		case IMG_ANI_PLAYER05_Status1_ID: return "IMG_ANI_PLAYER05_Status1";
		case IMG_ANI_PLAYER05_Status2_ID: return "IMG_ANI_PLAYER05_Status2";
		case IMG_ANI_PLAYER05_Status3_ID: return "IMG_ANI_PLAYER05_Status3";
		default: return "";
	}
}

