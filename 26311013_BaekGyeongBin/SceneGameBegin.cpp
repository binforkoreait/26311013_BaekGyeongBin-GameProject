#include "SceneGameBegin.h"
#include "KoreanText.h"
#include "glc2d.h"
#include <algorithm>
#include <string>

namespace
{
const char *const menuLabels[] = {"START", "SETTING", "EXIT"};
}

int SceneGameBegin::Init()
{
    const char *path = "resource/texture/Title.png";
    texture = g2_TextureLoad(path, 0);
    if (texture < 0)
    {
        MessageBoxW(g2_GetHwnd(),
                    L"표지 이미지를 불러올 수 없습니다. resource/texture/Title.png를 확인하세요.",
                    L"Dungeon Escape", MB_ICONERROR);
        return -1;
    }

    forestBackground = g2_TextureLoad("resource/texture/Forest.png", 0);
    playerSprite = g2_TextureLoad("resource/texture/Player.png", 0);
    forestEnemySprite = g2_TextureLoad("resource/texture/ForestEnemy.png", 0);
    potionSprite = g2_TextureLoad("resource/texture/Potion.png", 0);
    superPotionSprite = g2_TextureLoad("resource/texture/SuperPotion.png", 0);

    font = g2_FontCreate("Noto Serif KR", 40);
    smallFont = g2_FontCreate("Noto Serif KR", 18);

    HDC dc = GetDC(g2_GetHwnd());
    HFONT measuringFont = CreateFontA(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                      DEFAULT_PITCH | FF_DONTCARE, "Noto Serif KR");
    HGDIOBJ oldFont = SelectObject(dc, measuringFont);

    for (int i = 0; i < 3; ++i)
    {
        SIZE size{};
        std::string localLabel = ToLocalText(menuLabels[i]);
        GetTextExtentPoint32A(dc, localLabel.c_str(), static_cast<int>(localLabel.size()), &size);
        menuWidths[i] = size.cx;
    }

    SelectObject(dc, oldFont);
    DeleteObject(measuringFont);
    ReleaseDC(g2_GetHwnd(), dc);

    audioAvailable = audio.Init();
    if (audioAvailable)
    {
        audioAvailable = audio.SetVolume(volumePercent);
    }

    mainBgm = g2_SoundLoad("resource/sound/Main bgm.mp3");
    if (mainBgm >= 0)
    {
        g2_SoundPlay(mainBgm, true);
    }
    else
    {
        MessageBoxW(g2_GetHwnd(), L"resource/sound/Main bgm.mp3를 불러올 수 없습니다.",
                    L"Dungeon Escape - 소리", MB_ICONWARNING);
    }

    return 0;
}
int SceneGameBegin::Update()
{
    const int keys[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_RETURN, VK_ESCAPE, 'A', 'B', 'Q'};

    bool pressed[256]{};

    for (int key : keys)
    {
        bool down = GetForegroundWindow() == g2_GetHwnd() && (GetAsyncKeyState(key) & 0x8000) != 0;
        pressed[key] = down && !previous[key];
        previous[key] = down;
    }
    if (pressed[VK_ESCAPE])
    {
        if (page == Page::Battle)
        {
            battle.Back();
        }
        else if (page != Page::Menu)
        {
            page = Page::Menu;
        }
        else
        {
            PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
        }
        return 0;
    }
    if (page == Page::Menu)
    {
        if (pressed[VK_UP])
        {
            if (selected == 0)
            {
                selected = 2;
            }
            else
            {
                selected--;
            }
        }
        if (pressed[VK_DOWN])
        {
            if (selected == 2)
            {
                selected = 0;
            }
            else
            {
                selected++;
            }
        }
        if (pressed[VK_RETURN])
        {
            if (selected == 0)
            {
                StartBattle();
            }
            else if (selected == 1)
            {
                page = Page::Settings;
                settingSelected = 0;
            }
            else
            {
                PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
            }
        }
    }
    else if (page == Page::Battle)
    {
        if (pressed['A'])
        {
            battle.QuickAttack();
        }
        else if (pressed['B'])
        {
            battle.QuickRun();
        }
        else if (pressed['Q'])
        {
            battle.QuickPotion();
        }
        if (pressed['A'] || pressed['B'] || pressed['Q'])
        {
            return 0;
        }
        if (pressed[VK_UP])
        {
            battle.MoveUp();
        }
        if (pressed[VK_DOWN])
        {
            battle.MoveDown();
        }
        if (pressed[VK_RETURN])
        {
            ForestBattle::Menu menu = battle.CurrentMenu();
            if (menu == ForestBattle::Menu::Pause)
            {
                if (battle.Selection() == 0)
                {
                    battle.Back();
                }
                else
                {
                    ReturnToTitle();
                }
            }
            else if (menu == ForestBattle::Menu::Finished)
            {
                if (battle.Selection() == 0)
                {
                    battle.Start();
                }
                else
                {
                    ReturnToTitle();
                }
            }
            else
            {
                battle.Confirm();
            }
        }
    }
    else if (page == Page::Settings)
    {
        if (pressed[VK_UP])
        {
            if (settingSelected == 0)
            {
                settingSelected = 2;
            }
            else
            {
                settingSelected--;
            }
        }
        if (pressed[VK_DOWN])
        {
            if (settingSelected == 2)
            {
                settingSelected = 0;
            }
            else
            {
                settingSelected++;
            }
        }
        if (settingSelected == 0 && (pressed[VK_LEFT] || pressed[VK_RIGHT]))
        {
            int next = volumePercent;
            if (pressed[VK_RIGHT])
            {
                next += 10;
            }
            else
            {
                next -= 10;
            }
            next = (std::max)(0, (std::min)(100, next));
            audioAvailable = audio.SetVolume(next);
            if (audioAvailable)
            {
                volumePercent = next;
            }
        }
        else if (settingSelected == 1 &&
                 (pressed[VK_LEFT] || pressed[VK_RIGHT] || pressed[VK_RETURN]))
        {
            g2_ChangeWindow(!windowed);
            windowed = g2_GetWindowMode();
        }
        else if (settingSelected == 2 && pressed[VK_RETURN])
        {
            page = Page::Menu;
        }
    }
    else if (pressed[VK_RETURN])
    {
        page = Page::Menu;
    }
    return 0;
}
void SceneGameBegin::StartBattle()
{
    if (mainBgm >= 0)
    {
        g2_SoundStop(mainBgm);
    }
    battle.Start();
    page = Page::Battle;
}
void SceneGameBegin::ReturnToTitle()
{
    page = Page::Menu;
    if (mainBgm >= 0)
    {
        g2_SoundPlay(mainBgm, true);
    }
}
int SceneGameBegin::Render() const
{
    if (texture < 0)
    {
        return 0;
    }
    const int width = g2_TextureWidth(texture);
    const int height = g2_TextureHeight(texture);
    if (width <= 0 || height <= 0)
    {
        return 0;
    }
    // 화면 비율을 유지하면서 창 전체를 채운다.
    const float scale = (std::max)(float(g2_GetScnW()) / width, float(g2_GetScnH()) / height);
    VEC2 scaling(scale, scale);
    VEC2 position((g2_GetScnW() - width * scale) / 2, (g2_GetScnH() - height * scale) / 2);
    if (page == Page::Battle && forestBackground >= 0)
    {
        int forestWidth = g2_TextureWidth(forestBackground);
        int forestHeight = g2_TextureHeight(forestBackground);
        float forestScale =
            (std::max)(float(g2_GetScnW()) / forestWidth, float(g2_GetScnH()) / forestHeight);
        VEC2 forestScaling(forestScale, forestScale);
        VEC2 forestPosition((g2_GetScnW() - forestWidth * forestScale) / 2,
                            (g2_GetScnH() - forestHeight * forestScale) / 2);
        g2_Draw2D(forestBackground, nullptr, &forestPosition, &forestScaling, nullptr, 0,
                  0xff777777);
    }
    else
    {
        g2_Draw2D(texture, nullptr, &position, &scaling);
    }
    float cx = g2_GetScnW() / 2.0f;
    float top = g2_GetScnH() * 0.70f;
    auto text = [&](int x, int y, const char *value, DWORD color, bool useSmallFont = false) {
        RECT bounds{x, y, int(cx + 420), y + 44};
        RECT shadow{x + 2, y + 2, int(cx + 422), y + 46};
        int textFont = font;
        if (useSmallFont)
        {
            textFont = smallFont;
        }
        std::string localValue = ToLocalText(value);
        g2_FontDrawText(textFont, shadow, 0xff18202a, "%s", localValue.c_str());
        g2_FontDrawText(textFont, bounds, color, "%s", localValue.c_str());
    };
    if (page == Page::Menu)
    {
        for (int i = 0; i < 3; ++i)
        {
            int y = int(top) + i * 64;
            DWORD color = 0xffd5c5a5;
            if (i == selected)
            {
                color = 0xffffd77a;
            }
            int x = int(cx) - menuWidths[i] / 2;
            text(x, y, menuLabels[i], color);
            if (i == selected)
            {
                text(x - 36, y, ">", color);
                text(x + menuWidths[i] + 24, y, "<", color);
            }
        }
    }
    else if (page == Page::Settings)
    {
        text(int(cx) - 50, int(top) - 64, "설정", 0xffffd77a);
        std::string volumeLabel = "사용 불가";
        if (audioAvailable)
        {
            volumeLabel = "<  " + std::to_string(volumePercent) + "%  >";
        }
        const char *labels[] = {"소리", "화면", "돌아가기"};
        for (int i = 0; i < 3; ++i)
        {
            int y = int(top) + i * 64;
            DWORD color = 0xffd5c5a5;
            if (settingSelected == i)
            {
                color = 0xffffd77a;
            }
            text(int(cx) - 290, y, labels[i], color);
            if (settingSelected == i)
            {
                text(int(cx) - 330, y, ">", color);
            }
            if (i == 0)
            {
                text(int(cx) + 10, y, volumeLabel.c_str(), color);
            }
            if (i == 1)
            {
                const char *displayLabel = "< 전체 화면 >";
                if (windowed)
                {
                    displayLabel = "< 창 모드 >";
                }
                text(int(cx) + 10, y, displayLabel, color);
            }
        }
    }
    else if (page == Page::Battle)
    {
        RenderBattle();
    }
    return 0;
}
void SceneGameBegin::RenderBattle() const
{
    const auto line = [&](int y, const std::string &value, DWORD color = 0xffedf0f4) {
        RECT bounds{120, y, g2_GetScnW() - 100, y + 36};
        RECT shadow{122, y + 2, g2_GetScnW() - 98, y + 38};
        std::string localValue = ToLocalText(value);
        g2_FontDrawText(smallFont, shadow, 0xff101720, "%s", localValue.c_str());
        g2_FontDrawText(smallFont, bounds, color, "%s", localValue.c_str());
    };
    const auto sprite = [&](int id, float x, float y, float height) {
        if (id < 0)
        {
            return;
        }
        float scaleFactor = height / g2_TextureHeight(id);
        VEC2 position(x, y);
        VEC2 scale(scaleFactor, scaleFactor);
        g2_Draw2D(id, nullptr, &position, &scale);
    };

    line(55, "숲 - 일반 전투 1 / 3", 0xffffd77a);
    line(95, "플레이어 체력: " + std::to_string(battle.PlayerHp()) + " / 100");
    line(130, "숲의 야수 체력: " + std::to_string(battle.EnemyHp()) + " / 50");
    sprite(playerSprite, 180, 245, 360);
    if (battle.EnemyHp() > 0)
    {
        sprite(forestEnemySprite, 750, 305, 310);
    }
    line(635, battle.Message(), 0xffffd77a);

    std::string options[3];
    int count = 0;
    ForestBattle::Menu menu = battle.CurrentMenu();
    if (menu == ForestBattle::Menu::Main)
    {
        options[0] = "공격";
        options[1] = "도구";
        options[2] = "도망 (성공 확률 60%)";
        count = 3;
    }
    else if (menu == ForestBattle::Menu::Attack)
    {
        options[0] = "기본 공격 - 피해 20";
        options[1] = "강한 공격 - 피해 30, 명중률 75%";
        options[2] = "흡수 공격 - 피해 10, 준 피해의 절반 회복";
        count = 3;
    }
    else if (menu == ForestBattle::Menu::Item)
    {
        options[0] = "회복약 - 체력 30 회복 (" + std::to_string(battle.Potions()) + "개)";
        options[1] = "고급 회복약 - 체력 60 회복 (" + std::to_string(battle.SuperPotions()) + "개)";
        count = 2;
    }
    else if (menu == ForestBattle::Menu::Pause)
    {
        options[0] = "계속하기";
        options[1] = "타이틀로";
        count = 2;
    }
    else if (menu == ForestBattle::Menu::Finished)
    {
        options[0] = "첫 전투 다시 시작";
        options[1] = "타이틀로";
        count = 2;
    }

    for (int i = 0; i < count; ++i)
    {
        std::string choice = options[i];
        if (i == battle.Selection())
        {
            choice = "> " + choice;
        }
        line(700 + i * 45, choice);
        if (menu == ForestBattle::Menu::Item)
        {
            int itemSprite = potionSprite;
            if (i == 1)
            {
                itemSprite = superPotionSprite;
            }
            sprite(itemSprite, 900, float(690 + i * 45), 42);
        }
    }
    if (menu == ForestBattle::Menu::Message)
    {
        line(700, "Enter - 계속");
    }
    else if (menu == ForestBattle::Menu::Attack || menu == ForestBattle::Menu::Item)
    {
        line(865, "Esc - 턴을 쓰지 않고 돌아가기");
    }
    else if (menu == ForestBattle::Menu::Main)
    {
        line(865, "A - 공격   B - 도망   Q - 회복약   Esc - 일시정지");
    }
}
int SceneGameBegin::Destroy()
{
    if (mainBgm >= 0)
    {
        g2_SoundStop(mainBgm);
        g2_SoundRelease(mainBgm);
        mainBgm = -1;
    }
    audio.Destroy();
    if (texture >= 0)
    {
        g2_TextureRelease(texture);
        texture = -1;
    }
    int *battleTextures[] = {&forestBackground, &playerSprite, &forestEnemySprite, &potionSprite,
                             &superPotionSprite};
    for (int *id : battleTextures)
    {
        if (*id >= 0)
        {
            g2_TextureRelease(*id);
            *id = -1;
        }
    }
    return 0;
}
