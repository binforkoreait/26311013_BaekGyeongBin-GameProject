#pragma once
#include "glc2d.h"
#include <string>
class DungeonUI {
public:
    int pixel=-1, font=-1, heading=-1;
    void Init() {
        pixel=g2_TextureLoad("resource/texture/Pixel.png",0);
        font=g2_FontCreate("Segoe UI",18); heading=g2_FontCreate("Segoe UI",44);
    }
    void Destroy() { if(pixel>=0) g2_TextureRelease(pixel); }
    void Box(float x,float y,float w,float h,DWORD color) const {
        VEC2 position(x,y),scale(w,h); g2_Draw2D(pixel,nullptr,&position,&scale,nullptr,0,color);
    }
    void Text(int x,int y,const std::string& text,DWORD color=0xffdbe5ec,bool big=false) const {
        RECT r{x,y,960,y+(big?64:32)}; g2_FontDrawText(big?heading:font,r,color,"%s",text.c_str());
    }
    bool Hover(int x,int y,int w=320,int h=52) const {
        int mx=g2_GetMouseX(),my=g2_GetMouseY(); return mx>=x && mx<x+w && my>=y && my<y+h;
    }
    void Button(int x,int y,const char* label,bool primary=false) const {
        Box(float(x),float(y),320,52,Hover(x,y)?0xff456575:primary?0xff286c67:0xff25323e);
        Box(float(x),float(y),3,52,primary?0xff77e1ba:0xff748795); Text(x+24,y+14,label);
    }
    void Backdrop() const {
        Box(0,0,960,720,0xff101720);
        for(int y=0;y<18;++y) for(int x=0;x<24;++x)
            Box(float(x*40+1),float(y*40+1),38,38,(x+y)%2?0xff141e28:0xff17212b);
        Box(96,90,768,550,0xf51a2530); Box(96,90,768,3,0xffbca16c);
    }
};
