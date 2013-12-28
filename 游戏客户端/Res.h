#ifndef __Res_H__
#define __Res_H__

namespace Sexy
{
	class ResourceManager;
	class Image;
	class Font;

	Image* LoadImageById(ResourceManager *theManager, int theId);
	void ReplaceImageById(ResourceManager *theManager, int theId, Image *theImage);
	bool ExtractResourcesByName(ResourceManager *theManager, const char *theName);

	// Init Resources
	bool ExtractInitResources(ResourceManager *theMgr);
	extern Image* IMG_ANI_AddMoney;
	extern Image* IMG_ANI_AutoDesk;
	extern Image* IMG_ANI_LikeIngotAni;
	extern Image* IMG_ANI_OffLineAni;
	extern Image* IMG_ANI_PLAYER00_Status0;
	extern Image* IMG_ANI_PLAYER00_Status1;
	extern Image* IMG_ANI_PLAYER00_Status2;
	extern Image* IMG_ANI_PLAYER00_Status3;
	extern Image* IMG_ANI_PLAYER01_Status0;
	extern Image* IMG_ANI_PLAYER01_Status1;
	extern Image* IMG_ANI_PLAYER01_Status2;
	extern Image* IMG_ANI_PLAYER01_Status3;
	extern Image* IMG_ANI_PLAYER02_Status0;
	extern Image* IMG_ANI_PLAYER02_Status1;
	extern Image* IMG_ANI_PLAYER02_Status2;
	extern Image* IMG_ANI_PLAYER02_Status3;
	extern Image* IMG_ANI_PLAYER03_Status0;
	extern Image* IMG_ANI_PLAYER03_Status1;
	extern Image* IMG_ANI_PLAYER03_Status2;
	extern Image* IMG_ANI_PLAYER03_Status3;
	extern Image* IMG_ANI_PLAYER04_Status0;
	extern Image* IMG_ANI_PLAYER04_Status1;
	extern Image* IMG_ANI_PLAYER04_Status2;
	extern Image* IMG_ANI_PLAYER04_Status3;
	extern Image* IMG_ANI_PLAYER05_Status0;
	extern Image* IMG_ANI_PLAYER05_Status1;
	extern Image* IMG_ANI_PLAYER05_Status2;
	extern Image* IMG_ANI_PLAYER05_Status3;
	extern Image* IMG_ANI_PLAYER0_Status0;
	extern Image* IMG_ANI_PLAYER0_Status1;
	extern Image* IMG_ANI_PLAYER0_Status2;
	extern Image* IMG_ANI_PLAYER0_Status3;
	extern Image* IMG_ANI_PLAYER1_Status0;
	extern Image* IMG_ANI_PLAYER1_Status1;
	extern Image* IMG_ANI_PLAYER1_Status2;
	extern Image* IMG_ANI_PLAYER1_Status3;
	extern Image* IMG_ANI_PLAYER2_Status0;
	extern Image* IMG_ANI_PLAYER2_Status1;
	extern Image* IMG_ANI_PLAYER2_Status2;
	extern Image* IMG_ANI_PLAYER2_Status3;
	extern Image* IMG_ANI_PLAYER3_Status0;
	extern Image* IMG_ANI_PLAYER3_Status1;
	extern Image* IMG_ANI_PLAYER3_Status2;
	extern Image* IMG_ANI_PLAYER3_Status3;
	extern Image* IMG_ANI_PLAYER4_Status0;
	extern Image* IMG_ANI_PLAYER4_Status1;
	extern Image* IMG_ANI_PLAYER4_Status2;
	extern Image* IMG_ANI_PLAYER4_Status3;
	extern Image* IMG_ANI_PLAYER5_Status0;
	extern Image* IMG_ANI_PLAYER5_Status1;
	extern Image* IMG_ANI_PLAYER5_Status2;
	extern Image* IMG_ANI_PLAYER5_Status3;
	extern Image* IMG_ANI_SubMoney;
	extern Image* IMG_ANI_TuoGuanAni;
	extern Image* IMG_ANI_WaitOhter;
	extern Image* IMG_BTN_BtnAddChip;
	extern Image* IMG_BTN_BtnBuyChip;
	extern Image* IMG_BTN_BtnCancelTrustee;
	extern Image* IMG_BTN_BtnChangeDesk;
	extern Image* IMG_BTN_BtnContinue;
	extern Image* IMG_BTN_BtnExit;
	extern Image* IMG_BTN_BtnHint;
	extern Image* IMG_BTN_BtnOpenCard;
	extern Image* IMG_BTN_BtnQuickSub;
	extern Image* IMG_BTN_BtnReList;
	extern Image* IMG_BTN_BtnTrustee;
	extern Image* IMG_MYANI_MyStatus0;
	extern Image* IMG_MYANI_MyStatus1;
	extern Image* IMG_MYANI_MyStatus2;
	extern Image* IMG_MYANI_MyStatus3;
	extern Image* IMG_NUM_AddScore;
	extern Image* IMG_NUM_ExChangeNum;
	extern Image* IMG_NUM_FailNum;
	extern Image* IMG_NUM_IngotNum;
	extern Image* IMG_NUM_SignAdd;
	extern Image* IMG_NUM_SignMinus;
	extern Image* IMG_NUM_TimeNum;
	extern Image* IMG_NUM_WinNum;
	extern Image* IMG_RATIOX_BACK;
	extern Image* IMG_RATIOX_BigCardType;
	extern Image* IMG_RATIOX_BigChip0;
	extern Image* IMG_RATIOX_BigChip1;
	extern Image* IMG_RATIOX_BigChip2;
	extern Image* IMG_RATIOX_BigChip3;
	extern Image* IMG_RATIOX_BigShadow;
	extern Image* IMG_RATIOX_BowlHint;
	extern Image* IMG_RATIOX_BtnBack;
	extern Image* IMG_RATIOX_BufferBK;
	extern Image* IMG_RATIOX_Card0;
	extern Image* IMG_RATIOX_Card1;
	extern Image* IMG_RATIOX_Card3;
	extern Image* IMG_RATIOX_Card4;
	extern Image* IMG_RATIOX_CardBack0;
	extern Image* IMG_RATIOX_CardBack1;
	extern Image* IMG_RATIOX_CardBack3;
	extern Image* IMG_RATIOX_CardBack4;
	extern Image* IMG_RATIOX_CardBackNormal;
	extern Image* IMG_RATIOX_CardNormal;
	extern Image* IMG_RATIOX_CellScoreBack;
	extern Image* IMG_RATIOX_EndBtnBack;
	extern Image* IMG_RATIOX_ExChipBack;
	extern Image* IMG_RATIOX_HurryTag;
	extern Image* IMG_RATIOX_LikeIngotBkDown;
	extern Image* IMG_RATIOX_LikeIngotBkUp;
	extern Image* IMG_RATIOX_LikeIngotHint1;
	extern Image* IMG_RATIOX_LikeIngotHint2;
	extern Image* IMG_RATIOX_MainMsg;
	extern Image* IMG_RATIOX_OpenCardTag;
	extern Image* IMG_RATIOX_OtherInfo;
	extern Image* IMG_RATIOX_Ready;
	extern Image* IMG_RATIOX_ResultCard;
	extern Image* IMG_RATIOX_ResultCardType;
	extern Image* IMG_RATIOX_SameCardTag;
	extern Image* IMG_RATIOX_ShadowBG;
	extern Image* IMG_RATIOX_SmallChip0;
	extern Image* IMG_RATIOX_SmallChip1;
	extern Image* IMG_RATIOX_SmallChip2;
	extern Image* IMG_RATIOX_SmallChip3;
	extern Image* IMG_RATIOX_SmallShadow;
	extern Image* IMG_RATIOX_ThreeSameTxt;
	extern Image* IMG_RATIOX_TimeBack;
	extern Image* IMG_RATIOX_TwoSameTxt;
	extern Image* IMG_RATIOX_UcCardBack;
	extern Image* IMG_RATIOX_UserChip;
	extern Image* IMG_RATIOX_UserInfo;
	extern Image* IMG_RATIOX_UserInfoLingZhu;
	extern Image* IMG_TOP_BULL_BG;
	extern Image* IMG_TOP_BULL_CLOSE;

	enum ResourceId
	{
		IMG_BTN_BtnCancelTrustee_ID,
		IMG_BTN_BtnChangeDesk_ID,
		IMG_BTN_BtnContinue_ID,
		IMG_BTN_BtnExit_ID,
		IMG_BTN_BtnOpenCard_ID,
		IMG_BTN_BtnTrustee_ID,
		IMG_BTN_BtnHint_ID,
		IMG_BTN_BtnReList_ID,
		IMG_BTN_BtnBuyChip_ID,
		IMG_BTN_BtnAddChip_ID,
		IMG_BTN_BtnQuickSub_ID,
		IMG_ANI_PLAYER0_Status0_ID,
		IMG_ANI_PLAYER0_Status1_ID,
		IMG_ANI_PLAYER0_Status2_ID,
		IMG_ANI_PLAYER0_Status3_ID,
		IMG_ANI_PLAYER1_Status0_ID,
		IMG_ANI_PLAYER1_Status1_ID,
		IMG_ANI_PLAYER1_Status2_ID,
		IMG_ANI_PLAYER1_Status3_ID,
		IMG_ANI_PLAYER2_Status0_ID,
		IMG_ANI_PLAYER2_Status1_ID,
		IMG_ANI_PLAYER2_Status2_ID,
		IMG_ANI_PLAYER2_Status3_ID,
		IMG_ANI_PLAYER3_Status0_ID,
		IMG_ANI_PLAYER3_Status1_ID,
		IMG_ANI_PLAYER3_Status2_ID,
		IMG_ANI_PLAYER3_Status3_ID,
		IMG_ANI_PLAYER4_Status0_ID,
		IMG_ANI_PLAYER4_Status1_ID,
		IMG_ANI_PLAYER4_Status2_ID,
		IMG_ANI_PLAYER4_Status3_ID,
		IMG_ANI_PLAYER5_Status0_ID,
		IMG_ANI_PLAYER5_Status1_ID,
		IMG_ANI_PLAYER5_Status2_ID,
		IMG_ANI_PLAYER5_Status3_ID,
		IMG_MYANI_MyStatus0_ID,
		IMG_MYANI_MyStatus1_ID,
		IMG_MYANI_MyStatus2_ID,
		IMG_MYANI_MyStatus3_ID,
		IMG_RATIOX_BACK_ID,
		IMG_RATIOX_UserInfo_ID,
		IMG_RATIOX_TimeBack_ID,
		IMG_RATIOX_UserChip_ID,
		IMG_RATIOX_CellScoreBack_ID,
		IMG_RATIOX_ExChipBack_ID,
		IMG_RATIOX_Ready_ID,
		IMG_RATIOX_BigShadow_ID,
		IMG_RATIOX_SmallShadow_ID,
		IMG_RATIOX_ShadowBG_ID,
		IMG_RATIOX_EndBtnBack_ID,
		IMG_RATIOX_BigChip0_ID,
		IMG_RATIOX_BigChip1_ID,
		IMG_RATIOX_BigChip2_ID,
		IMG_RATIOX_BigChip3_ID,
		IMG_RATIOX_SmallChip0_ID,
		IMG_RATIOX_SmallChip1_ID,
		IMG_RATIOX_SmallChip2_ID,
		IMG_RATIOX_SmallChip3_ID,
		IMG_RATIOX_CardBackNormal_ID,
		IMG_RATIOX_CardBack0_ID,
		IMG_RATIOX_CardBack1_ID,
		IMG_RATIOX_CardBack3_ID,
		IMG_RATIOX_CardBack4_ID,
		IMG_RATIOX_CardNormal_ID,
		IMG_RATIOX_Card0_ID,
		IMG_RATIOX_Card1_ID,
		IMG_RATIOX_Card3_ID,
		IMG_RATIOX_Card4_ID,
		IMG_RATIOX_ResultCard_ID,
		IMG_RATIOX_BigCardType_ID,
		IMG_RATIOX_UcCardBack_ID,
		IMG_RATIOX_MainMsg_ID,
		IMG_RATIOX_OtherInfo_ID,
		IMG_RATIOX_HurryTag_ID,
		IMG_RATIOX_OpenCardTag_ID,
		IMG_RATIOX_BufferBK_ID,
		IMG_RATIOX_UserInfoLingZhu_ID,
		IMG_RATIOX_BtnBack_ID,
		IMG_RATIOX_BowlHint_ID,
		IMG_RATIOX_LikeIngotHint1_ID,
		IMG_RATIOX_LikeIngotHint2_ID,
		IMG_RATIOX_ThreeSameTxt_ID,
		IMG_RATIOX_TwoSameTxt_ID,
		IMG_RATIOX_SameCardTag_ID,
		IMG_RATIOX_LikeIngotBkUp_ID,
		IMG_RATIOX_LikeIngotBkDown_ID,
		IMG_RATIOX_ResultCardType_ID,
		IMG_ANI_AddMoney_ID,
		IMG_ANI_SubMoney_ID,
		IMG_ANI_WaitOhter_ID,
		IMG_ANI_AutoDesk_ID,
		IMG_ANI_LikeIngotAni_ID,
		IMG_ANI_OffLineAni_ID,
		IMG_ANI_TuoGuanAni_ID,
		IMG_NUM_TimeNum_ID,
		IMG_NUM_WinNum_ID,
		IMG_NUM_FailNum_ID,
		IMG_NUM_SignAdd_ID,
		IMG_NUM_SignMinus_ID,
		IMG_NUM_AddScore_ID,
		IMG_NUM_ExChangeNum_ID,
		IMG_NUM_IngotNum_ID,
		IMG_TOP_BULL_BG_ID,
		IMG_TOP_BULL_CLOSE_ID,
		IMG_ANI_PLAYER00_Status0_ID,
		IMG_ANI_PLAYER00_Status1_ID,
		IMG_ANI_PLAYER00_Status2_ID,
		IMG_ANI_PLAYER00_Status3_ID,
		IMG_ANI_PLAYER01_Status0_ID,
		IMG_ANI_PLAYER01_Status1_ID,
		IMG_ANI_PLAYER01_Status2_ID,
		IMG_ANI_PLAYER01_Status3_ID,
		IMG_ANI_PLAYER02_Status0_ID,
		IMG_ANI_PLAYER02_Status1_ID,
		IMG_ANI_PLAYER02_Status2_ID,
		IMG_ANI_PLAYER02_Status3_ID,
		IMG_ANI_PLAYER03_Status0_ID,
		IMG_ANI_PLAYER03_Status1_ID,
		IMG_ANI_PLAYER03_Status2_ID,
		IMG_ANI_PLAYER03_Status3_ID,
		IMG_ANI_PLAYER04_Status0_ID,
		IMG_ANI_PLAYER04_Status1_ID,
		IMG_ANI_PLAYER04_Status2_ID,
		IMG_ANI_PLAYER04_Status3_ID,
		IMG_ANI_PLAYER05_Status0_ID,
		IMG_ANI_PLAYER05_Status1_ID,
		IMG_ANI_PLAYER05_Status2_ID,
		IMG_ANI_PLAYER05_Status3_ID,
		RESOURCE_ID_MAX
	};

	Image* GetImageById(int theId);
	Font* GetFontById(int theId);
	int GetSoundById(int theId);

	Image*& GetImageRefById(int theId);
	Font*& GetFontRefById(int theId);
	int& GetSoundRefById(int theId);

	ResourceId GetIdByImage(Image *theImage);
	ResourceId GetIdByFont(Font *theFont);
	ResourceId GetIdBySound(int theSound);
	const char* GetStringIdById(int theId);
	ResourceId GetIdByStringId(const char *theStringId);

} // namespace Sexy


#endif
