#pragma once
#include "ForestBattle.h"
#include "GameAudio.h"
class SceneGameBegin
{
  public:
    int Init();
    int Update();
    int Render() const;
    int Destroy();

  private:
    void StartBattle();
    void ReturnToTitle();
    void RenderBattle() const;
    int texture = -1;
    int forestBackground = -1;
    int ruinsBackground = -1;
    int caveBackground = -1;
    int citadelBackground = -1;
    int playerSprite = -1;
    int forestEnemySprite = -1;
    int ruinsEnemySprite = -1;
    int caveEnemySprite = -1;
    int bossSprite = -1;
    int potionSprite = -1;
    int superPotionSprite = -1;
    int rewardIcons = -1;
    int mainBgm = -1;
    int font = -1;
    int smallFont = -1;
    int selected = 0;
    int menuWidths[3]{};
    enum class Page
    {
        Menu,
        Battle,
        Settings
    };
    Page page = Page::Menu;
    bool previous[256]{};
    bool windowed = true;
    int settingSelected = 0;
    int volumePercent = 100;
    bool audioAvailable = false;
    GameAudio audio;
    ForestBattle battle;
};
