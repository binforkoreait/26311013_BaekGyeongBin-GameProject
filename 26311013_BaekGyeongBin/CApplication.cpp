#include "CApplication.h"
#include "glc2d.h"
extern CApplication g_app;
int AppUpdate() { return g_app.Update(); }
int AppRender() { return g_app.Render(); }
int CApplication::Init() {
    g2_InitSdk();
    g2_SetFrameMove(AppUpdate);
    g2_SetRender(AppRender);
    g2_CreateWin(100,30,1280,960,"Dungeon Escape",false);
    if (!g2_GetHwnd()) return -1;
    g2_ChangeWindow(true);
    g2_SetClearColor(0xff101720);
    g2_SetStateShow(false);
    return begin.Init();
}
int CApplication::Update() {
    return begin.Update();
}
int CApplication::Render() { return begin.Render(); }
int CApplication::Destroy() { begin.Destroy(); g2_DestroyWin(); return 0; }
