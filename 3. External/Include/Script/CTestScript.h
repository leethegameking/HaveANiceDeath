﻿#pragma once
#include <Engine/CScript.h>

#include <Engine/Ptr.h>
class CPrefab;

class CTestScript :
    public CScript
{
public:
    Vec3 vPos;
    Vec2 vDir;
    float fSpeed;
    Ptr<CPrefab>  testPrefab;

public:
    void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};


    CLONE(CTestScript)
public:
    CTestScript();
    ~CTestScript();
};

