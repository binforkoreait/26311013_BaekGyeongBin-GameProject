#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

namespace dungeon {
constexpr int Columns = 24, Rows = 14, Tile = 40, Top = 112;
struct Vec { float x = 0, y = 0; };
inline float Distance(Vec a, Vec b) { return std::hypot(a.x-b.x, a.y-b.y); }
struct Player { Vec position{60,60}, facing{1,0}; int hp = 100; float immunity = 0, cooldown = 0; };
struct Enemy { Vec position; int hp = 60; float cooldown = 0; };
struct Bullet { Vec position, velocity; float life = 1.4f; };
enum class ItemType { Potion, Key };
struct Item { Vec position; ItemType type; bool taken = false; };
enum class Screen { Title, Playing, GameOver, Clear };
enum class Sound { Attack, Hit, EnemyDeath, Item, Damage, Clear, GameOver };
struct Input { float x = 0, y = 0; bool attack = false; };

class Map {
public:
    bool walls[Rows][Columns]{};
    Vec exit{900,500};
    void Load(int stage) {
        for (int y=0;y<Rows;++y) for(int x=0;x<Columns;++x)
            walls[y][x] = x==0 || y==0 || x==Columns-1 || y==Rows-1;
        // Alternating doorways leave a connected route through each dungeon.
        for(int y=2;y<12;++y) if(y!=4 && y!=5 && y!=10) walls[y][7]=true;
        for(int y=2;y<12;++y) if(y!=3 && y!=8 && y!=9) walls[y][15]=true;
        if(stage>=2) for(int x=9;x<14;++x) if(x!=11) walls[6][x]=true;
        if(stage>=3) for(int x=17;x<22;++x) if(x!=19) walls[7][x]=true;
    }
    bool Blocked(Vec p, float radius) const {
        int left=int(std::floor((p.x-radius)/Tile)), right=int(std::floor((p.x+radius)/Tile));
        int top=int(std::floor((p.y-radius)/Tile)), bottom=int(std::floor((p.y+radius)/Tile));
        for(int y=top;y<=bottom;++y) for(int x=left;x<=right;++x)
            if(x<0 || y<0 || x>=Columns || y>=Rows || walls[y][x]) return true;
        return false;
    }
    void Move(Vec& p, Vec delta, float radius) const {
        Vec next{p.x+delta.x,p.y}; if(!Blocked(next,radius)) p=next;
        next={p.x,p.y+delta.y}; if(!Blocked(next,radius)) p=next;
    }
    bool Visible(Vec a, Vec b) const {
        int steps=int(Distance(a,b)/8)+1;
        for(int i=1;i<=steps;++i) {
            float t=float(i)/steps;
            if(Blocked({a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t},2)) return false;
        }
        return true;
    }
};

class Game {
public:
    Screen screen = Screen::Title;
    Player player;
    Map map;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Item> items;
    std::vector<Sound> sounds;
    int stage=1, score=0;
    bool hasKey=false;
    float banner=0;
    void Start() { player=Player{}; stage=1; score=0; sounds.clear(); screen=Screen::Playing; LoadStage(); }
    void LoadStage() {
        map.Load(stage); player.position={60,60}; player.facing={1,0};
        player.immunity=1; player.cooldown=0; hasKey=false; bullets.clear(); enemies.clear(); items.clear(); banner=2.5f;
        const Vec spawns[]={{220,180},{460,140},{540,420},{740,180},{820,460},{380,460},{700,380},{860,100},{180,420}};
        for(int i=0;i<3+stage*2;++i) enemies.push_back({spawns[i],60,0});
        items.push_back({{860,60},ItemType::Key,false});
        items.push_back({{140,460},ItemType::Potion,false});
        items.push_back({{540,300},ItemType::Potion,false});
    }
    void Update(float dt, Input input) {
        sounds.clear(); if(screen!=Screen::Playing) return;
        dt=(std::min)(dt,0.05f); banner=(std::max)(0.f,banner-dt);
        player.immunity=(std::max)(0.f,player.immunity-dt);
        player.cooldown=(std::max)(0.f,player.cooldown-dt);
        float length=std::hypot(input.x,input.y);
        if(length>0) {
            player.facing={input.x/length,input.y/length};
            map.Move(player.position,{player.facing.x*180*dt,player.facing.y*180*dt},13);
        }
        if(input.attack && player.cooldown<=0) {
            bullets.push_back({player.position,{player.facing.x*440,player.facing.y*440},1.4f});
            player.cooldown=0.23f; sounds.push_back(Sound::Attack);
        }
        for(auto& bullet:bullets) {
            bullet.life-=dt;
            // Small substeps avoid skipping walls or enemies at low frame rates.
            for(int step=0;step<4 && bullet.life>0;++step) {
                bullet.position.x+=bullet.velocity.x*dt/4; bullet.position.y+=bullet.velocity.y*dt/4;
                if(map.Blocked(bullet.position,4)) { bullet.life=0; break; }
                for(auto& enemy:enemies) if(enemy.hp>0 && Distance(enemy.position,bullet.position)<19) {
                    enemy.hp-=30; bullet.life=0;
                    if(enemy.hp<=0) { score+=100; sounds.push_back(Sound::EnemyDeath); }
                    else sounds.push_back(Sound::Hit);
                    break;
                }
            }
        }
        bullets.erase(std::remove_if(bullets.begin(),bullets.end(),[](const Bullet& b){return b.life<=0;}),bullets.end());
        enemies.erase(std::remove_if(enemies.begin(),enemies.end(),[](const Enemy& e){return e.hp<=0;}),enemies.end());
        for(auto& enemy:enemies) {
            enemy.cooldown=(std::max)(0.f,enemy.cooldown-dt);
            float d=Distance(enemy.position,player.position);
            if(d<300 && d>25 && map.Visible(enemy.position,player.position)) {
                float speed=65.f+stage*10.f;
                map.Move(enemy.position,{(player.position.x-enemy.position.x)/d*speed*dt,(player.position.y-enemy.position.y)/d*speed*dt},14);
            }
            if(Distance(enemy.position,player.position)<30 && enemy.cooldown<=0 && player.immunity<=0) {
                player.hp=(std::max)(0,player.hp-15); player.immunity=0.85f; enemy.cooldown=1;
                sounds.push_back(Sound::Damage);
                if(player.hp==0) { screen=Screen::GameOver; sounds.push_back(Sound::GameOver); return; }
            }
        }
        for(auto& item:items) if(!item.taken && Distance(player.position,item.position)<27) {
            if(item.type==ItemType::Key) hasKey=true;
            else { if(player.hp==100) continue; player.hp=(std::min)(100,player.hp+30); }
            item.taken=true; sounds.push_back(Sound::Item);
        }
        if(hasKey && Distance(player.position,map.exit)<27) {
            score+=500; sounds.push_back(Sound::Clear);
            if(stage==3) screen=Screen::Clear;
            else { ++stage; LoadStage(); }
        }
    }
};
}
