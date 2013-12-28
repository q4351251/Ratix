#include "stdafx.h"
#include "Chip.h"
#include "Res.h"
#include "SexyAppBase.h"
#include "widget.h"
#include "WidgetManager.h"
#include "GameView.h"

using namespace Sexy;

#define		MAX_FRAME		35			//最大帧数		
#define		ABS(a)			((a)<0?(-(a)):(a))



DXChip::DXChip(void)
{

	m_pGameApp = (CGameClientApp *)AfxGetApp();
	m_pGameDlg = (CGameClientDlg *)m_pGameApp->m_pGameClientDlg;
	m_bMoveFinish = false;
	m_BigChipImg[0] = IMG_RATIOX_BigChip0;
	m_BigChipImg[1] = IMG_RATIOX_BigChip1;
	m_BigChipImg[2] = IMG_RATIOX_BigChip2;
	m_BigChipImg[3] = IMG_RATIOX_BigChip3;

	m_SmallChipImg[0] = IMG_RATIOX_SmallChip0;
	m_SmallChipImg[1] = IMG_RATIOX_SmallChip1;
	m_SmallChipImg[2] = IMG_RATIOX_SmallChip2;
	m_SmallChipImg[3] = IMG_RATIOX_SmallChip3;

	BYTE cbMode1[4] = {1,0,0,0};
	CopyMemory(m_cbDeskHeapCount[0], cbMode1 , sizeof(cbMode1));
	BYTE cbMode2[4] = {3,8,0,0};
	CopyMemory(m_cbDeskHeapCount[1], cbMode2 , sizeof(cbMode2));
	BYTE cbMode3[4] = {2,6,5,0};
	CopyMemory(m_cbDeskHeapCount[2], cbMode3 , sizeof(cbMode3));
	BYTE cbMode4[4] = {3,8,7,0};
	CopyMemory(m_cbDeskHeapCount[3], cbMode4 , sizeof(cbMode4));
	BYTE cbMode5[4] = {10,10,10,10};
	CopyMemory(m_cbDeskHeapCount[4], cbMode5 , sizeof(cbMode5));
	
	CPoint cppp(440,363);
	m_ptDesk[0][0] = cppp;
	cppp.SetPoint(460,363);
	m_ptDesk[0][1] = cppp;
	cppp.SetPoint(450,377);
	m_ptDesk[0][2] = cppp;
	cppp.SetPoint(470,377);
	m_ptDesk[0][3] = cppp;

	cppp.SetPoint(200,420);
	m_ptDesk[1][0] = cppp;
	cppp.SetPoint(220,420);
	m_ptDesk[1][1] = cppp;
	cppp.SetPoint(210,434);
	m_ptDesk[1][2] = cppp;
	cppp.SetPoint(230,434);
	m_ptDesk[1][3] = cppp;

	cppp.SetPoint(308,570);
	m_ptDesk[2][0] = cppp;
	cppp.SetPoint(308+32,570);
	m_ptDesk[2][1] = cppp;
	cppp.SetPoint(308+32+32,570);
	m_ptDesk[2][2] = cppp;
	cppp.SetPoint(308,570+32);
	m_ptDesk[2][3] = cppp;

	cppp.SetPoint(810,543);
	m_ptDesk[3][0] = cppp;
	cppp.SetPoint(830,543);
	m_ptDesk[3][1] = cppp;
	cppp.SetPoint(820,557);
	m_ptDesk[3][2] = cppp;
	cppp.SetPoint(840,557);
	m_ptDesk[3][3] = cppp;

	cppp.SetPoint(700+20,388);
	m_ptDesk[4][0] = cppp;
	cppp.SetPoint(720+20,388);
	m_ptDesk[4][1] = cppp;
	cppp.SetPoint(700+20,402);
	m_ptDesk[4][2] = cppp;
	cppp.SetPoint(720+20,402);
	m_ptDesk[4][3] = cppp;
	//起始位置
	cppp.SetPoint(440,363);
	m_ptBegin[0] = cppp;
	cppp.SetPoint(200,434);
	m_ptBegin[1] = cppp;
	cppp.SetPoint(350,570);
	m_ptBegin[2] = cppp;
	cppp.SetPoint(810,523);
	m_ptBegin[3] = cppp;
	cppp.SetPoint(700+20,383);
	m_ptBegin[4] = cppp;
	//终点位置
	cppp.SetPoint(464,442);
	m_ptEnd[0] = cppp;
	cppp.SetPoint(413,465);
	m_ptEnd[1] = cppp;
	cppp.SetPoint(488,482);
	m_ptEnd[2] = cppp;
	cppp.SetPoint(549,470);
	m_ptEnd[3] = cppp;
	cppp.SetPoint(525,441);
	m_ptEnd[4] = cppp;

	//增加筹码
	m_iAddChipKind1[0][0] = 1;
	m_iAddChipKind1[0][1] = CHIP_100;
	m_iAddChipKind1[1][0] = 2;
	m_iAddChipKind1[1][1] = CHIP_1000;
	m_iAddChipKind1[2][0] = 5;
	m_iAddChipKind1[2][1] = CHIP_5000;
	m_iAddChipKind1[3][0] = 2;
	m_iAddChipKind1[3][1] = CHIP_10000;
	m_iAddChipKind1[4][0] = 5;
	m_iAddChipKind1[4][1] = CHIP_10000;

	m_iAddChipKind2[0][0] = 1;
	m_iAddChipKind2[0][1] = CHIP_100;
	m_iAddChipKind2[1][0] = 3;
	m_iAddChipKind2[1][1] = CHIP_100;
	m_iAddChipKind2[2][0] = 5;
	m_iAddChipKind2[2][1] = CHIP_1000;
	m_iAddChipKind2[3][0] = 3;
	m_iAddChipKind2[3][1] = CHIP_5000;
	m_iAddChipKind2[4][0] = 5;
	m_iAddChipKind2[4][1] = CHIP_5000;
	//0
	m_Shadow[0][0].bShow = false;
	m_Shadow[0][0].ptBegin.x = m_ptBegin[0].x;
	m_Shadow[0][0].ptBegin.y = m_ptBegin[0].y+2;
	m_Shadow[0][0].ptEnd.x =  m_ptEnd[0].x;
	m_Shadow[0][0].ptEnd.y =  m_ptEnd[0].y+2.;
	m_Shadow[0][0].wFrameNow = 0;
	m_Shadow[0][0].wTotalFrame=MAX_FRAME;

	m_Shadow[0][1].bShow = false;
	m_Shadow[0][1].ptBegin.x = m_ptBegin[0].x+23;
	m_Shadow[0][1].ptBegin.y = m_ptBegin[0].y+2;
	m_Shadow[0][1].ptEnd.x =  m_ptEnd[0].x;
	m_Shadow[0][1].ptEnd.y =  m_ptEnd[0].y+2.;
	m_Shadow[0][1].wFrameNow = 0;
	m_Shadow[0][1].wTotalFrame=MAX_FRAME;
	//1
	m_Shadow[1][0].bShow = false;
	m_Shadow[1][0].ptBegin.x = m_ptBegin[1].x;
	m_Shadow[1][0].ptBegin.y = m_ptBegin[1].y+2;
	m_Shadow[1][0].ptEnd.x =  m_ptEnd[1].x;
	m_Shadow[1][0].ptEnd.y =  m_ptEnd[1].y+2.;
	m_Shadow[1][0].wFrameNow = 0;
	m_Shadow[1][0].wTotalFrame=MAX_FRAME;

	m_Shadow[1][1].bShow = false;
	m_Shadow[1][1].ptBegin.x = m_ptBegin[1].x+23;
	m_Shadow[1][1].ptBegin.y = m_ptBegin[1].y+2;
	m_Shadow[1][1].ptEnd.x =  m_ptEnd[1].x;
	m_Shadow[1][1].ptEnd.y =  m_ptEnd[1].y+2.;
	m_Shadow[1][1].wFrameNow = 0;
	m_Shadow[1][1].wTotalFrame=MAX_FRAME;
	//2
	m_Shadow[2][0].bShow = false;
	m_Shadow[2][0].ptBegin.x = m_ptBegin[2].x;
	m_Shadow[2][0].ptBegin.y = m_ptBegin[2].y+2;
	m_Shadow[2][0].ptEnd.x =  m_ptEnd[2].x;
	m_Shadow[2][0].ptEnd.y =  m_ptEnd[2].y+2.;
	m_Shadow[2][0].wFrameNow = 0;
	m_Shadow[2][0].wTotalFrame=MAX_FRAME;

	m_Shadow[2][1].bShow = false;
	m_Shadow[2][1].ptBegin.x = m_ptBegin[2].x+23;
	m_Shadow[2][1].ptBegin.y = m_ptBegin[2].y+2;
	m_Shadow[2][1].ptEnd.x =  m_ptEnd[2].x;
	m_Shadow[2][1].ptEnd.y =  m_ptEnd[2].y+2.;
	m_Shadow[2][1].wFrameNow = 0;
	m_Shadow[2][1].wTotalFrame=MAX_FRAME;
	//3
	m_Shadow[3][0].bShow = false;
	m_Shadow[3][0].ptBegin.x = m_ptBegin[3].x;
	m_Shadow[3][0].ptBegin.y = m_ptBegin[3].y+2;
	m_Shadow[3][0].ptEnd.x =  m_ptEnd[3].x;
	m_Shadow[3][0].ptEnd.y =  m_ptEnd[3].y+2.;
	m_Shadow[3][0].wFrameNow = 0;
	m_Shadow[3][0].wTotalFrame=MAX_FRAME;

	m_Shadow[3][1].bShow = false;
	m_Shadow[3][1].ptBegin.x = m_ptBegin[3].x+23;
	m_Shadow[3][1].ptBegin.y = m_ptBegin[3].y+2;
	m_Shadow[3][1].ptEnd.x =  m_ptEnd[3].x;
	m_Shadow[3][1].ptEnd.y =  m_ptEnd[3].y+2.;
	m_Shadow[3][1].wFrameNow = 0;
	m_Shadow[3][1].wTotalFrame=MAX_FRAME;
	//4
	m_Shadow[4][0].bShow = false;
	m_Shadow[4][0].ptBegin.x = m_ptBegin[4].x;
	m_Shadow[4][0].ptBegin.y = m_ptBegin[4].y+2;
	m_Shadow[4][0].ptEnd.x =  m_ptEnd[4].x;
	m_Shadow[4][0].ptEnd.y =  m_ptEnd[4].y+2.;
	m_Shadow[4][0].wFrameNow = 0;
	m_Shadow[4][0].wTotalFrame=MAX_FRAME;

	m_Shadow[4][1].bShow = false;
	m_Shadow[4][1].ptBegin.x = m_ptBegin[4].x+23;
	m_Shadow[4][1].ptBegin.y = m_ptBegin[4].y+2;
	m_Shadow[4][1].ptEnd.x =  m_ptEnd[4].x;
	m_Shadow[4][1].ptEnd.y =  m_ptEnd[4].y+2.;
	m_Shadow[4][1].wFrameNow = 0;
	m_Shadow[4][1].wTotalFrame=MAX_FRAME;
}


DXChip::~DXChip(void)
{
}
//绘制
void DXChip::Draw( Sexy::Graphics* g )
{

	//绘制筹码
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_vecChip[i].size() <= 0) continue;

		g->DrawImage(IMG_RATIOX_SmallShadow,m_vecChip[i][0].ptNow.x-1,m_vecChip[i][0].ptNow.y+2);
		g->DrawImage(IMG_RATIOX_SmallShadow,m_vecChip[i][0].ptNow.x+22,m_vecChip[i][0].ptNow.y+2);
		for (int j=0; j<(int)m_vecChip[i].size(); j++)
		{
			g->DrawImage(m_SmallChipImg[m_vecChip[i][j].type],m_vecChip[i][j].ptNow.x,m_vecChip[i][j].ptNow.y);
		}
	}

}
//绘制桌面筹码
void DXChip::DrawDeskChip1( Sexy::Graphics* g, WORD index, LONGLONG lScrapChip )
{
	int mode =0 ;
	if (lScrapChip < 10000)
	{
		mode = 1;
	}
	else if (lScrapChip < 80000)
	{
		mode = 2;
	}
	else if (lScrapChip < 150000)
	{
		mode = 3;
	}
	else if (lScrapChip < 200000)
	{
		mode = 4;
	}
	else
	{
		mode = 5;
	}	
	BYTE cbCount[4];
	ZeroMemory(&cbCount ,sizeof(cbCount));
	for(INT i=3 ; i>=0; --i)
	{
		INT iScore = GetChipScore((ChipType)(i));
		BYTE cbTempCount=lScrapChip/iScore;
		if(m_cbDeskHeapCount[mode-1][i] >= cbTempCount)
		{
			cbCount[i] = cbTempCount;
		}
		else
		{
			cbCount[i] = m_cbDeskHeapCount[mode-1][i];
		}
		lScrapChip -= cbCount[i]*iScore;
	}

	int iHeapIndex=0;
	for(INT i=3 ; i>=0; --i)
	{
		if(cbCount[i] == 0)continue;
		//画底部阴影
		if (index == 2)//自己画大筹码
		{
			g->DrawImage(IMG_RATIOX_BigShadow,m_ptDesk[index][iHeapIndex].x,m_ptDesk[index][iHeapIndex].y+3);
		}
		else
		{
			g->DrawImage(IMG_RATIOX_SmallShadow,m_ptDesk[index][iHeapIndex].x,m_ptDesk[index][iHeapIndex].y+3);
		}	
		for(int j=0 ; j<cbCount[i]; j++)
		{
			if (index == 2)//自己画大筹码
			{
				g->DrawImage(m_BigChipImg[i],m_ptDesk[index][iHeapIndex].x,m_ptDesk[index][iHeapIndex].y-(j*3));
			}
			else
			{
				g->DrawImage(m_SmallChipImg[i],m_ptDesk[index][iHeapIndex].x,m_ptDesk[index][iHeapIndex].y-(j*3));
			}
		}
		iHeapIndex++;
	}
}

LONGLONG DXChip::GetChipScore( ChipType type )
{
	switch(type)
	{
	case CHIP_100:
		return 100;
	case CHIP_1000:
		return 1000;
	case CHIP_5000:
		return 5000;
	case CHIP_10000:
		return 10000;
	}
	return 0;
}

void DXChip::setPos( int x, int y )
{
	m_ix = x;
	m_iy = y;
}


float DXChip::GetInitSpeed(ChipInfo chipInfo , enDirection EnDirection)
{
	if(chipInfo.bAddSpeed) return 0;
	INT distance = 0;
	switch(EnDirection)
	{
	case d_x:
		distance = abs(chipInfo.ptEnd.x - chipInfo.ptBegin.x);
		break;
	case d_y:
		distance = abs(chipInfo.ptEnd.y - chipInfo.ptBegin.y);
		break;
	}
	return 2.f*distance/chipInfo.wTotalFrame;
}

float DXChip::GetAddSpeed(ChipInfo chipInfo, enDirection EnDirection)
{
	INT distance = 0;
	switch(EnDirection)
	{
	case d_x:
		distance = abs(chipInfo.ptEnd.x - chipInfo.ptBegin.x);
		break;
	case d_y:
		distance = abs(chipInfo.ptEnd.y - chipInfo.ptBegin.y);
		break;
	}
	return 2.f*(distance-GetInitSpeed(chipInfo , EnDirection)*chipInfo.wTotalFrame)/pow(chipInfo.wTotalFrame , 2.f);
}

float DXChip::GetCoordinate(ChipInfo chipInfo , enDirection EnDirection)
{
	INT startPoint = 0;
	INT direction=1;
	switch(EnDirection)
	{
	case d_x:
		startPoint = chipInfo.ptBegin.x;
		direction = (chipInfo.ptEnd.x-chipInfo.ptBegin.x)>=0?1:-1;
		break;
	case d_y:
		startPoint = chipInfo.ptBegin.y;
		direction = (chipInfo.ptEnd.y-chipInfo.ptBegin.y)>=0?1:-1;
		break;
	}
	float fAspeed = GetAddSpeed(chipInfo , EnDirection);
	return direction*(GetInitSpeed(chipInfo , EnDirection)*chipInfo.wFrameNow+fAspeed*pow(chipInfo.wFrameNow , 2.f)/2.f)+startPoint;
}

void DXChip::AddChips( LONGLONG lTotalPoint, int iPos , bool bAllIn/*=false*/ )
{
	//printf("玩家[%d] , 下注筹码[%I64d]\n" , iPos , lTotalPoint);



	if (lTotalPoint <5000)
	{
		ChipInfo chipInfo;
		for (int i=0; i<m_iAddChipKind1[0][0]; i++)
		{
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind1[0][1];
			chipInfo.ptBegin = m_ptBegin[iPos];
			chipInfo.ptEnd.x = m_ptEnd[iPos].x /*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y - i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow =m_ptBegin[iPos];
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
		CPoint temp_point;
		temp_point.SetPoint(m_ptBegin[iPos].x+20,m_ptBegin[iPos].y);
		for (int i=0; i<m_iAddChipKind2[0][0]; i++)
		{
			/*ChipInfo chipInfo;*/
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind2[0][1];
			chipInfo.ptBegin = temp_point;
			chipInfo.ptEnd.x = m_ptEnd[iPos].x +20/*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y -i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow = temp_point;
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}

	}
	else if (lTotalPoint <10000)
	{
		ChipInfo chipInfo;
		for (int i=0; i<m_iAddChipKind1[1][0]; i++)
		{
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind1[1][1];
			chipInfo.ptBegin = m_ptBegin[iPos];
			chipInfo.ptEnd.x = m_ptEnd[iPos].x /*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y - i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow =m_ptBegin[iPos];
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
		CPoint temp_point;
		temp_point.SetPoint(m_ptBegin[iPos].x+20,m_ptBegin[iPos].y);
		for (int i=0; i<m_iAddChipKind2[1][0]; i++)
		{
			/*ChipInfo chipInfo;*/
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind2[1][1];
			chipInfo.ptBegin = temp_point;
			chipInfo.ptEnd.x = m_ptEnd[iPos].x +20/*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y -i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow = temp_point;
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
	}
	else if (lTotalPoint <15000)
	{
		ChipInfo chipInfo;
		for (int i=0; i<m_iAddChipKind1[2][0]; i++)
		{
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind1[2][1];
			chipInfo.ptBegin = m_ptBegin[iPos];
			chipInfo.ptEnd.x = m_ptEnd[iPos].x /*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y - i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow =m_ptBegin[iPos];
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
		CPoint temp_point;
		temp_point.SetPoint(m_ptBegin[iPos].x+20,m_ptBegin[iPos].y);
		for (int i=0; i<m_iAddChipKind2[2][0]; i++)
		{
			/*ChipInfo chipInfo;*/
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind2[2][1];
			chipInfo.ptBegin = temp_point;
			chipInfo.ptEnd.x = m_ptEnd[iPos].x +20/*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y -i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow = temp_point;
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
	}
	else if (lTotalPoint <20000)
	{
		ChipInfo chipInfo;
		for (int i=0; i<m_iAddChipKind1[3][0]; i++)
		{
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind1[3][1];
			chipInfo.ptBegin = m_ptBegin[iPos];
			chipInfo.ptEnd.x = m_ptEnd[iPos].x /*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y - i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow =m_ptBegin[iPos];
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
		CPoint temp_point;
		temp_point.SetPoint(m_ptBegin[iPos].x+20,m_ptBegin[iPos].y);
		for (int i=0; i<m_iAddChipKind2[3][0]; i++)
		{
			/*ChipInfo chipInfo;*/
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind2[3][1];
			chipInfo.ptBegin = temp_point;
			chipInfo.ptEnd.x = m_ptEnd[iPos].x +20/*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y -i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow = temp_point;
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
	}
	else
	{
		ChipInfo chipInfo;
		for (int i=0; i<m_iAddChipKind1[4][0]; i++)
		{
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind1[4][1];
			chipInfo.ptBegin = m_ptBegin[iPos];
			chipInfo.ptEnd.x = m_ptEnd[iPos].x /*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y - i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow =m_ptBegin[iPos];
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
		CPoint temp_point;
		temp_point.SetPoint(m_ptBegin[iPos].x+20,m_ptBegin[iPos].y);
		for (int i=0; i<m_iAddChipKind2[4][0]; i++)
		{
			/*ChipInfo chipInfo;*/
			ZeroMemory(&chipInfo , sizeof(chipInfo));
			chipInfo.bAddSpeed=false;
			chipInfo.type = (ChipType)m_iAddChipKind2[4][1];
			chipInfo.ptBegin = temp_point;
			chipInfo.ptEnd.x = m_ptEnd[iPos].x +20/*+ rand()%20*/;
			chipInfo.ptEnd.y = m_ptEnd[iPos].y -i*2;
			chipInfo.bFly = true;
			chipInfo.ptNow = temp_point;
			chipInfo.wFrameNow = 0;
			chipInfo.wTotalFrame=MAX_FRAME;
			m_vecChip[iPos].push_back(chipInfo);
		}
	}
}

void DXChip::Reset()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_vecChip[i].clear();
		/*m_Shadow[i].bShow = false;*/
	}
	m_bMoveFinish = false;
}

void DXChip::SetCallBack( ChipCallback * pCallBack )
{
	m_pCallback = pCallBack;
}

void DXChip::AniTimer()
{
	//运动计算
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<(int)m_vecChip[i].size(); j++)
		{
			if (m_vecChip[i][j].wFrameNow < m_vecChip[i][j].wTotalFrame)
			{
				ChipAddFrame(m_vecChip[i][j]);


// 				m_vecChip[i][j].wFrameNow++;
// 				m_vecChip[i][j].ptNow.x = GetCoordinate(m_vecChip[i][j] , d_x) ;
// 				m_vecChip[i][j].ptNow.y = GetCoordinate(m_vecChip[i][j] , d_y) ;
			}
			else if (!m_bMoveFinish)
			{
				m_bMoveFinish = true;
				m_pCallback->OnYaDiFinish();
			}
		}
	}
}

void DXChip::ChipAddFrame( ChipInfo& chipinfo,float fAngle/*=0*/ )
{

	if (chipinfo.wFrameNow < chipinfo.wTotalFrame)
	{
// 		DWORD time = GetTickCount();
// 		printf("时间：%d \n",time);
// 		printf("NowFrame：%d--TotalFrame：%d \n",chipinfo.wFrameNow,chipinfo.wTotalFrame);
		chipinfo.wFrameNow++;
		chipinfo.ptNow.x = (chipinfo.ptEnd.x - chipinfo.ptBegin.x)*chipinfo.wFrameNow/chipinfo.wTotalFrame + chipinfo.ptBegin.x;
		chipinfo.ptNow.y = (chipinfo.ptEnd.y - chipinfo.ptBegin.y)*chipinfo.wFrameNow/chipinfo.wTotalFrame + chipinfo.ptBegin.y;	
		
		/*		return true;*/
	}

}

void DXChip::Update()
{
	AniTimer();
}
