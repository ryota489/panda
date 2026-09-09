#pragma once
#include ".\Library\GameObject.h"
#include "global.h"
#include "Player.h"
#include "EnemyState.h"
#include "PatrolState.h"
#include "ChaseState.h"
#include "AttackState.h"
#include "SearchState.h"

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
	bool CanSeePlayer();
	void ChangeState(PandaState newState);
	void SetRandomDirection();
	Player* GetPlayer() const;
	Point GetPosition() const;
	void SetDirection(DIR dir);
	int DistanceToPlayer();

private:
	int hImage_;//画像ID
	Point pos_;//位置
	DIR dir_;//移動方向
	Player* player_ = nullptr;

	//タイマー
	float dir_timer_;
	float prog_timer_;
	float searchTimer_;
	float move_timer = 0.5f;

	//アニメーション
	float animTimer_;
	int frame_;

	// 視野
	float radius_;
	float fov_;
	
	// 内部処理
	bool SearchFinished();
	void UpdatePatrol();
	void UpdateChase();
	void MoveForward();
	void DrawFan(Point pos, float directionRad, float radius, float fov, int div);
	void DrawVision();

	//ステート
	EnemyState* currentState_ = nullptr;
	PandaState state_ = PandaState::Patrol;
	PatrolState patrolState_;
	ChaseState chaseState_;
	AttackState attackState_;
	SearchState searchState_;
};



