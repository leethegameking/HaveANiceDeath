#pragma once
#include <Engine/CScript.h>


class TestScript :
    public CScript
{
public:
    Vec3 vPos;
    Vec2 vDir;
    float fSpeed;

public:
    void tick() override;

    CLONE(TestScript)
public:
    TestScript();
    ~TestScript();
};

