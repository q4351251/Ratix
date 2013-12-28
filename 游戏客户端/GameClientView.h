#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
class CGameView;
//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//函数定义
public:
	
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();

	CGameView						*m_pGameView;						//d2d窗口类
	CSkinRichEdit					m_ChatMessage;						//聊天组件
	//控制接口
public:
	//重置界面
	virtual void ResetGameView();

	//继承函数
protected:
	//调整控件
	virtual void RectifyGameView(INT nWidth, INT nHeight);
	//绘画界面
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//消息函数
public:
	//创建函数
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//消息截取
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()		

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//////////////////////////////////////////////////////////////////////////
