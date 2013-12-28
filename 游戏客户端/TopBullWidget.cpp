#include "Stdafx.h"
#include "TopBullWidget.h"
#include "GameClient.h"
#include "ButtonWidget.h"
#include "WidgetManager.h"
#include "SysFont.h"
#include "Timer.h"

using namespace Sexy; 

CTopBullWidget::CTopBullWidget()
{
	m_pGameApp = (CGameClientApp *)AfxGetApp();

	m_pBtnHide = new ButtonWidget(BTN_HIDE_ID, this);
	m_pBtnHide->SetBtnImage(m_pGameApp,IMG_TOP_BULL_CLOSE);
	AddWidget(m_pBtnHide);
	m_pFontHei = new SysFont("黑体", 16, true);
	m_pTimer = new Timer(this);
	m_pTimer->SetTimer(TIM_BULL_SHOW_ID, 15000);
	ZeroMemory(m_szBullContent, sizeof(m_szBullContent));
}

CTopBullWidget::~CTopBullWidget(void)
{
	RemoveAllWidgets(true);

	delete m_pFontHei;
}

void CTopBullWidget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX,theY,theWidth,theHeight);

	//隐藏消息按钮位置
	m_pBtnHide->Resize(theWidth-IMG_TOP_BULL_CLOSE->GetCelWidth(),5,IMG_TOP_BULL_CLOSE->GetCelWidth(),IMG_TOP_BULL_CLOSE->GetCelHeight());
}

void CTopBullWidget::Update()
{
	//定时器
	m_pTimer->Update();
}

void CTopBullWidget::Draw(Sexy::Graphics *g)
{
	if (wcslen(m_szBullContent) > 0)
	{
		g->DrawImage(IMG_TOP_BULL_BG, 0, 0, mWidth, IMG_TOP_BULL_BG->GetHeight());
		SexyString str(m_szBullContent);
		g->SetFont((Sexy::Font*)(m_pFontHei));
		g->SetColor(Sexy::Color(255, 0, 0));
		WriteString(g, str, mWidth / 2, 20, -1, 0);
	}
}

void CTopBullWidget::OnTimer(int nTimerId)
{
	if(nTimerId == TIM_BULL_SHOW_ID)	//显示公告
	{
		m_pTimer->SetTimer(TIM_BULL_HIDE_ID, 10000);
		memset(m_szBullContent, 0, sizeof(m_szBullContent));
		if (m_strBull.size() != 0)
		{
			_bstr_t t = m_strBull[0].c_str();
			wchar_t* pwchar = (wchar_t*)t;
			wcscpy(m_szBullContent, pwchar);
			SetVisible(true);
			m_strBull.erase(m_strBull.begin());
		}
	}
	else if(nTimerId == TIM_BULL_HIDE_ID)	//隐藏公告
	{
		m_pTimer->KillTimer(TIM_BULL_HIDE_ID);
		SetVisible(false);
	}
}

void CTopBullWidget::ButtonDepress(int theId)
{
	if(theId == BTN_HIDE_ID)
	{
		OnBtnHide();
	}
}

void CTopBullWidget::OnBtnHide()
{
	m_pTimer->KillTimer(TIM_BULL_HIDE_ID);
	SetVisible(false);
}

void CTopBullWidget::AddString( LPCTSTR pszString )
{
	m_strBull.push_back(pszString);
}


