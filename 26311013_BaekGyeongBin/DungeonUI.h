#pragma once
#include "glc2d.h"
#include "KoreanText.h"
#include <string>
class DungeonUI
{
  public:
    int pixel = -1;
    int font = -1;
    int heading = -1;
    void Init()
    {
        pixel = g2_TextureLoad("resource/texture/Pixel.png", 0);
        font = g2_FontCreate("Noto Serif KR", 18);
        heading = g2_FontCreate("Noto Serif KR", 44);
    }
    void Destroy()
    {
        if (pixel >= 0)
        {
            g2_TextureRelease(pixel);
            pixel = -1;
        }
    }
    void Box(float x, float y, float w, float h, DWORD color) const
    {
        VEC2 position(x, y);
        VEC2 scale(w, h);
        g2_Draw2D(pixel, nullptr, &position, &scale, nullptr, 0, color);
    }
    void Text(int x, int y, const std::string &text, DWORD color = 0xffdbe5ec,
              bool big = false) const
    {
        int textHeight = 32;
        int textFont = font;
        if (big)
        {
            textHeight = 64;
            textFont = heading;
        }
        RECT r{x, y, 960, y + textHeight};
        std::string localText = ToLocalText(text);
        g2_FontDrawText(textFont, r, color, "%s", localText.c_str());
    }
    bool Hover(int x, int y, int w = 320, int h = 52) const
    {
        int mx = g2_GetMouseX();
        int my = g2_GetMouseY();
        return mx >= x && mx < x + w && my >= y && my < y + h;
    }
    void Button(int x, int y, const char *label, bool primary = false) const
    {
        DWORD backgroundColor = 0xff25323e;
        DWORD accentColor = 0xff748795;
        if (primary)
        {
            backgroundColor = 0xff286c67;
            accentColor = 0xff77e1ba;
        }
        if (Hover(x, y))
        {
            backgroundColor = 0xff456575;
        }
        Box(float(x), float(y), 320, 52, backgroundColor);
        Box(float(x), float(y), 3, 52, accentColor);
        Text(x + 24, y + 14, label);
    }
    void Backdrop() const
    {
        Box(0, 0, 960, 720, 0xff101720);
        for (int y = 0; y < 18; ++y)
        {
            for (int x = 0; x < 24; ++x)
            {
                DWORD tileColor = 0xff17212b;
                if ((x + y) % 2 != 0)
                {
                    tileColor = 0xff141e28;
                }
                Box(float(x * 40 + 1), float(y * 40 + 1), 38, 38, tileColor);
            }
        }
        Box(96, 90, 768, 550, 0xf51a2530);
        Box(96, 90, 768, 3, 0xffbca16c);
    }
};
