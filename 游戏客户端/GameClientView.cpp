#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include "SexyAppBase.h"



BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

//构造函数
CGameClientView::CGameClientView()
{
	m_pGameView = NULL;
	return;
}

//析构函数
CGameClientView::~CGameClientView()
{
}

//重置界面
void CGameClientView::ResetGameView()
{
	return ;
}

//调整控件
VOID CGameClientView::RectifyGameView(INT nWidth, INT nHeight)
{
	 m_ChatMessage.MoveWindow(CRect(821,598,1007,677));
	return;
}

//绘画界面
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//创建函数
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	//设置窗体
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);


	m_ChatMessage.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL ,CRect(0,0,0,0) , this , 1);
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(27,9,0));
	//SetWindowLong(m_ChatMessage.m_hWnd, GWL_EXSTYLE, GetWindowLong(m_ChatMessage.m_hWnd,GWL_EXSTYLE)|WS_EX_TRANSPARENT); 

	CHARFORMAT cf;
	m_ChatMessage.GetSelectionCharFormat(cf);
	cf.dwEffects&=~CFE_BOLD;//设置粗体，取消用cf.dwEffects&=~CFE_BOLD;
	cf.crTextColor = RGB(255,255,255);//设置颜色
	cf.dwMask|=CFM_SIZE;
	cf.yHeight =180;//设置高度
	cf.dwMask|=CFM_FACE;
	strcpy(cf.szFaceName ,_T("宋体"));//设置字体

	m_ChatMessage.SetDefaultCharFormat(cf);
	m_ChatMessage.SetReadOnly(true);
	m_ChatMessage.HideSelection(TRUE , true);
	m_ChatMessage.ShowWindow(SW_HIDE);

	return 0;
}



//消息截取
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	return FALSE;
	if (pMsg->message == WM_KEYDOWN)
	{
		
		int i=0;
	}
	return __super::PreTranslateMessage(pMsg);
}

LRESULT CGameClientView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_SHOWCHAT)
	{
		m_ChatMessage.ShowWindow(SW_SHOW);
	}
	return CGameFrameViewGDI::WindowProc(message, wParam, lParam);
}

void CGameClientView::OnKillFocus(CWnd* pNewWnd)
{
	CGameFrameViewGDI::OnKillFocus(pNewWnd);
	//SetFocus();
	// TODO: 在此处添加消息处理程序代码
}
