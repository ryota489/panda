#pragma once
#include "Library\GameObject.h"

class Enemy;

class EnemyState :
    public GameObject
{
public:
    virtual ~EnemyState() = default;
    virtual void Update(Enemy& enemy) = 0;
};

