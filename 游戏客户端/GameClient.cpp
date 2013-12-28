#include "Stdafx.h"
#include "GameView.h"
#include <io.h>
#include "GameClient.h"
#include "GameClientDlg.h"
#include "ResourceManager.h"
#include "WidgetManager.h"
#include "SoundManager.h"
#include "Common.h"
#include "SoundPlayer.h"
#include "MusicInterface.h"
#include "LoadWidget.h"
#include "Res.h"


//应用程序对象
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

using namespace Sexy;

//构造函数
CGameClientApp::CGameClientApp()
{
	mProdName = "CddClient";
	mProductVersion = "v1.0.1.2";
	mTitle = _S("通比牛牛 ") + StringToSexyStringFast(mProductVersion);
	mRegKey = "Gameplus\\kong";
	m_pGameView = NULL;
	m_pGameClientDlg = NULL;
	mWidth = 1015;
	mHeight = 727;

	/*AfxMessageBox("");*/

}

//析构函数
CGameClientApp::~CGameClientApp() 
{

	delete m_pGameView;
}

//////////////////////////////////////////////////////////////////////////
BOOL CGameClientApp::InitInstance()
{
	//设置工作目录
	ZeroMemory(g_szWorkDir,sizeof(g_szWorkDir));
	GetModuleFileName(AfxGetInstanceHandle(),g_szWorkDir,sizeof(g_szWorkDir));
	int nModuleLen=lstrlen(g_szWorkDir);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	g_szWorkDir[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(g_szWorkDir);
	if ('\\'!=g_szWorkDir[lstrlen(g_szWorkDir)-1]) g_szWorkDir[lstrlen(g_szWorkDir)-1]='\\';
	BOOL bRt = CGameFrameApp::InitInstance();
	((CGameClientDlg *)m_pGameClientDlg)->m_GameClientView.SetActiveWindow();
	//AfxMessageBox("f");
	Init(((CGameClientDlg *)AfxGetMainWnd())->m_GameClientView.GetSafeHwnd());
	::SetFocus(mHWnd);
	
	//取得背景音数目
	
// 	mMusicInterface->LoadMusic(0, "music/music.mo3");
// 	mMusicInterface->FadeIn(0, 0, 0.002, false);
	//RehupFocus();
	m_b3DAccelerated = Is3DAccelerated();
	Set3DAcclerated(true);

	InitLoadWidget();
	_beginthread(StartThread, 0, this);                  // 创建子线程
	return bRt;
}

void CGameClientApp::StartThread( void *p )
{
	CGameClientApp *pGameApp = (CGameClientApp *)p;

	pGameApp->Start();
}

void CGameClientApp::LoadingThreadProc()
{
	if (!mResourceManager->ParseResourcesFile("Res\\RatiOx\\properties\\resources.xml"))
		ShowResourceError(true);
	mNumLoadingThreadTasks = mResourceManager->GetNumResources("Init");
	mResourceManager->StartLoadResources("Init");
	while (mResourceManager->LoadNextResource())
	{
		mCompletedLoadingThreadTasks++;
		if (mShutdown)
			return;
		m_pLoadWidget->MarkDirty();
	}
	if (mResourceManager->HadError() || !ExtractInitResources(mResourceManager))
	{		
		ShowResourceError(false);
		mLoadingFailed = true;
		return;
	}
	
}

void CGameClientApp::LoadingThreadCompleted()
{
	SexyAppBase::LoadingThreadCompleted();

	m_pSoundPlayer = new CSoundPlayer(this);
	m_pGameView = new CGameView;
	m_pGameView->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(m_pGameView);
	((CGameClientDlg *)m_pGameClientDlg)->SetGameView(m_pGameView);
	SetRecvNetMsgWidget(m_pGameView);
	SetGameViewFocus();
}

void CGameClientApp::DestroyGameView()
{
	Shutdown();
	printf("\n----Shutdown-----\n");
	mWidgetManager->RemoveAllWidgets();
	Set3DAcclerated(m_b3DAccelerated);
	delete m_pSoundPlayer;
}

void CGameClientApp::SetGameViewFocus()
{
	mWidgetManager->SetFocus(m_pGameView);
}

void CGameClientApp::OnGameNetMessage(WPARAM wParam, LPARAM lParam)
{
	m_pGameView->OnGameMessage(wParam, lParam);
}

void CGameClientApp::InitLoadWidget()
{
	m_pLoadWidget = new LoadWidget(this);
	m_pLoadWidget->Init();
	m_pLoadWidget->Resize(0,0,mWidth , mHeight);
	mWidgetManager->AddWidget(m_pLoadWidget);
}

void CGameClientApp::EnforceCursor()
{
 	if (m_pGameView == NULL)
 	{
		__super::EnforceCursor();		
 	}
	else
	{
		__super::EnforceCursor();
	}
	
}
