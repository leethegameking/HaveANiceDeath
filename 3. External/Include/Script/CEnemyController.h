#pragma once
#include "CAnimController.h"



class CEnemyController :
    public CAnimController
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    
public:
    virtual void SetPattern();
    // 각 Obj script에서 다음 비트 넣어줌.
    virtual void SetCondBit() override;
    virtual void NodeProgress();

public:

    CLONE(CEnemyController)
public:
    CEnemyController();
    CEnemyController(const CEnemyController& _origin);
    ~CEnemyController();
};

