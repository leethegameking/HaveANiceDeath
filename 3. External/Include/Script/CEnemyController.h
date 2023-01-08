﻿#pragma once
#include "CAnimController.h"


class CEnemyController :
    public CAnimController
{

public:
    virtual void begin() override;
    virtual void tick() override;
    
public:
    virtual void SetCondBit() override;
    virtual void NodeProgress();

    CLONE(CEnemyController)
public:
    CEnemyController();
    CEnemyController(const CEnemyController& _origin);
    ~CEnemyController();
};
