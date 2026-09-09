#include "SearchState.h"
#include "Enemy.h"
#include "time.h"

void SearchState::Update(Enemy& enemy)
{
	static float searchTimer = 0.0f;
    searchTimer += Time::DeltaTime();

    //Œ©‚Â‚¯‚½‚çUŒ‚ó‘Ô
    if (enemy.CanSeePlayer())
    {
        searchTimer = 0.0f;
        enemy.ChangeState(PandaState::Attack);
        return;
    }

    // 5•bŠÔŒ©‚Â‚©‚ç‚È‚¯‚ê‚Î„‰ñ‚Ö–ß‚é
    if (searchTimer >= 5.0f)
    {
        searchTimer = 0.0f;
        enemy.ChangeState(PandaState::Patrol);
    }
}
