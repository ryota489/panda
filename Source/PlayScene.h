#pragma once
#include "../Library/SceneBase.h"

class Player;
class Enemy;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

private:
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
};
