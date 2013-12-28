#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

#define	WM_SHOWCHAT					    WM_USER+101					    //显示对话框
//////////////////////////////////////////////////////////////////////////
class CGameClientApp;
class CGameView;
//游戏对话框
class CGameClientDlg : public CGameFrameDlg
{
public:
	CGameClientView					m_GameClientView;					//游戏视图
	CGameView						*m_pGameView;						//D2D游戏视图
	CGameClientApp					*m_pGameApp;						

	//状态变量
public:
    
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//常规继承
private:
	//系统消息
	virtual bool __cdecl InsertNoticeMessage(LPCTSTR pszString , BYTE cbType);

	virtual bool SetDlgTitle(){return true;}
	//初始函数
	virtual bool InitGameFrame();
	//初始函数
	virtual BOOL OnInitDialog();
	//重置框架
	virtual void ResetGameFrame();
	//游戏帮助 新加
	virtual void OnGameRuleSet(){};
	//游戏设置
	virtual void OnGameOptionSet();
	//时间消息
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//旁观状态
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize){return;}
	//网络消息
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//游戏场景
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);
	//关闭按钮
	virtual void OnCancel(); 
	//是否可以设置声音大小
	virtual bool IsSetSound(){return true;};

public:
	//设置主窗口
	void	SetGameView(CGameView *pGameView);

	//功能函数
	void TimeOutCloseWidget();
	//隐藏聊天组件
	void ShowChat(bool bShow);
	//出现筹码不足信息框
	void ShowHintMsg();
	//消息处理
protected:
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

	//消息映射
protected:
	//定时器消息
	afx_msg void OnTimer(UINT nIDEvent);

	//用户事件
private:
	//用户进入
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//用户离开
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//用户积分
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);


	//开关声音
	LRESULT OnMessageSound(WPARAM wParam, LPARAM lParam);
	//显示或隐藏连天组件
	LRESULT OnMessageShowChat(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

extern HANDLE g_hEventVec;

//////////////////////////////////////////////////////////////////////////
