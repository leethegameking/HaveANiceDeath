﻿#pragma once
#include "CEnemyScript.h"

enum WORKMAN_PATTERN
{
    WORK_MAN_WAITING,
    WORK_MAN_PATROL,
    WORK_MAN_DELAY,
    WORK_MAN_CHASE,
    WORK_MAN_ATTACK
};

class CWorkman :
    public CEnemyScript
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

private:


public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CWorkman);
public:
    CWorkman();
    CWorkman(const CWorkman& _origin);
    ~CWorkman();
};

