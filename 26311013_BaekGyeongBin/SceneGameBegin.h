#pragma once
class SceneGameBegin
{
public:
	int Init();
	int Update();
	int Render();
	int Destroy();

protected:
	// game texture
	int m_txMario = -1;

};
