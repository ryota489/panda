#include "Enemy.h"
#include "time.h"

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	state_ = PandaState::Patrol;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	switch (state_)
	{
	case PandaState::Patrol:
		patrolState_.Update(*this);
		break;

	case PandaState::Chase:
		chaseState_.Update(*this);
		break;

	case PandaState::Attack:
		attackState_.Update(*this);
		break;

	case PandaState::Search:
		searchState_.Update(*this);
		break;
	}

	move_timer -= Time::DeltaTime();

	if (move_timer < 0.0f)
	{
		Point newPos = pos_;

		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		default:
			break;
		}
		//移動先がステージの外に出ないようにする
		if (!(newPos.x < 1 || newPos.x >(STAGE_WIDTH - 2) * ENEMY_DRAW_SIZE
			|| newPos.y < 1 || newPos.y >(STAGE_HEIGHT - 2) * ENEMY_DRAW_SIZE))
		{
			pos_ = newPos;
		}
		move_timer = 0.5f;
	}

}


void Enemy::Draw()
{
	DrawVision();

	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE,2);
	DrawRectExtendGraph(pos_.x, pos_.y,pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		               iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();

	//状態表示
	const char* stateText = "";

	switch (state_)
	{
	case PandaState::Patrol:
			stateText = "PATROL";
			break;

	case PandaState::Chase:
		stateText = "CHASE";
		break;

	case PandaState::Attack:
		stateText = "ATTACK";
		break;

	case PandaState::Search:
		stateText = "SEARCH";
	}
	DrawString(pos_.x, pos_.y - 20, stateText, GetColor(255, 255, 255));
}

//void Enemy::Patrol()
//{
//	//GetRand(数値)
//	//3秒に1回向きをランダムに変える
//	static float dir_timer = 3.0f;
//	static float prog_timer = 0.5f;
//	float dt = Time::DeltaTime();
//
//	if (CanSeePlayer())
//	{
//		ChangeState(PandaState::Chase);
//		return;
//	}
//
//	dir_timer -= dt;
//
//	if (dir_timer < 0.0f)
//	{
//		dir_ = (DIR)GetRand(3);
//		dir_timer = 3.0f;
//	}
//}
//
//void Enemy::Chase()
//{
//	if (player_ == nullptr)
//		return;
//
//	Point playerPos = player_->GetPlayerPos();
//
//	int dx = playerPos.x - pos_.x;
//	int dy = playerPos.y - pos_.y;
//
//	if(abs(dx) > abs(dy))
//	{
//		dir_ = (dx > 0) ? RIGHT : LEFT;
//	}
//	else
//	{
//		dir_ = (dy > 0) ? DOWN : UP;
//	}
//
//	if (DistanceToPlayer() <= 2)
//	{
//		state_ = PandaState::Attack;
//	}
//}
//
//void Enemy::Attack()
//{
//	if (DistanceToPlayer() > 2)
//	{
//		state_ = PandaState::Search;
//	}
//}
//
//void Enemy::Search()
//{
//	static float searchTimer = 0.0f;
//
//	searchTimer += Time::DeltaTime();
//
//	if (CanSeePlayer())
//	{
//		searchTimer = 0.0f;
//		state_ = PandaState::Attack;
//		return;
//	}
//
//	if (searchTimer >= 5.0f)
//	{
//		searchTimer = 0.0f;
//		state_ = PandaState::Patrol;
//	}
//}

bool Enemy::CanSeePlayer()
{
	return DistanceToPlayer() <= 5;
}

void Enemy::ChangeState(PandaState newState)
{
	state_ = newState;
}

void Enemy::SetRandomDirection()
{
	dir_ = (DIR)GetRand(3);
}

Player* Enemy::GetPlayer() const
{
	return player_;
}

Point Enemy::GetPosition() const
{
	return pos_;
}

void Enemy::SetDirection(DIR dir)
{
	dir_ = dir;
}

int Enemy::DistanceToPlayer()
{
	if (player_ == nullptr)
		return 999;

	Point p = player_->GetPlayerPos();

	int dx = abs(pos_.x - p.x) / 32;
	int dy = abs(pos_.y - p.y) / 32;

	return dx + dy;
}

bool Enemy::SearchFinished()
{
	return false;
}

void Enemy::DrawVision()
{
	// 視界の長さ（マス数）
	const float visionRange = 5.0f;

	// 視界の広さ（度）
	const float visionAngle = 45.0f;

	// 敵の中心座標
	float centerX = pos_.x + ENEMY_DRAW_SIZE / 2.0f;
	float centerY = pos_.y + ENEMY_DRAW_SIZE / 2.0f;

	// 向いている方向を角度に変換
	float angle = 0.0f;

	switch (dir_)
	{
	case UP:
		angle = -90.0f;
		break;

	case DOWN:
		angle = 90.0f;
		break;

	case LEFT:
		angle = 180.0f;
		break;

	case RIGHT:
		angle = 0.0f;
		break;
	}

	// 度 → ラジアン
	float rad = angle * 3.14159265f / 180.0f;
	float halfRad = visionAngle * 3.14159265f / 180.0f;

	// 視界の先端
	float range = visionRange * ENEMY_DRAW_SIZE;

	float leftX = centerX + cosf(rad - halfRad) * range;
	float leftY = centerY + sinf(rad - halfRad) * range;

	float rightX = centerX + cosf(rad + halfRad) * range;
	float rightY = centerY + sinf(rad + halfRad) * range;

	// 半透明の赤色
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);

	DrawTriangle(
		(int)centerX, (int)centerY,
		(int)leftX, (int)leftY,
		(int)rightX, (int)rightY,
		GetColor(255, 0, 0),
		TRUE
	);

	// 描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

