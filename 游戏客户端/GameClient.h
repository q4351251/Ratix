#pragma once

#include "Resource.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
#include "SexyAppBase.h"

class CSoundPlayer;
class LoadWidget;
class CGameView;
//应用程序类
class CGameClientApp : public CGameFrameApp,public Sexy::SexyAppBase
{
	//函数定义
public:
	//构造函数
	CGameClientApp();
	//析构函数
	virtual ~CGameClientApp();
	
	CGameFrameDlg		*m_pGameClientDlg;

	CGameView			*m_pGameView;

	//播放音乐类
	CSoundPlayer		*m_pSoundPlayer;

	//加载进度显示类
	LoadWidget			*m_pLoadWidget;

	//是否开启3D加速
	bool				m_b3DAccelerated;
	//继承函数
public:
	//创建主窗口
	virtual CGameFrameDlg * GetGameFrameDlg() { 
		if (m_pGameClientDlg == NULL)m_pGameClientDlg = new CGameClientDlg;	return m_pGameClientDlg;
	}

	virtual BOOL InitInstance();

	void	InitLoadWidget();

	virtual void	LoadingThreadProc();

	virtual void	LoadingThreadCompleted();

	void DestroyGameView();							//销毁GameView窗口
	void SetGameViewFocus();						//设置焦点到GameView

	static bool bStart;
	static void StartThread(void *p);

	virtual void OnGameNetMessage(WPARAM wParam, LPARAM lParam);

	virtual void			EnforceCursor();
};

//////////////////////////////////////////////////////////////////////////

//应用程序对象
extern CGameClientApp theApp;


//////////////////////////////////////////////////////////////////////////
