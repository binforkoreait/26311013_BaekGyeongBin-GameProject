#include "SceneGamePlay.h"
using namespace dungeon;
void SceneGamePlay::Render(const DungeonUI& ui) const {
    ui.Box(0,0,960,720,0xff101720);
    ui.Text(24,14,"DUNGEON / "+std::to_string(game.stage),0xffbca16c);
    ui.Text(360,14,"SCORE  "+std::to_string(game.score));
    ui.Text(695,14,game.hasKey?"KEY ACQUIRED - FIND EXIT":"FIND THE GOLDEN KEY",game.hasKey?0xff77e1ba:0xffe8c574);
    ui.Box(24,55,240,14,0xff303b44); ui.Box(24,55,2.4f*game.player.hp,14,game.player.hp>30?0xff70d8af:0xffef797b);
    ui.Text(280,49,"HP "+std::to_string(game.player.hp)+" / 100");
    ui.Text(520,49,"ENEMIES  "+std::to_string(game.enemies.size()),0xff93a8b8);
    ui.Text(24,84,"WASD  Move     SPACE  Fire     M  Mute     ESC  Quit",0xff93a8b8);
    for(int y=0;y<Rows;++y) for(int x=0;x<Columns;++x) {
        float px=float(x*Tile),py=float(Top+y*Tile);
        ui.Box(px,py,40,40,0xff101720);
        if(game.map.walls[y][x]) {
            ui.Box(px+1,py+1,38,38,0xff34434f); ui.Box(px+1,py+1,38,4,0xff4c5a65);
            ui.Box(px+3,py+32,34,6,0xff25313c); ui.Box(px+19,py+5,2,25,0xff2a3742);
        } else ui.Box(px+1,py+1,38,38,(x+y)%2?0xff1b2732:0xff1e2b36);
    }
    float ex=game.map.exit.x,ey=game.map.exit.y+Top;
    ui.Box(ex-19,ey-24,38,48,0xff111922); ui.Box(ex-15,ey-20,30,40,game.hasKey?0xff3e9f87:0xff665d48);
    ui.Box(ex-8,ey-14,16,30,game.hasKey?0xff93e4b9:0xff272a29);
    ui.Text(int(ex)-25,int(ey)+26,game.hasKey?"EXIT":"LOCK",0xffe8c574);
    for(const auto& item:game.items) if(!item.taken) {
        float x=item.position.x,y=item.position.y+Top;
        ui.Box(x-12,y+11,25,5,0xff101720);
        if(item.type==ItemType::Key) {
            ui.Box(x-12,y-9,14,14,0xffe8c574); ui.Box(x-8,y-5,6,6,0xff1b2732);
            ui.Box(x,y-3,17,5,0xffe8c574); ui.Box(x+9,y+1,4,6,0xffe8c574);
        } else {
            ui.Box(x-5,y-14,10,5,0xffc5a786); ui.Box(x-9,y-8,18,21,0xffe77b91);
            ui.Box(x-2,y-5,4,15,0xffffe7df); ui.Box(x-6,y,12,4,0xffffe7df);
        }
    }
    for(const auto& e:game.enemies) {
        float x=e.position.x,y=e.position.y+Top;
        ui.Box(x-15,y+13,30,5,0xff101720); ui.Box(x-14,y-10,28,24,0xffa8586a);
        ui.Box(x-11,y-15,22,8,0xffce7586); ui.Box(x-8,y-5,5,5,0xffffd1a5); ui.Box(x+4,y-5,5,5,0xffffd1a5);
        ui.Box(x-14,y-24,28,3,0xff442f3a); ui.Box(x-14,y-24,28.f*e.hp/60,3,0xffed8c9f);
    }
    for(const auto& b:game.bullets) {
        ui.Box(b.position.x-6,b.position.y+Top-6,12,12,0xffb37b48);
        ui.Box(b.position.x-3,b.position.y+Top-3,6,6,0xffffe4a2);
    }
    const auto& p=game.player;
    if(p.immunity<=0 || int(p.immunity*14)%2==0) {
        float x=p.position.x,y=p.position.y+Top;
        ui.Box(x-14,y+13,28,5,0xff101720); ui.Box(x-11,y-6,22,22,0xff468ea0);
        ui.Box(x-11,y-17,22,14,0xffa7d2d6); ui.Box(x-8,y-10,16,5,0xff203540);
        ui.Box(x-10,y+12,7,7,0xff54727d); ui.Box(x+3,y+12,7,7,0xff54727d);
        ui.Box(x+p.facing.x*16-5,y+p.facing.y*16-5,10,10,0xffe8c574);
    }
    ui.Text(24,686,game.hasKey?"The exit is open. Reach the glowing doorway.":"Gold key: upper-right chamber   |   Potions restore 30 HP",0xff93a8b8);
    if(game.banner>0) {
        ui.Box(338,350,284,62,0xee101720);
        ui.Text(374,367,"STAGE "+std::to_string(game.stage)+"  /  FIND THE KEY",0xffe8c574);
    }
}
