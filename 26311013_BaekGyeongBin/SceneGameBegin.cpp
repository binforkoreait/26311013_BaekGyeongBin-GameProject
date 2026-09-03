#include "SceneGameBegin.h"
#include "glc2d.h"

int SceneGameBegin::Init()
{
	this->m_txMario = g2_TextureLoad("resource/texture/mario.png");
	return 0;
}

int SceneGameBegin::Destroy()
{
	return 0;
}

int SceneGameBegin::Update()
{
	return 0;
}

int SceneGameBegin::Render()
{
	g2_Draw2D(m_txMario, nullptr);
	return 0;
}