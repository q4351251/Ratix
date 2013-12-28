#include "stdafx.h"
#include "Player.h"
#include "Res.h"
#include "SexyAppBase.h"
#include "widget.h"
#include "WidgetManager.h"
#include "DDImage.h"

using namespace Sexy;

//侧面（每帧的时间）0大亨1富二代2白富美3都市女性4黑帮大佬5千金小姐
const int Cmillisecond[6][4] = {
	200,200,200,200,
	200,200,150,190,
	176,209,150,167,
	230,220,150,100,
	250,133,100,200,
	200,200,200,200
};

const int Zmillisecond[6][4] = {
	167,208,208,208,
	167,208,208,167,
	167,167,167,167,
	167,167,167,167,
	167,292,83,167,
	300,250,167,167
};

const int MYmillisecond[4] = {
	167,167,208,167,
};
//0大亨1富二代2白富美3都市女性4黑帮大佬5千金小姐
const static CPoint cpPosition[GAME_PLAYER][6] = {
	CPoint(149,48),CPoint(56,-71),CPoint(147,25),CPoint(198,58),CPoint(112,78),CPoint(106,24),
	CPoint(0,187),CPoint(1,4),CPoint(0,179),CPoint(-1,198),CPoint(0,208),CPoint(2,172),
	CPoint(0,0),CPoint(0,0),CPoint(0,0),CPoint(0,0),CPoint(0,0),CPoint(0,0),
	CPoint(839,186),CPoint(767,-1),CPoint(838,169),CPoint(876,196),CPoint(838,205),CPoint(801,169),
	CPoint(516,47),CPoint(463,-71),CPoint(517,24),CPoint(588,60),CPoint(535,77),CPoint(517,24),
};

vector<int> Player::m_vecPlayerIndex(0);

Player::Player(void)
{
    srand(GetTickCount());
    if (m_vecPlayerIndex.size() == 0)
    {
        m_vecPlayerIndex.push_back(0);
        m_vecPlayerIndex.push_back(1);
        m_vecPlayerIndex.push_back(2);
        m_vecPlayerIndex.push_back(3);
        m_vecPlayerIndex.push_back(4);
        m_vecPlayerIndex.push_back(5);
    }
	m_palyerX =0;
	m_palyerY =0;
	m_iDirection =false;
	m_PlayerAct = MAX_NORMAL_ACT;
	m_pPlayerActAni = NULL;
	m_pMeActAni1 = NULL;
	m_bShowUserInfo = false;
	m_iSex=0;
	m_pGameApp = (CGameClientApp *)AfxGetApp();
	m_iPlayerIndex = 0;
	//InitPlayerAni(DAI_JI);
}

Player::~Player(void)
{
}

void Player::InitPlayerAni(PlayerAct act)
{

	//AfxMessageBox("");
    Sexy::Image *pImgWoMan[6][4] = {
		IMG_ANI_PLAYER0_Status0,IMG_ANI_PLAYER0_Status1,IMG_ANI_PLAYER0_Status2,IMG_ANI_PLAYER0_Status3,/*IMG_ANI_PLAYER0_Status0,*/
		IMG_ANI_PLAYER1_Status0,IMG_ANI_PLAYER1_Status1,IMG_ANI_PLAYER1_Status2,IMG_ANI_PLAYER1_Status3,/*IMG_ANI_PLAYER0_Status0,*/
		IMG_ANI_PLAYER2_Status0,IMG_ANI_PLAYER2_Status1,IMG_ANI_PLAYER2_Status2,IMG_ANI_PLAYER2_Status3,/*IMG_ANI_PLAYER0_Status0,*/
		IMG_ANI_PLAYER3_Status0,IMG_ANI_PLAYER3_Status1,IMG_ANI_PLAYER3_Status2,IMG_ANI_PLAYER3_Status3,/*IMG_ANI_PLAYER0_Status0,*/
		IMG_ANI_PLAYER4_Status0,IMG_ANI_PLAYER4_Status1,IMG_ANI_PLAYER4_Status2,IMG_ANI_PLAYER4_Status3,/*IMG_ANI_PLAYER0_Status0,*/
		IMG_ANI_PLAYER5_Status0,IMG_ANI_PLAYER5_Status1,IMG_ANI_PLAYER5_Status2,IMG_ANI_PLAYER5_Status3,/*IMG_ANI_PLAYER0_Status0,*/
    };

    Sexy::Image *pImgWoMan_0[6][4] = {
		IMG_ANI_PLAYER00_Status0,IMG_ANI_PLAYER00_Status1,IMG_ANI_PLAYER00_Status2,IMG_ANI_PLAYER00_Status3,/*IMG_ANI_PLAYER00_Status0,*/
		IMG_ANI_PLAYER01_Status0,IMG_ANI_PLAYER01_Status1,IMG_ANI_PLAYER01_Status2,IMG_ANI_PLAYER01_Status3,/*IMG_ANI_PLAYER00_Status0,*/
		IMG_ANI_PLAYER02_Status0,IMG_ANI_PLAYER02_Status1,IMG_ANI_PLAYER02_Status2,IMG_ANI_PLAYER02_Status3,/*IMG_ANI_PLAYER00_Status0,*/
		IMG_ANI_PLAYER03_Status0,IMG_ANI_PLAYER03_Status1,IMG_ANI_PLAYER03_Status2,IMG_ANI_PLAYER03_Status3,/*IMG_ANI_PLAYER00_Status0,*/
		IMG_ANI_PLAYER04_Status0,IMG_ANI_PLAYER04_Status1,IMG_ANI_PLAYER04_Status2,IMG_ANI_PLAYER04_Status3,/*IMG_ANI_PLAYER00_Status0,*/
		IMG_ANI_PLAYER05_Status0,IMG_ANI_PLAYER05_Status1,IMG_ANI_PLAYER05_Status2,IMG_ANI_PLAYER05_Status3,/*IMG_ANI_PLAYER00_Status0,*/
	};


	Sexy::Image *pImgMe[4] ={
		IMG_MYANI_MyStatus0,IMG_MYANI_MyStatus1,IMG_MYANI_MyStatus2,IMG_MYANI_MyStatus3,/*IMG_MYANI_MyStatus0*/
		
	};
	
	int ms;
	if (m_pPlayerActAni==NULL)
	{
		m_pPlayerActAni = new AnimLayer(m_pGameApp, this);
	}
	else
	{
		/*m_pPlayerActAni->StopAnimate(true, false);*/
		delete m_pPlayerActAni;
		m_pPlayerActAni = new AnimLayer(m_pGameApp, this);
	}

	if (m_pMeActAni1 !=NULL);
	{
		delete m_pMeActAni1;
		m_pMeActAni1 = NULL;
	}
	
	if (m_iDirection == 3 || m_iDirection ==4)
	{
		m_pPlayerActAni->SetReversal(true);
	}
	if(act == ZHENG_CHANG)
	{
					
        if (m_iDirection == 2)
        {
            m_pPlayerActAni->Init(pImgMe[0],1);//自己
        }
        else if (m_iDirection%2)
        {
			m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][0],ZHENG_CHANG);	//两侧
			
        }
		else
        {
            m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][0],ZHENG_CHANG);		//正面
        }			

		m_pPlayerActAni->SetFixFrame(0);
		m_pPlayerActAni->SetLoopNum(-100);
		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),300);
	
		/*m_pPlayerActAni->SetEndHide(true);*/
	}
	if (act == DAI_JI)
	{
		
		ms = 300;
		if (m_iDirection == 2)
		{
			m_pPlayerActAni->Init(pImgMe[act-1],1);//自己
			ms = MYmillisecond[act -1];
		}
		else if (m_iDirection%2)
		{
			m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][act-1],DAI_JI);	//两侧
			ms = Cmillisecond[m_iPlayerIndex][act-1];
		}
		else
		{
			m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][act-1],DAI_JI);		//正面
			ms = Zmillisecond[m_iPlayerIndex][act-1];;
		}				
	
		m_pPlayerActAni->SetFixFrame(-1);
		m_pPlayerActAni->SetLoopNum(1);
		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),ms);
		
		m_pPlayerActAni->SetEndHide(true);
	}
	if (act == DAI_JI2)
	{
		ms = 300;
		if (m_iDirection == 2)
		{
			m_pPlayerActAni->Init(pImgMe[0],1);//自己
			ms = MYmillisecond[act -1];
		}
		else if (m_iDirection%2)
		{
			m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][act-1],DAI_JI2);	//两侧
			ms = Cmillisecond[m_iPlayerIndex][act-1];
		}
		else
		{
			m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][act-1],DAI_JI2);		//正面
			ms = Zmillisecond[m_iPlayerIndex][act-1];;
		}		

		m_pPlayerActAni->SetFixFrame(-1);
		m_pPlayerActAni->SetLoopNum(1);

		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),ms);
		
		m_pPlayerActAni->SetEndHide(true);
	}
	if (act ==HAPPY)
	{
		ms = 300;
		if (m_iDirection == 2)
		{
			m_pPlayerActAni->Init(pImgMe[act-1],HAPPY);//自己
			ms = 150;
			if (m_pMeActAni1!=NULL)
			{
				delete m_pMeActAni1;
			}
			m_pMeActAni1 = new Sexy::AnimLayer(m_pGameApp, this);
			m_pMeActAni1->Init(pImgMe[act-2],MY_ACT_ONE);
			m_pMeActAni1->SetLoopNum(1);
			m_pMeActAni1->SetEndHide(true);
			m_pMeActAni1->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),167);
			
			m_pPlayerActAni->Init(pImgMe[act-1],HAPPY);
			m_pPlayerActAni->SetLoopNum(1);
			m_pPlayerActAni->SetEndHide(true);


		}
		else
		{
			if (m_iDirection%2)
			{
				m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][act-1],HAPPY);	//两侧
				ms = Cmillisecond[m_iPlayerIndex][act-1];
			}
			else
			{
				m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][act-1],HAPPY);		//正面
				ms = Zmillisecond[m_iPlayerIndex][act-1];;
			}	
			m_pPlayerActAni->SetFixFrame(-1);
			m_pPlayerActAni->SetLoopNum(1);

			m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),ms);

			m_pPlayerActAni->SetEndHide(true);
		}

	}
	if (act ==JING_YA)
	{
		ms = 300;
		if (m_iDirection == 2)
		{
			m_pPlayerActAni->Init(pImgMe[0],1);//自己
			ms = 300;
		}
		else if (m_iDirection%2)
		{
			m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][0],1);	//两侧
		}
		else
		{
			m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][0],1);		//正面
		}			

		m_pPlayerActAni->SetFixFrame(-1);
		m_pPlayerActAni->SetLoopNum(1);

		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),300);
		
		m_pPlayerActAni->SetEndHide(true);
	}
	if (act ==JU_SANG)
	{
		ms = 300;
		if (m_iDirection == 2)
		{
			m_pPlayerActAni->Init(pImgMe[act-1],JU_SANG);//自己
			ms = MYmillisecond[act -1];
		}
		else if (m_iDirection%2)
		{
			m_pPlayerActAni->Init(pImgWoMan_0[m_iPlayerIndex][act-1],JU_SANG);	//两侧
			ms = Cmillisecond[m_iPlayerIndex][act-1];
		}
		else
		{
			m_pPlayerActAni->Init(pImgWoMan[m_iPlayerIndex][act-1],JU_SANG);		//正面
			ms = Zmillisecond[m_iPlayerIndex][act-1];;
		}			

		m_pPlayerActAni->SetFixFrame(-1);
		m_pPlayerActAni->SetLoopNum(1);

		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),ms);
		
		m_pPlayerActAni->SetEndHide(true);
	}
}

void Player::Draw( Sexy::Graphics* g )
{
	if (m_pPlayerActAni != NULL)
	{
		m_pPlayerActAni->Draw(g);
	}
	if (m_pMeActAni1 != NULL)
	{
		m_pMeActAni1->Draw(g);
	}
}

void Player::Update()
{
	//Widget::Update();
	if (m_pPlayerActAni != NULL)
	{
		m_pPlayerActAni->Update();
	}
	if (m_pMeActAni1 != NULL)
	{
		m_pMeActAni1->Update();
	}
	//m_bShowUserInfo = false;
}

void Player::SetPlayerSex( int isex/*=-1*/ )
{
 	if (isex == -1)
 	{
 		int i = rand()%m_vecPlayerIndex.size();
 		 m_iPlayerIndex = m_vecPlayerIndex[i];
 		 m_vecPlayerIndex.erase(m_vecPlayerIndex.begin()+i);
 	}
 	else
 	{
 		 /*m_iPlayerIndex =0;*/
 		 m_vecPlayerIndex.push_back(m_iPlayerIndex);
 		 int i = rand()%m_vecPlayerIndex.size();
 		 m_iPlayerIndex = m_vecPlayerIndex[i];
 		 m_vecPlayerIndex.erase(m_vecPlayerIndex.begin()+i);
 
 	}

	/*m_iPlayerIndex = 4;*/
	if (m_iDirection == 0|| m_iDirection==4||m_iDirection==1||m_iDirection==3)
	{
		SetPlayerPos(cpPosition[m_iDirection][m_iPlayerIndex].x, cpPosition[m_iDirection][m_iPlayerIndex].y);
	}
	
	/*m_iPlayerIndex =4;*/
	/*m_iSex = isex;   */ 
}

void Player::SetPlayerPos(int x,int y)
{
	m_palyerX = x;
	m_palyerY = y;
}

void Player::SetReversal( bool bReversal )
{
    m_bReversal = bReversal;
    m_pPlayerActAni->SetReversal(bReversal);
}

void Player::SetDirection(int iDirection)
{
	m_iDirection = iDirection;	
}

void Player::SetStatus(PlayerAct act)
{
	m_PlayerAct = act;
	//初始化动画
	InitPlayerAni(act);
}

void Player::OnAniFinish( int id )
{
	if(id == 7)
	{
		m_pPlayerActAni->ShowAnimate(Sexy::Point(m_palyerX,m_palyerY),Sexy::Point(m_palyerX,m_palyerY),208);
	}
	else
	{
		SetStatus(ZHENG_CHANG);
	}
	
}


//void Player::MouseMove( int x, int y )
//{
//	m_bShowUserInfo = true;
//}
