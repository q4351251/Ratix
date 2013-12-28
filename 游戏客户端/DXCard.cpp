#include "stdafx.h"
#include "DXCard.h"
#include "Res.h"
#include "SexyAppBase.h"
#include "Rect.h"
#include "WidgetManager.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameView.h"
#include "SexyMatrix.h"
#include "SysFont.h"

using namespace Sexy;

#define	  MAX_FRAME					10
#define	  MAX_DISAPPEAR_FRAME		20

#define   SPACE                     22              //自己的牌间距
#define   TOWARDSSPACE              7               //对面的手牌间距
#define   OUTSPACE                  15              //出牌间距
#define   LEFTRIGHTSPACE            5               //左右的牌间距
#define   UPSPACE                   25              //选中牌后的上移间距
#define   HANDLE_CARD_TOP           550             //手牌坐标（上）
#define   SEND_CARD_TOP             477             //出牌坐标（上）

CPoint cpOutCard[GAME_PLAYER] = {CPoint(321,360),CPoint(161,474),CPoint(0,0),CPoint(751,458),CPoint(565,360)};

//手牌位置（五张）
/*CPoint cpCardPos[GAME_PLAYER] = {CPoint()}; */

//0x01 -- 0x0d  方块 A - K
//0x11 -- 0x1d  草花 A - K
//0x21 -- 0x2d  红桃 A - K
//0x31 -- 0x3d  黑桃 A - K
//0x4e -- 0x4f  小鬼 大鬼
CCard::CCard(void)
{
	m_bSelect = false;
	m_bMouseSelect = false;
	m_iMark = 0;
	m_CardX = 0;
	m_CardY = 0;    
	m_iTotalFrame =0;
	m_iNowFrame = 0;
}

CCard::CCard( char cCard )
{
	m_bSelect = false;
	m_bMouseSelect = false;
	m_iMark = 0;
	m_cCard = cCard;
	m_CardX = 0;
	m_CardY = 0;    
	m_iTotalFrame =0;
	m_iNowFrame = 0;
}

CCard::~CCard(void)
{
}

int CCard::GetType()
{
	return m_cCard >> 4;
}

int CCard::GetVaule()
{
	return (m_cCard & 0x0F)-1;
}

void CCard::SetCard( char cCard )
{
	m_cCard = cCard;
}

void CCard::SetSelect( bool bSelect )
{
	m_bSelect = bSelect;
}

void CCard::SetMouseSelect( bool bSelect )
{
	m_bMouseSelect = bSelect;
}

bool CCard::IsSelect()
{
	return m_bSelect;
}

bool CCard::IsMouseSelect()
{
	return m_bMouseSelect;
}

void CCard::SetCardPos( int x,int y )
{
	m_CardX = x;
	m_CardY = y;
}

void CCard::SetMark( int iMark )
{
	m_iMark = iMark;
}





//---------------------------------------------------------------
CGameCard::CGameCard( void )
{
	m_pGameApp = (CGameClientApp *)AfxGetApp();
	m_pGameDlg = (CGameClientDlg *)m_pGameApp->m_pGameClientDlg;

	for (int i =0; i<MAX_COUNT; i++)
	{
		m_iTransNow[i] = 255;
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_vecHandCard[i].clear();
	}
	
	m_vecFaceCard.clear();
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_vecSendCard[i].clear();
	}
	ZeroMemory(m_cbOtherHandCard,sizeof(m_cbOtherHandCard));
	mLeftDown = false;
	mRightDown = false;
	mMiddleDown = false;
	m_bSendCard = false;
	mMouseX = 0;
	mMouseY = 0;
	mMouseDownX = 0;
	mMouseDownY = 0;
	m_iDisTrans = 255;
	m_bDisappear = false;
	m_pFont10WeiRuanYaHei = new Sexy::SysFont("微软雅黑",10);
}

CGameCard::~CGameCard()
{
	delete m_pFont10WeiRuanYaHei;
}

void CGameCard::Draw( Sexy::Graphics* g )
{

	//自己的牌
	DrawMeCard(g);
// 	//其他人的牌
 	DrawOtherCard(g);    
}

void CGameCard::DrawMeCard( Sexy::Graphics* g )
{    

	for (int j=0; j<(int)m_vecHandCard[2].size(); j++)
	{
		g->SetColor(Sexy::Color(255, 255, 255, m_iTransNow[j]));//设置透明度
		if(m_bDisappear)
		{
			g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
		}
		g->SetColorizeImages(true);
		g->DrawImageCel(IMG_RATIOX_CardNormal,414 + j*25, 580, m_vecHandCard[2][j].GetVaule(),m_vecHandCard[2][j].GetType()%4);
		g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
	}

	if(m_bDisappear)
	{
		g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
		g->SetColorizeImages(true);
	}
	for (int i=0; i<(int)m_vecSendCard[2].size();i++)
	{
		g->DrawImageCel(IMG_RATIOX_CardNormal,450+i*25, 550, m_vecSendCard[2][i].GetVaule(),m_vecSendCard[2][i].GetType()%4);
	}

	for (int i=0; i<(int)m_vecSendCard2[2].size(); i++)
	{
		g->DrawImageCel(IMG_RATIOX_CardNormal,439 + i*25, 580, m_vecSendCard2[2][i].GetVaule(),m_vecSendCard2[2][i].GetType()%4);
	}
	g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
}

void CGameCard::DrawOtherCard( Sexy::Graphics* g )
{

	for(int i=0;i<GAME_PLAYER; i++)
	{
		if (i==2) continue;
// 		for (int i=0; i<(int)m_vecSendCard[i].size(), i++)
// 		{
// 			g->DrawImageCel(IMG_RATIOX_CardNormal,414 + j*25, 570, m_vecHandCard[2][j].GetVaule(),m_vecHandCard[2][j].GetType()%4);
// 		}
// 
// 		for (int i=0; i<(int)m_vecSendCard2[i].size(), i++)
// 		{
// 			g->DrawImageCel(IMG_RATIOX_CardNormal,414 + j*25, 570, m_vecHandCard[2][j].GetVaule(),m_vecHandCard[2][j].GetType()%4);
// 		}
		if (i ==0)
		{
 			for (int j=0; j<m_vecHandCard[i].size(); j++)
 			{
				g->SetColor(Sexy::Color(255, 255, 255, m_iTransNow[j]));//设置透明度
				if(m_bDisappear)
				{
					g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				}
				g->SetColorizeImages(true);
 				if (m_vecHandCard[i][j].GetVaule() == -1)
 				{
 					g->DrawImage(IMG_RATIOX_CardBack0, 325-15*j,339+4*j);

 				}
 				else
 				{
 					g->DrawImageCel(IMG_RATIOX_Card0, 325-15*j,339+4*j ,m_vecHandCard[i][j].GetVaule(),m_vecHandCard[i][j].GetType()%4);
 				}
				g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
 			}
			//摊牌后的牌
			if(m_bDisappear)
			{
				g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				g->SetColorizeImages(true);
			}
			
			for (int k =0; k<(int)m_vecSendCard[i].size(); k++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card0, 321-15*k,360+4*k ,m_vecSendCard[i][k].GetVaule(),m_vecSendCard[i][k].GetType()%4);
			}
			for (int l =0; l<(int)m_vecSendCard2[i].size(); l++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card0, 325-15*(l+1),339+4*(l+1) ,m_vecSendCard2[i][l].GetVaule(),m_vecSendCard2[i][l].GetType()%4);
			}
		}
		else if (i == 1)
		{
			for (int j=0; j<m_vecHandCard[i].size(); j++)
			{
				g->SetColor(Sexy::Color(255, 255, 255, m_iTransNow[j]));//设置透明度
				if(m_bDisappear)
				{
					g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				}
				g->SetColorizeImages(true);
				if (m_vecHandCard[i][j].GetVaule() == -1)
				{
					g->DrawImage(IMG_RATIOX_CardBack1, 133 -5*j,456+12*j);
				}
				else
				{
					g->DrawImageCel(IMG_RATIOX_Card1, 133 -5*j,456+12*j,m_vecHandCard[i][j].GetVaule(),m_vecHandCard[i][j].GetType()%4);
				}
				g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
			}
			//摊牌后的牌
			if(m_bDisappear)
			{
				g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				g->SetColorizeImages(true);
			}
			
			for (int k =0; k<(int)m_vecSendCard[i].size(); k++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card1, cpOutCard[i].x -5*k,cpOutCard[i].y+12*k,m_vecSendCard[i][k].GetVaule(),m_vecSendCard[i][k].GetType()%4);
			}
			for (int l =0; l<(int)m_vecSendCard2[i].size(); l++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card1, 133 -5*(l+1),456+12*(l+1),m_vecSendCard2[i][l].GetVaule(),m_vecSendCard2[i][l].GetType()%4);
			}
		}
		else if (i == 3)
		{
			for (int j=0; j<m_vecHandCard[i].size(); j++)
			{
				g->SetColor(Sexy::Color(255, 255, 255, m_iTransNow[j]));//设置透明度
				if(m_bDisappear)
				{
					g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				}
				g->SetColorizeImages(true);
				if (m_vecHandCard[i][j].GetVaule() == -1)
				{
					g->DrawImage(IMG_RATIOX_CardBack3, 793-j*5,475-12*j);
				}
				else
				{
					g->DrawImageCel(IMG_RATIOX_Card3, 793-j*5,475-12*j,m_vecHandCard[i][j].GetVaule(),m_vecHandCard[i][j].GetType()%4);
				}
				g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
			}
			//摊牌后的牌
			if(m_bDisappear)
			{
				g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				g->SetColorizeImages(true);
			}
			
			for (int k =0; k<(int)m_vecSendCard[i].size(); k++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card3, cpOutCard[i].x-k*5,cpOutCard[i].y-12*k,m_vecSendCard[i][k].GetVaule(),m_vecSendCard[i][k].GetType()%4);
			}
			for (int l =0; l<(int)m_vecSendCard2[i].size(); l++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card3, 793-(l+1)*5,475-12*(l+1),m_vecSendCard2[i][l].GetVaule(),m_vecSendCard2[i][l].GetType()%4);
			}
		}
		if (i== 4)
		{
			for (int j=0; j<m_vecHandCard[i].size();j++)
			{
				g->SetColor(Sexy::Color(255, 255, 255, m_iTransNow[j]));//设置透明度
				if(m_bDisappear)
				{
					g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				}
				g->SetColorizeImages(true);
				if (m_vecHandCard[i][j].GetVaule() == -1)
				{
					g->DrawImage(IMG_RATIOX_CardBack4, 603+20 -15*j,349-4*j);
				}
				else
				{
					g->DrawImageCel(IMG_RATIOX_Card4,603+20 -15*j,349-4*j,m_vecHandCard[i][j].GetVaule(),m_vecHandCard[i][j].GetType()%4);
				}
				g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
			}
			//摊牌后的牌
			if(m_bDisappear)
			{
				g->SetColor(Sexy::Color(255, 255, 255,m_iDisTrans));//设置透明度
				g->SetColorizeImages(true);
			}
			for (int k =0; k<(int)m_vecSendCard[i].size(); k++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card4,565+20 -15*k,360-4*k ,m_vecSendCard[i][k].GetVaule(),m_vecSendCard[i][k].GetType()%4);
			}
			for (int l =0; l<(int)m_vecSendCard2[i].size(); l++ )
			{
				g->DrawImageCel(IMG_RATIOX_Card4,603+20 -15*(l+1),349-4*(l+1),m_vecSendCard2[i][l].GetVaule(),m_vecSendCard2[i][l].GetType()%4);
			}
		}

	}
	g->SetColor(Sexy::Color(255, 255, 255, 255));//设置不透明
}

void CGameCard::MouseMove( int x, int y )
{
	
}

void CGameCard::MouseDrag( int x, int y )
{
	
}

void CGameCard::MouseDown( int x, int y, int theClickCount )
{
	
}

void CGameCard::MouseUp( int x, int y, int theClickCount )
{
       
}

DWORD CGameCard::SetCardData( std::vector<CCard> &Card,const BYTE bCardData[], DWORD dwCardCount )
{
	Card.clear();
	//设置扑克
	for (DWORD i=0;i<dwCardCount;i++)
	{
		Card.push_back(bCardData[i]);
	}

	return dwCardCount;
}

void CGameCard::SetPayTruibuteShoot( BYTE bCardData,WORD wCardCount )
{
// 	//设置变量
// 	for(DWORD j=0;j<(DWORD)m_vecHandCard.size();j++)
// 	{
// 		if(m_vecHandCard[j].GetCard()==bCardData &&!m_vecHandCard[j].IsSelect())
// 		{
// 			m_vecHandCard[j].SetSelect(true);
// 			break;
// 		}
// 	}
}

void CGameCard::SetHandCardShoot( BYTE bCardData[],WORD wCardCount )
{
// 	ASSERT(wCardCount<=PIECES);
// 	if (wCardCount>PIECES)
// 	{
// 		return;
// 	}
// 
// 	//设置变量
// 	CCard * pCard;
// 	for(DWORD i=0;i<wCardCount;i++)
// 	{
// 		for(DWORD j=0;j<(DWORD)m_vecHandCard.size();j++)
// 		{
// 			pCard=&m_vecHandCard[j];
// 			if(pCard->GetCard()==bCardData[i]&&!pCard->IsSelect())
// 			{
// 				pCard->SetSelect(true);
// 				break;
// 			}
// 		}
// 	}
// 
// 	//if (m_hWnd!=NULL) RectifyControl();
}

void CGameCard::RemoveHandCardDate( BYTE bCardData[] )
{
// 	for (int i=0;i<PIECES;i++)
// 	{
// 		for (int j=0;j<(int)m_vecHandCard.size();j++)
// 		{
// 			if (m_vecHandCard[j].GetCard() == bCardData[i])
// 			{
// 				m_vecHandCard.erase(m_vecHandCard.begin()+j);
// 			}
// 		}            
// 	}
}

void CGameCard::SetGameView( CGameView *pGameView )
{
	m_pGameView = pGameView;
}

bool CGameCard::SetShootCard( const BYTE bCardData[], DWORD dwCardCount )
{
// 	bool bChangeStatus=false;
// 
// 	for(DWORD i=0;i<dwCardCount;i++)
// 	{
// 		CCard * pCard = &m_vecHandCard[bCardData[i]];
// 		pCard->SetSelect(true);
// 	}
// 
// 	//弹起扑克
// 	for (WORD j=0;j<m_vecHandCard.size();j++)
// 	{
// 		if (m_vecHandCard[j].m_bSelect) 
// 		{
// 			int ileft = (1014-((m_vecHandCard.size()-1)*SPACE+IMG_LAND_HANDCARD->GetCelWidth()))/2;//牌左边坐标
// 			m_vecHandCard[j].SetCardPos(ileft+j*SPACE,HANDLE_CARD_TOP-UPSPACE);
// 			bChangeStatus=true;
// 			m_vecHandCard[j].m_bSelect=true;
// 			break;
// 		}
// 	}
// 
// 
// 	return bChangeStatus;
	return true;
}

int CGameCard::GetHandCardShoot( BYTE bCardData[] )
{
// 	//变量定义
// 	DWORD bShootCount=0L;
// 	DWORD dwCardCount=(DWORD)m_vecHandCard.size();
// 
// 	//搜索扑克
// 	CCard * pCard=NULL;
// 	for (DWORD i=0;i<dwCardCount;i++)
// 	{
// 		//获取扑克
// 		pCard=&m_vecHandCard[i];
// 
// 		//扑克分析
// 		if (pCard->IsSelect())
// 		{
// 			bCardData[bShootCount++]=pCard->GetCard();
// 		}
// 	}
// 
// 	return bShootCount;
	return 0;
}

void CGameCard::SetHandAllShoot( bool bSelect )
{
// 	//设置扑克
// 	for (DWORD i=0;i<m_vecHandCard.size();i++)
// 	{
// 		//获取扑克
// 		m_vecHandCard[i].SetSelect(bSelect);
// 	}
}

WORD CGameCard::GetOutCard( CCard cbCardData[], WORD wBufferCount )
{
// 	//变量定义
// 	WORD wShootCount=0;
// 
// 	for (int i=0;i<m_vecHandCard.size();i++)
// 	{
// 		//效验参数
// 		ASSERT(wBufferCount>wShootCount);
// 		if (wBufferCount<=wShootCount) break;
// 		if (m_vecHandCard[i].IsSelect())
// 		{
// 			cbCardData[wShootCount].m_cCard=m_vecHandCard[i].m_cCard;
// 			cbCardData[wShootCount].m_CardX = m_vecHandCard[i].m_CardX;
// 			cbCardData[wShootCount].m_CardY = m_vecHandCard[i].m_CardY;
// 			wShootCount++;
// 		}
// 	}
// 
// 	return wShootCount;
	return 0;
}

bool CGameCard::SetOutCardData( const BYTE cbOutCardData[], WORD wOutCardCount,WORD wViewChairID,WORD wOutCardShowCount/*=0*/ )
{
// 	//效验参数
// 	ASSERT(wOutCardCount<=MAX_COUNT);
// 	if (wOutCardCount>MAX_COUNT) return false;
// 
// 	m_vecSendCard[wViewChairID].clear();
// 	//设置扑克
// 	for (WORD i=0;i<wOutCardShowCount;i++)
// 	{
// 		m_vecSendCard[wViewChairID].push_back(cbOutCardData[i]);
// 	}
// 
// 	return true;
	return true;
}

void CGameCard::DrawTextString( Sexy::Graphics* g, const SexyString& theString, int theX, int theY, int iWidth,int itheJustification, Sexy::Color crText, Sexy::Color crFrame )
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
//初始化扑克
DWORD CGameCard::InitCardData( WORD wViewChair,const BYTE bCardData[], DWORD dwCardCount )
{

	m_vecHandCard[wViewChair].clear();
	//设置扑克
	for (DWORD i=0;i<dwCardCount;i++)
	{
		CCard tempCard(bCardData[i]);
		m_vecHandCard[wViewChair].push_back(tempCard);
	}

// 	for (int i=0; i<GAME_PLAYER; i++)
// 	{
// 		if (i==2) continue;
// 		for (int j=0; j<MAX_COUNT; i++)
// 		{
// 			CCard tempCard(bCardData[i]);
// 			m_vecHandCard[i].push_back(0);
// 		}
// 	}
	return dwCardCount;	
}	

void CGameCard::SetPos( int x, int y )
{
	m_iCardx = x;
	m_iCardy = y;
}

void CGameCard::Reset()
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		m_vecHandCard[i].clear();
		m_vecResultCard[i].clear();
		m_vecSendCard[i].clear();
		m_vecSendCard2[i].clear();
	}
	m_iDisTrans = 255;
	m_bDisappear = false;
}

void CGameCard::setCallBack( CardCallback * pCallBack )
{
	m_pCallBack = pCallBack;
}

void CGameCard::InitSendCardAni( int id )
{
	ASSERT(id >=0 && id<GAME_PLAYER);
	if (id==0)
	{
		ZeroMemory(m_iTransNow, sizeof(m_iTransNow));

		for(int j=0; j<GAME_PLAYER; j++)
		{
			if (m_vecHandCard[j].empty()) continue;
			m_vecHandCard[j][id].m_iTotalFrame = MAX_FRAME;
		}
	}
	else
	{
		for(int j=0; j<GAME_PLAYER; j++)
		{
			if (m_vecHandCard[j].empty()) continue;
			m_vecHandCard[j][id].m_iTotalFrame = MAX_FRAME;
		}
	}
	m_bSendCard = true;
}

bool CGameCard::CardAddFrame( int id )
{
	ASSERT(id >=0 && id<GAME_PLAYER);
	for(int j=0; j<GAME_PLAYER; j++)
	{
		if (m_vecHandCard[j].empty()) continue;
		if (m_vecHandCard[j][id].m_iNowFrame < m_vecHandCard[j][id].m_iTotalFrame )
		{
			m_vecHandCard[j][id].m_iNowFrame ++;
			m_iTransNow[id] += 255/MAX_FRAME;
			m_iTransNow[id] = (m_iTransNow[id]>255)?255:m_iTransNow[id];
			/*return false;*/
		}
		else
		{
			return false;
		}
	}
	return true;
}

void CGameCard::AniTimer()
{
	if (m_bSendCard)
	{
		int iID = -1;
		for (int i=0; i<MAX_COUNT; i++)
		{
			//_log("RatioxLog","每一帧%d",GetTickCount());
			if(iID != -1) break;
			if(CardAddFrame(i))
			{

				iID = i;
				//printf("GAME_BANKER iID=[%d]\n" , iID);
				break;
			}
		}
		if(IsAllFinish() && iID != -1)
		{
			//_log("RatioxLog","发每张牌结束%d\n",GetTickCount());
			m_bSendCard = false;
			m_pCallBack->OnMoveFinish(iID);
			//printf("发完第[%d]张牌\n", iID);

		}
	}
}

bool CGameCard::IsAllFinish()
{
	for (int i=0; i<MAX_COUNT; i++)
	{
		for (int j=0; j<GAME_PLAYER; j++)
		{
			if (m_vecHandCard[j].empty()) continue;
			if (m_vecHandCard[j][i].m_iNowFrame < m_vecHandCard[j][i].m_iTotalFrame )
			{
				return false;
			}
		}
	}
	return true;
}
//设置摊牌
void CGameCard::SetOutCard( int iIndex, BYTE cbOutCard[] )
{
	ASSERT(iIndex >=0 && iIndex<GAME_PLAYER);

	if (m_vecHandCard[iIndex].empty()) return;
	m_vecSendCard[iIndex].clear();
	m_vecSendCard2[iIndex].clear();
	
	for (int i=0; i<MAX_COUNT; i++)
	{
		if (m_vecHandCard[iIndex][i].GetCard() == cbOutCard[0]|| m_vecHandCard[iIndex][i].GetCard() == cbOutCard[1])
		{
			m_vecSendCard[iIndex].push_back(m_vecHandCard[iIndex][i]);
		}
		else
		{
			m_vecSendCard2[iIndex].push_back(m_vecHandCard[iIndex][i]);
		}
	}
	m_vecHandCard[iIndex].clear();

}

void CGameCard::RemoveMyAllCard()
{
	m_vecHandCard[2].clear();
	m_vecSendCard[2].clear();
	m_vecSendCard2[2].clear();
}

void CGameCard::SetResultCardData()
{
	for(int i=0; i<GAME_PLAYER; i++)
	{
		m_vecResultCard[i].clear();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_vecHandCard[i].size()<=0 && m_vecSendCard[i].size()<=0 && m_vecSendCard2[i].size()<=0)
			continue;

		if (m_vecSendCard[i].size()>0&& m_vecSendCard2[i].size()>0)
		{
			for (int j=0; j<(int)m_vecSendCard[i].size();j++)
			{
				m_vecResultCard[i].push_back(m_vecSendCard[i][j]);
			}
			for (int j=0; j<(int)m_vecSendCard2[i].size();j++)
			{
				m_vecResultCard[i].push_back(m_vecSendCard2[i][j]);
			}
		}
		else
		{
			for (int j=0; j<(int)m_vecHandCard[i].size(); j++)
			{
				m_vecResultCard[i].push_back(m_vecHandCard[i][j]);
			}
		}
	}


}

void CGameCard::Update()
{
	

}

void CGameCard::CardDisAppearFrame()
{
	if(m_bDisappear)
	{
		if(m_iDisTrans > 0)
		{
			m_iDisTrans  -= 255/MAX_DISAPPEAR_FRAME;
			if(m_iDisTrans<0)m_iDisTrans = 0;
		}
		else
		{
			Reset();
		}
	}
}

void CGameCard::SetCardDisAppear( bool bDis )
{
	m_iDisTrans = 255;
	m_bDisappear = bDis;
}
// 
// void CGameCard::AniMoveOpenCardFrame()
// {
// 	if (m_bMove)
// 	{
// 
// 	}
// }
// 
// void CGameCard::SetCardMove( bool bMove )
// {
// 	m_bMove = bMove;
// }
