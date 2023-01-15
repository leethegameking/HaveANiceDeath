#pragma once
#include "CGameCameraScript.h"
class CUICameraScript :
    public CGameCameraScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};

    CLONE(CUICameraScript);

public:
    CUICameraScript();
    CUICameraScript(const CUICameraScript& _origin);
    ~CUICameraScript();
};

