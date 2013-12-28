#ifndef GAME_LOADWIDGET_FILE
#define GAME_LOADWIDGET_FILE

#include "Widget.h"

#pragma once

namespace Sexy
{
	class WidgetManager;
	class Graphics;
}

class CGameClientApp;

class LoadWidget : public Sexy::Widget
{

public:
	LoadWidget(CGameClientApp *pApp);
	~LoadWidget();

private:

	CGameClientApp*			mApp;

	Sexy::Image			*m_pImgLoadBar;
	Sexy::Image			*m_pImgBg;

public:

	void Init(void);

	void AddedToManager(Sexy::WidgetManager* theWidgetManager);

	void RemovedFromManager(Sexy::WidgetManager* theWidgetManager);
	
	virtual void ButtonDepress(int theId);

	virtual void Draw(Sexy::Graphics* g);

	void LoadingComplete();
};

#endif
