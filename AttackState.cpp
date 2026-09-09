#include "AttackState.h"
#include "Enemy.h"

void AttackState::Update(Enemy& enemy)
{
    if (enemy.DistanceToPlayer() > 2)
    {
        enemy.ChangeState(PandaState::Search);
        return;
    }

}
