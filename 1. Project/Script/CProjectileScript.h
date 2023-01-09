#pragma once
#include <Engine/CScript.h>
class CProjectileScript :
    public CScript
{
private:
    CGameObject* m_pAttObj;

    float   m_fSpeed;
    bool    m_bRot;
    float   m_fMaxLifeTime;

    float   m_fAccLifeTime;
    Vec2    m_vDir;
    bool    m_bFirstTick;

public:
    void SetDir(Vec2 _vDir) { m_vDir = _vDir; }

private:
    void FirstTick();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CProjectileScript);
public:
    CProjectileScript();
    ~CProjectileScript();
};

