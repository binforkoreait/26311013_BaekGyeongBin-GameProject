#pragma once
#include "DungeonGame.h"
#include "DungeonUI.h"
class SceneGamePlay {
public:
    dungeon::Game game;
    void Render(const DungeonUI& ui) const;
};
