#include "ChaseState.h"
#include "Enemy.h"

void ChaseState::Update(Enemy& enemy)
{
    if (enemy.GetPlayer() == nullptr)
    {
        return;
    }

    Point playerPos = enemy.GetPlayer()->GetPlayerPos();
    Point enemyPos = enemy.GetPosition();

    int dx = playerPos.x - enemyPos.x;
    int dy = playerPos.y - enemyPos.y;

    // プレイヤーの方向を向く
    if (abs(dx) > abs(dy))
    {
        enemy.SetDirection((dx > 0) ? RIGHT : LEFT);
    }
    else
    {
        enemy.SetDirection((dy > 0) ? DOWN : UP);
    }

    // 2マス以内なら攻撃状態へ
    if (enemy.DistanceToPlayer() <= 2)
    {
        enemy.ChangeState(PandaState::Attack);
    }
}