#pragma once
#include "SceneGameBegin.h"
class CApplication {
public:
    int Init();
    int Update();
    int Render();
    int Destroy();
private:
    SceneGameBegin begin;
};
