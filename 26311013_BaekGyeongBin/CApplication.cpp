#include <stdio.h>
#include "CApplication.h"
#include "glc2d.h"

extern CApplication g_app;

int AppUpdate()
{
	return g_app.Update();
}

int AppRender()
{
	return g_app.Render();
}


int CApplication::Init()
{
	InitSdk();

	m_sceneBegin.Init();

	return 0;
}

int CApplication::Update()
{
	printf("Update ...\n\n");

	m_sceneBegin.Update();

	return 0;
}

int CApplication::Render()
{
	printf("Render ...\n\n");

	m_sceneBegin.Render();

	return 0;
}

int CApplication::Destroy()
{
	m_sceneBegin.Destroy();

	g2_DestroyWin();
	return 0;
}

int CApplication::InitSdk()
{

	g2_InitSdk();

	printf("Starting ... \n\n");
	g2_SetFrameMove(AppUpdate);
    g2_SetRender(AppRender);


	g2_CreateWin(m_winPos.x, m_winPos.y
		, m_winSize.cx, m_winSize.cy, m_winName.c_str());


	return 0;
}
