#include "stdafx.h"
#include "ResultNum.h"
#include "Res.h"
#include "SexyAppBase.h"
#include "widget.h"
#include "WidgetManager.h"
#include "GameView.h"
#include "GameClient.h"
#include "GameClientDlg.h"
using namespace Sexy;

#define				MAX_NUM_FRAME			20;
#define				NUM_MOVE				60
//view Chair
CPoint cpPosition[GAME_PLAYER] = {CPoint(430,292),CPoint(180,362),CPoint(295,520),CPoint(810,488),CPoint(700,295)};
static int iHeight[GAME_PLAYER] ={0};


CResultNum::CResultNum(void)
{
	m_pGameApp = (CGameClientApp *)AfxGetApp();
	m_pGameDlg = (CGameClientDlg *)m_pGameApp->m_pGameClientDlg;

 	ZeroMemory(m_lResultNum, sizeof(m_lResultNum));
 	ZeroMemory(m_cbViewStatus, sizeof(m_cbViewStatus));
// // 	for (int i=0; i<GAME_PLAYER; i++)
// // 	{
// // 		m_lResultNum[i] = 1000;
// // 	}
  	m_bShow = false;
	m_iTrans =0;
}

CResultNum::~CResultNum(void)
{
}

void CResultNum::SetResultNum( LONGLONG lResultNum[GAME_PLAYER], BYTE cbPlayStatus[GAME_PLAYER] )
{
 	ZeroMemory(m_lResultNum, sizeof(m_lResultNum));
 	CopyMemory(m_lResultNum, lResultNum, sizeof(m_lResultNum));
 	ZeroMemory(m_cbViewStatus, sizeof(m_cbViewStatus));
 	CopyMemory(m_cbViewStatus, cbPlayStatus, sizeof(m_cbViewStatus));
 	//for (int i=0; i<GAME_PLAYER; i++)
 	//{
 	//	m_lResultNum[i] = lResultNum[i];
 	///*	m_cbPlayStatus[i] = cbPlayStatus[i];*/
 	//}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		iHeight[i] = cpPosition[i].y+NUM_MOVE;
	}
	m_iTrans = 0;
 	m_bShow = true;
}



//ÉèÖÃ·ÖÊý	
void CResultNum::Draw( Sexy::Graphics* g )
{
 	if (m_bShow)
 	{
		g->SetColor(Sexy::Color(255,255,255,m_iTrans));
		g->SetColorizeImages(true);
 		for(int i=0; i<GAME_PLAYER; i++)
 		{
// 			const tagUserData *pUserData = m_pGameDlg->GetUserData(i);
// 			if (pUserData == NULL) continue; 
			if(m_cbViewStatus[i] == FALSE)continue;
 			WORD wChair =i/* m_pGameDlg->SwitchViewChairID(i)*/;
 			if (m_lResultNum[i]>=0)
 			{
 				g->DrawImage(IMG_NUM_SignAdd,cpPosition[wChair].x, iHeight[wChair]);
 				DrawNum(g,IMG_NUM_WinNum, m_lResultNum[i],cpPosition[wChair].x+18,  iHeight[wChair], -1);
 			}
 			else
 			{
 				g->DrawImage(IMG_NUM_SignMinus,cpPosition[wChair].x, iHeight[wChair]);
 				DrawNum(g,IMG_NUM_FailNum, -m_lResultNum[i],cpPosition[wChair].x+19, iHeight[wChair], -1);
 			}
 			
 		}
		g->SetColor(Sexy::Color(255,255,255,255));
 	}
}

void CResultNum::SetNowShow( bool bShow )
{
	m_bShow = bShow;
	if(!bShow)
	{
		ZeroMemory(m_cbViewStatus, sizeof(m_cbViewStatus));
	}
}



void CResultNum::SetPosition( int x , int y )
{
	m_iy = y;
	m_ix = x;
}

void CResultNum::AniTimer()
{
	NumAddFrame();
}

void CResultNum::NumAddFrame()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(m_cbViewStatus[i] == FALSE) continue;
// 		const tagUserData *pUserData = m_pGameDlg->GetUserData(i);
// 		if (pUserData == NULL) continue; 
		WORD wChair = i/*m_pGameDlg->SwitchViewChairID(i)*/;
		if (iHeight[wChair]>cpPosition[wChair].y)
		{
			iHeight[wChair] -= NUM_MOVE/MAX_NUM_FRAME;
			m_iTrans += 255/MAX_NUM_FRAME;
			m_iTrans = (m_iTrans>255)?255:m_iTrans;
		}
	}
}
