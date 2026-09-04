#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "Player.h"

enum class PandaState
{
	Patrol,
	Chase,
	Attack,
	Search
};

class Enemy :
    public GameObject
{
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	void SetPlayer(Player* player)
	{
		player_ = player;
	}

private:
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
	Player* player_ = nullptr;
	PandaState state_ = PandaState::Patrol;

	//タイマー
	float dir_timer_;
	float prog_timer_;
	float searchTimer_;

	//アニメーション
	float animTimer_;
	int frame_;

	// 視野
	float radius_;
	float fov_;

	//ステート
	void Patrol();
	void Chase();
	void Attack();
	void Search();

	
	// 内部処理
	bool CanSeePlayer();
	int DistanceToPlayer();
	bool SearchFinished();
	void UpdatePatrol();
	void UpdateChase();
	void MoveForward();
	void DrawFan(Point pos, float directionRad, float radius, float fov, int div);
	
};



