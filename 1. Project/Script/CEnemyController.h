#pragma once

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

