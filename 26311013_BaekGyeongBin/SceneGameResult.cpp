#include "SceneGameResult.h"
void SceneGameResult::Render(const DungeonUI &ui, const dungeon::Game &game) const
{
    bool clear = game.screen == dungeon::Screen::Clear;
    const char *heading = "모험이 여기서 끝났습니다";
    const char *result = "게임 오버";
    DWORD resultColor = 0xffff9c93;
    if (clear)
    {
        heading = "던전을 탈출했습니다";
        result = "게임 클리어";
        resultColor = 0xff77e1ba;
    }
    ui.Backdrop();
    ui.Text(330, 140, heading, 0xffbca16c);
    ui.Text(338, 200, result, resultColor, true);
    ui.Text(398, 285, "스테이지   " + std::to_string(game.stage) + " / 3", 0xff93a8b8);
    ui.Button(320, 390, "다시 시작     /     Enter", true);
    ui.Button(320, 460, "타이틀 화면     /     T");
    ui.Text(375, 560, "다음 모험을 시작하세요.", 0xff93a8b8);
}
