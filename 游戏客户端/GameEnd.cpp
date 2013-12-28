#include "StdAfx.h"
#include "GameEnd.h"
#include "GameClient.h"
#include "DDImage.h"
#include "ButtonWidget.h"
#include "Res.h"
#include "SysFont.h"
#include "GameClientDlg.h"
#include "GameView.h"

using namespace Sexy;

#define		ABS(a)			((a)<0?(-(a)):(a))

#define		OUTSPACE        18       //出牌间距     
#define SAFE_DELETE(p)				{ if(p) { delete (p); (p)=NULL; } }					//! 安全的删除new出来的变量

			

int GameEnd::m_x = 0;
int GameEnd::m_y = 0;
bool GameEnd::m_bCtrlDown = false;

CPoint cpResultCard[GAME_PLAYER] = {CPoint(402,108),CPoint(111,177),CPoint(469,562),CPoint(820,182),CPoint(539,111)};
CPoint cpResultCardType[GAME_PLAYER] = {CPoint(399,60),CPoint(105,123),CPoint(467,505),CPoint(817,127),CPoint(532,60)};
CPoint cpBtnBack(283,435);


GameEnd::GameEnd(void)
{
    m_pGameApp = (CGameClientApp *)AfxGetApp();
    m_pGameDlg = (CGameClientDlg *)m_pGameApp->m_pGameClientDlg;
	m_pFont10WeiRuanYaHei = new Sexy::SysFont(m_pGameApp,"微软雅黑",12);

	ZeroMemory(m_RemainCardCount,sizeof(m_RemainCardCount));
	ZeroMemory(m_EndScore,sizeof(m_EndScore));
	ZeroMemory(m_EndScoreTemp,sizeof(m_EndScoreTemp));



	m_wBanker = INVALID_CHAIR;
	m_wTimerNum = 10;
	m_wBeiShu = 0;
	m_pTimerAni =NULL;
	m_pTimerAni = new AnimLayer(m_pGameApp, this);
	InitTimerAni();
	m_bShowEndScore =false;


    ResetPlace();
	

    m_iScore = 0;

	InitWidget();
}

GameEnd::~GameEnd(void)
{
	RemoveAllWidgets(true);
    SAFE_DELETE(m_pTimerAni);
	delete m_pFont10WeiRuanYaHei;
}

void GameEnd::Draw( Sexy::Graphics* g )
{
	g->SetColor(Sexy::Color(255,255,255,150));
	g->SetColorizeImages(true);
	g->DrawImage(IMG_RATIOX_ShadowBG,0,0);
	g->SetColor(Sexy::Color(255,255,255,255));


	g->DrawImage(IMG_RATIOX_EndBtnBack,cpBtnBack.x,cpBtnBack.y);

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if(m_cbOX[i] == 11)continue;
		/*_log("RatioxLog","m_cbOX[%d] = %d", i, m_cbOX[i]);*/

		if(m_cbOX[i]>0)
		{
			for (int j=0; j<m_vecResultCard[i].size(); j++)
			{
				if (j<2)
				{
					g->DrawImageCel(IMG_RATIOX_ResultCard,cpResultCard[i].x+13+27*j, cpResultCard[i].y,m_vecResultCard[i][j].GetVaule(),m_vecResultCard[i][j].GetType()%4);
				}
				else
				{
					g->DrawImageCel(IMG_RATIOX_ResultCard,cpResultCard[i].x+27*(j-2),cpResultCard[i].y+48,m_vecResultCard[i][j].GetVaule(),m_vecResultCard[i][j].GetType()%4);
				}
			}
			g->DrawImageCel(IMG_RATIOX_ResultCardType,cpResultCardType[i].x,cpResultCardType[i].y,m_cbOX[i]>=10?10:m_cbOX[i],0);
		}
		else
		{
			for (int j=0; j<m_vecResultCard[i].size(); j++)
			{
				if (j<3)
				{
					g->DrawImageCel(IMG_RATIOX_ResultCard,cpResultCard[i].x+27*j, cpResultCard[i].y,m_vecResultCard[i][j].GetVaule(),m_vecResultCard[i][j].GetType()%4);
				}
				else
				{
					g->DrawImageCel(IMG_RATIOX_ResultCard,cpResultCard[i].x+27*(j-3),cpResultCard[i].y+48,m_vecResultCard[i][j].GetVaule(),m_vecResultCard[i][j].GetType()%4);
				}
			}
			g->DrawImageCel(IMG_RATIOX_ResultCardType,cpResultCardType[i].x,cpResultCardType[i].y,m_cbOX[i]>=10?10:m_cbOX[i],0);
		}
	}
}


void GameEnd::Update()
{
	if (m_pTimerAni!=NULL)
	{
		m_pTimerAni->Update();
	}

    Widget::Update();
}

void GameEnd::MouseDown( int x, int y, int theClickCount )
{
    GameEnd::m_x = x;
    GameEnd::m_y = y;
	if (theClickCount > 0)
	{
		m_bLeftDown = true;
	}

    printf("--鼠标 [%d][%d]\n", m_x, m_y);
}

void GameEnd::MouseDrag( int x, int y )
{
	int m_Movex = 0;
	int m_Movey = 0;
	//if (m_bLeftDown)
	//{
	//	m_Movex = mX+x-GameEnd::m_x;
	//	m_Movey = mY+y-GameEnd::m_y;
	//	Move(m_Movex,m_Movey);
	//}
}

void GameEnd::MouseMove( int x, int y )
{
}

void GameEnd::MouseUp( int x, int y, int theClickCount )
{
	m_bLeftDown = false;
}

void GameEnd::KeyDown( Sexy::KeyCode theKey )
{
    if (theKey == KEYCODE_UP)
    {
        m_bCtrlDown?(GameEnd::m_y-=10):GameEnd::m_y--;
    }
    else if(theKey == KEYCODE_DOWN)
    {
        m_bCtrlDown?(GameEnd::m_y+=10):GameEnd::m_y++;
    }
    else if(theKey == KEYCODE_LEFT)
    {
        m_bCtrlDown?(GameEnd::m_x-=10):GameEnd::m_x--;
    }
    else if(theKey == KEYCODE_RIGHT)
    {
        m_bCtrlDown?(GameEnd::m_x+=10):GameEnd::m_x++;
    }
    else if (theKey == KEYCODE_CONTROL)
    {
        m_bCtrlDown = !m_bCtrlDown;
    }

	//m_pBtnContinue->Resize(283 + GameEnd::m_x,466 +GameEnd::m_y,IMG_BTN_BtnExit->GetWidth()/4, IMG_BTN_BtnExit->GetHeight());
    printf("--键盘%d [%d][%d]\n",(int)theKey, m_x, m_y);	
}

void GameEnd::ButtonDepress( int theId )
{
	switch(theId)
	{
	case BT_CONTINUE:			//继续
		{
			m_pGameView->OperaTimer(0,0,INVALID_CHAIR, FALSE);
			m_pGameDlg->SendUserReady(NULL,0);
			this->SetVisible(false);
			m_pGameView->m_pResultNum->SetNowShow(false);
			m_pGameView->AfterReady();
		}
		break;
	case BT_CHANGE_DESK:		//换桌
		{
			m_pGameView->OperaTimer(0,0,INVALID_CHAIR, FALSE);
			m_pGameDlg->SendChangeTable();
			this->SetVisible(false);
			m_pGameView->m_pResultNum->SetNowShow(false);
		}
		break;
	case BT_EXIT:				//退出
		{
			m_pGameView->OperaTimer(0,0,INVALID_CHAIR, FALSE);
			m_pGameView->m_pResultNum->SetNowShow(false);
			this->SetVisible(false);
			m_pGameDlg->PostMessage(WM_CLOSE);
		}
		break;
	}
	//m_pGameView->m_GameCard.Reset();
	m_pGameView->m_cpTimer.SetPoint(441,320);
	m_pGameView->m_pBtnTuoGuan->SetDisabled(true);
	m_pGameView->MoveButton(0,0);
	m_pGameView->m_pMyBasicInfo->m_pBtnAddChip->SetDisabled(true);
}

void GameEnd::AddedToManager( Sexy::WidgetManager* theWidgetManager )
{
    Widget::AddedToManager(theWidgetManager);
}

void GameEnd::Resize( int theX, int theY, int theWidth, int theHeight )
{
    Widget::Resize(theX,theY,theWidth,theHeight);
}




//画数字的方法 theJustification 对齐方式 -1为左对齐 0为居中对齐(x就为中心坐标了),1为右对齐(x就为最右边的坐标)
void GameEnd::DrawNum2( Sexy::Graphics* g, Sexy::Image *img, LONGLONG iNum, int x, int y, int theJustification /*= 0*/, int iSign /*= 0*/)
{
	int i;
	if(iNum >= 0)
	{		
		LONGLONG iResult = iNum;
		int iWidth = /*img->GetWidth()/10;*/img->GetCelWidth();
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
		if (iSign == 1)
		{
			g->DrawImageCel(img,iLeft-7,iTop-1,10);
			iLeft += 7;
		}
		else if (iSign == 2)
		{
			g->DrawImageCel(img,iLeft-7,iTop-1,11);
			iLeft += 7;
		}
		for(i=0; i<iIndex; i++)
		{
			//g->DrawImageCel(img,iLeft+31*i,iTop,iPos[iIndex-i-1]);
			g->DrawImageCel(img,iLeft+iWidth*i,iTop,iPos[iIndex-i-1]);
		}
	}
}

void GameEnd::SetTimerNum( WORD wNum )
{
	m_wTimerNum = wNum;
	m_pTimerAni->SetFixFrame(0);
	if (m_wTimerNum<=5)
	{
		m_pTimerAni->SetFixFrame(-1);
	}
}

//时钟动画
void GameEnd::InitTimerAni()
{
	m_pTimerAni->SetLoopNum(-100);
	m_pTimerAni->SetFixFrame(0);
	m_pTimerAni->ShowAnimate(Sexy::Point(475,335),Sexy::Point(475,335),150);
	m_pTimerAni->SetEndHide(true);
}

// bool GameEnd::FindChairID( WORD wChairID )
// {
//     for (WORD i=0;i<4;i++)
//     {
//         if (wChairID == m_cbRanKing[i])
//         {
//             return true;
//         }
//     }
//     return false;
// }

void GameEnd::SetGameView( CGameView *pGameView )
{
    m_pGameView = pGameView;
}





void GameEnd::ResetPlace()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_cbOX[i] = 11;
	}

}

void GameEnd::SetGameEndResultCard( std::vector<CCard> result[] )
{

	for(int i=0; i<GAME_PLAYER; i++)
	{
		m_vecResultCard[i].clear();
	}
	for(int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<(int)result[i].size(); j++)
		{
			m_vecResultCard[i].push_back(result[i][j]);
		}
	}

// 	//测试
// 	for(int i=0; i<GAME_PLAYER; i++)
// 	{
// 		for (int j=0; j<MAX_COUNT; j++)
// 		{
// 			CCard tempcard;
// 			tempcard.SetCard(1);
// 			m_vecResultCard[i].push_back(tempcard);
// 		}
// 	}
}

void GameEnd::InitWidget()
{
	//兑换筹码按钮
	m_pBtnExit = new Sexy::ButtonWidget(BT_EXIT, this);
	m_pBtnExit->SetBtnImage(m_pGameApp, IMG_BTN_BtnExit);
	m_pBtnExit->Resize(cpBtnBack.x+15,cpBtnBack.y+3,IMG_BTN_BtnExit->GetWidth()/4, IMG_BTN_BtnExit->GetHeight());
	m_pBtnExit->SetVisible(true);
	AddWidget(m_pBtnExit);

	//换桌按钮
	m_pBtnChangeDesk = new Sexy::ButtonWidget(BT_CHANGE_DESK, this);
	m_pBtnChangeDesk->SetBtnImage(m_pGameApp, IMG_BTN_BtnChangeDesk);
	m_pBtnChangeDesk->Resize(cpBtnBack.x+155,cpBtnBack.y+3,IMG_BTN_BtnChangeDesk->GetWidth()/4, IMG_BTN_BtnChangeDesk->GetHeight());
	m_pBtnChangeDesk->SetVisible(true);
	AddWidget(m_pBtnChangeDesk);

	//继续按钮
	m_pBtnContinue = new Sexy::ButtonWidget(BT_CONTINUE, this);
	m_pBtnContinue->SetBtnImage(m_pGameApp, IMG_BTN_BtnContinue);
	m_pBtnContinue->Resize(cpBtnBack.x+295,cpBtnBack.y+3,IMG_BTN_BtnContinue->GetWidth()/4, IMG_BTN_BtnContinue->GetHeight());
	m_pBtnContinue->SetVisible(true);
	AddWidget(m_pBtnContinue);

}

void GameEnd::SetOxData( BYTE cbOx[GAME_PLAYER] )
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_cbOX[i] = 11;
	}
	CopyMemory(m_cbOX, cbOx, sizeof(m_cbOX));
}
