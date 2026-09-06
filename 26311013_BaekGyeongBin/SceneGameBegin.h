#pragma once
#include "GameAudio.h"
class SceneGameBegin {
public:
    int Init();
    int Update();
    int Render() const;
    int Destroy();
private:
    int texture = -1;
    int mainBgm = -1;
    int font = -1;
    int smallFont = -1;
    int selected = 0;
    int menuWidths[3]{};
    enum class Page { Menu, Start, Settings };
    Page page = Page::Menu;
    bool previous[256]{};
    bool windowed = true;
    int settingSelected = 0;
    int volumePercent = 100;
    bool audioAvailable = false;
    GameAudio audio;
};
