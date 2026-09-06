#include "SceneGameResult.h"
void SceneGameResult::Render(const DungeonUI& ui,const dungeon::Game& game) const {
    bool clear=game.screen==dungeon::Screen::Clear;
    ui.Backdrop(); ui.Text(330,140,clear?"THE DUNGEON IS BEHIND YOU":"YOUR JOURNEY ENDS HERE",0xffbca16c);
    ui.Text(338,200,clear?"GAME CLEAR":"GAME OVER",clear?0xff77e1ba:0xffff9c93,true);
    ui.Text(374,285,"FINAL SCORE   "+std::to_string(game.score));
    ui.Text(398,320,"STAGE   "+std::to_string(game.stage)+" / 3",0xff93a8b8);
    ui.Button(320,390,"PLAY AGAIN     /     ENTER",true);
    ui.Button(320,460,"TITLE SCREEN     /     T");
    ui.Text(375,560,"Every escape starts again.",0xff93a8b8);
}
