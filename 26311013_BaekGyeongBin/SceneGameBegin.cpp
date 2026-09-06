#include "SceneGameBegin.h"
#include "glc2d.h"
#include <algorithm>
#include <string>
int SceneGameBegin::Init() {
    // Original generated cover artwork.
    const char* path = "resource/texture/Title.png";
    texture = g2_TextureLoad(path, 0);
    if (texture < 0) {
        MessageBoxA(g2_GetHwnd(), "Could not load the cover image. Check resource/texture/Title.png.", "Dungeon Escape", MB_ICONERROR);
        return -1;
    }
    font = g2_FontCreate("Castellar", 40);
    smallFont = g2_FontCreate("Segoe UI", 18);
    // Measure the actual font instead of using estimated per-label offsets.
    HDC dc = GetDC(g2_GetHwnd());
    HFONT measuringFont = CreateFontA(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Castellar");
    HGDIOBJ oldFont = SelectObject(dc, measuringFont);
    const char* labels[] = {"START", "SETTING", "EXIT"};
    for (int i = 0; i < 3; ++i) {
        SIZE size{};
        GetTextExtentPoint32A(dc, labels[i], lstrlenA(labels[i]), &size);
        menuWidths[i] = size.cx;
    }
    SelectObject(dc, oldFont);
    DeleteObject(measuringFont);
    ReleaseDC(g2_GetHwnd(), dc);
    audioAvailable = audio.Init() && audio.SetVolume(volumePercent);
    mainBgm = g2_SoundLoad("resource/sound/Main bgm.mp3");
    if (mainBgm >= 0) g2_SoundPlay(mainBgm, true);
    else MessageBoxA(g2_GetHwnd(), "Could not load resource/sound/Main bgm.mp3.", "Dungeon Escape - Audio", MB_ICONWARNING);
    return 0;
}
int SceneGameBegin::Update() {
    const int keys[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_RETURN, VK_ESCAPE};
    bool pressed[256]{};
    for (int key : keys) {
        bool down = GetForegroundWindow() == g2_GetHwnd() && (GetAsyncKeyState(key) & 0x8000) != 0;
        pressed[key] = down && !previous[key];
        previous[key] = down;
    }
    if (pressed[VK_ESCAPE]) {
        if (page != Page::Menu) page = Page::Menu;
        else PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
        return 0;
    }
    if (page == Page::Menu) {
        if (pressed[VK_UP]) selected = (selected + 2) % 3;
        if (pressed[VK_DOWN]) selected = (selected + 1) % 3;
        if (pressed[VK_RETURN]) {
            if (selected == 0) page = Page::Start;
            else if (selected == 1) { page = Page::Settings; settingSelected = 0; }
            else PostMessage(g2_GetHwnd(), WM_CLOSE, 0, 0);
        }
    } else if (page == Page::Settings) {
        if (pressed[VK_UP]) settingSelected = (settingSelected + 2) % 3;
        if (pressed[VK_DOWN]) settingSelected = (settingSelected + 1) % 3;
        if (settingSelected == 0 && (pressed[VK_LEFT] || pressed[VK_RIGHT])) {
            int next = (std::max)(0, (std::min)(100, volumePercent + (pressed[VK_RIGHT] ? 10 : -10)));
            audioAvailable = audio.SetVolume(next);
            if (audioAvailable) volumePercent = next;
        } else if (settingSelected == 1 && (pressed[VK_LEFT] || pressed[VK_RIGHT] || pressed[VK_RETURN])) {
            g2_ChangeWindow(!windowed);
            windowed = g2_GetWindowMode();
        } else if (settingSelected == 2 && pressed[VK_RETURN]) {
            page = Page::Menu;
        }
    } else if (pressed[VK_RETURN]) page = Page::Menu;
    return 0;
}
int SceneGameBegin::Render() const {
    if (texture < 0) return 0;
    const int width = g2_TextureWidth(texture), height = g2_TextureHeight(texture);
    if (width <= 0 || height <= 0) return 0;
    // Fill the window without stretching or leaving background margins.
    const float scale = (std::max)(float(g2_GetScnW())/width, float(g2_GetScnH())/height);
    VEC2 scaling(scale, scale);
    VEC2 position((g2_GetScnW()-width*scale)/2, (g2_GetScnH()-height*scale)/2);
    g2_Draw2D(texture, nullptr, &position, &scaling);
    float cx = g2_GetScnW() / 2.0f;
    float top = g2_GetScnH() * 0.70f;
    auto text = [&](int x, int y, const char* value, DWORD color, bool useSmallFont = false) {
        RECT bounds{x, y, int(cx + 420), y + 44};
        RECT shadow{x + 2, y + 2, int(cx + 422), y + 46};
        g2_FontDrawText(useSmallFont ? smallFont : font, shadow, 0xff18202a, "%s", value);
        g2_FontDrawText(useSmallFont ? smallFont : font, bounds, color, "%s", value);
    };
    if (page == Page::Menu) {
        const char* labels[] = {"START", "SETTING", "EXIT"};
        for (int i = 0; i < 3; ++i) {
            int y = int(top) + i * 64;
            DWORD color = i == selected ? 0xffffd77a : 0xffd5c5a5;
            int x = int(cx) - menuWidths[i] / 2;
            text(x, y, labels[i], color);
            if (i == selected) {
                text(x - 36, y, ">", color);
                text(x + menuWidths[i] + 24, y, "<", color);
            }
        }
    } else if (page == Page::Settings) {
        text(int(cx) - menuWidths[1] / 2, int(top) - 64, "SETTING", 0xffffd77a);
        const std::string volumeLabel = audioAvailable ? "<  " + std::to_string(volumePercent) + "%  >" : "UNAVAILABLE";
        const char* labels[] = {"SOUND", "DISPLAY", "BACK"};
        for (int i = 0; i < 3; ++i) {
            int y = int(top) + i * 64;
            DWORD color = settingSelected == i ? 0xffffd77a : 0xffd5c5a5;
            text(int(cx) - 290, y, labels[i], color);
            if (settingSelected == i) text(int(cx) - 330, y, ">", color);
            if (i == 0) text(int(cx) + 10, y, volumeLabel.c_str(), color);
            if (i == 1) text(int(cx) + 10, y, windowed ? "< WINDOWED >" : "< FULLSCREEN >", color);
        }
    } else {
        text(int(cx) - 47, int(top), "START", 0xffffd77a);
        text(int(cx) - 153, int(top) + 75, "The adventure is under construction.", 0xffedf0f4, true);
        text(int(cx) - 125, int(top) + 145, "ENTER / ESC   Back to menu", 0xffc3cbd3, true);
    }
    return 0;
}
int SceneGameBegin::Destroy() {
    if (mainBgm >= 0) {
        g2_SoundStop(mainBgm);
        g2_SoundRelease(mainBgm);
        mainBgm = -1;
    }
    audio.Destroy();
    if (texture >= 0) { g2_TextureRelease(texture); texture = -1; }
    return 0;
}

