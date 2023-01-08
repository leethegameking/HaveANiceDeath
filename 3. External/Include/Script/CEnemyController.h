#pragma once

enum E_ANIM_CONDITION
{
    E_WAITING,
    E_APPEAR,
    E_IDLE,
    E_UTURN,
    E_DETECT,
    E_RUN,
    E_ATTACK,
};

#include "CAnimController.h"
class CEnemyController :
    public CAnimController
{

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    CEnemyController();
    ~CEnemyController();
};

