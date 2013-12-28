#include "StdAfx.h"
#include "LoadWidget.h"
#include "GameClient.h"

#include "WidgetManager.h"
#include "Color.h"
#include "Graphics.h"



LoadWidget::LoadWidget(CGameClientApp *pApp)
{
	mApp = pApp;
}
LoadWidget::~LoadWidget()
{
	delete m_pImgLoadBar;
}

void LoadWidget::Init(void)
{
	m_pImgBg = (Sexy::Image *)mApp->GetImage(".\\Res\\RatiOx\\Img\\loader_bg.jpg");
	m_pImgLoadBar = (Sexy::Image *)mApp->GetImage(".\\Res\\RatiOx\\Img\\loader_bar.png");
}

void LoadWidget::AddedToManager(Sexy::WidgetManager* theWidgetManager)
{

}

void LoadWidget::RemovedFromManager(Sexy::WidgetManager* theWidgetManager)
{

}

void LoadWidget::ButtonDepress(int theId)
{
	
}

void LoadWidget::Draw(Sexy::Graphics* g)
{
	//g->SetColor(Sexy::Color(0,0,0));
	//g->FillRect(0, 0, mWidth, mHeight);
	g->DrawImage(m_pImgBg, 0, 0);
	int loaderBarWidth = m_pImgLoadBar->GetWidth();
	int drawWidth = (int)(mApp->GetLoadingThreadProgress() * loaderBarWidth);
	/*AfxMessageBox("");*/
	if (drawWidth > 0)
	{
		g->DrawImage(m_pImgLoadBar, mWidth / 2 - loaderBarWidth / 2, 636,
			Sexy::Rect(0, 0, drawWidth, m_pImgLoadBar->GetHeight()));
	}

}

void LoadWidget::LoadingComplete()
{

}
