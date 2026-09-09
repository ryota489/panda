#include "PatrolState.h"
#include "Enemy.h"
#include "time.h"

void PatrolState::Update(Enemy& enemy)
{
    if (enemy.CanSeePlayer())
    {
        enemy.ChangeState(PandaState::Chase);
        return;
    }

    static float dir_timer = 3.0f;

    dir_timer -= Time::DeltaTime();

    if (dir_timer <= 0.0f)
    {
        enemy.SetRandomDirection();
        dir_timer = 3.0f;
    }
}