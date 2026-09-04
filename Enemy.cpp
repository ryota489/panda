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
		Patrol();
		break;

	case PandaState::Chase:
		Chase();
		break;

	case PandaState::Attack:
		Attack();
		break;

	case PandaState::Search:
		Search();
		break;
	}

	//GetRand(数値)
	//3秒に1回向きをランダムに変える
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	
	if (state_ == PandaState::Patrol)
	{
		dir_timer -= dt;

		if (dir_timer < 0.0f)
		{
			dir_ = (DIR)GetRand(3);
			dir_timer = 3.0f;
		}
	}
	move_timer -= dt;

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

void Enemy::Patrol()
{
	if (CanSeePlayer())
	{
		state_ = PandaState::Chase;
	}
}

void Enemy::Chase()
{
	if (player_ == nullptr)
		return;

	Point playerPos = player_->GetPlayerPos();

	int dx = playerPos.x - pos_.x;
	int dy = playerPos.y - pos_.y;

	if(abs(dx) > abs(dy))
	{
		dir_ = (dx > 0) ? RIGHT : LEFT;
	}
	else
	{
		dir_ = (dy > 0) ? DOWN : UP;
	}

	if (DistanceToPlayer() <= 2)
	{
		state_ = PandaState::Attack;
	}
}

void Enemy::Attack()
{
	if (DistanceToPlayer() > 2)
	{
		state_ = PandaState::Search;
	}
}

void Enemy::Search()
{
	static float searchTimer = 0.0f;

	searchTimer += Time::DeltaTime();

	if (CanSeePlayer())
	{
		searchTimer = 0.0f;
		state_ = PandaState::Attack;
		return;
	}

	if (searchTimer >= 5.0f)
	{
		searchTimer = 0.0f;
		state_ = PandaState::Patrol;
	}
}

bool Enemy::CanSeePlayer()
{
	return DistanceToPlayer() <= 5;
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

