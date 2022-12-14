#pragma once
#include <Engine/CScript.h>


class CTestScript :
    public CScript
{
public:
    Vec3 vPos;
    Vec2 vDir;
    float fSpeed;

public:
    void tick() override;

    CLONE(CTestScript)
public:
    CTestScript();
    ~CTestScript();
};

