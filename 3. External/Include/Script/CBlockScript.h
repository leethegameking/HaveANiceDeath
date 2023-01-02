#pragma once
#include <Engine/CScript.h>

class CBlockScript :
    public CScript
{
private:
    Vec3 m_vBlockScale;
    Vec3 m_vBlockPos;

    CGameObject* m_pColObj;

    Vec3 m_vObjPos;
    Vec3 m_vObjScale;

    short m_sObjDir;

public:
    void tick() override;

private:
    //=======================
    //       Box 기준 
    void LeftCollision();
    void RightCollision();
    void UpCollision();
    void DownCollision();
    //=======================
    
    void SetMemberData(CCollider2D* _other);

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    CLONE(CBlockScript)
public:
    CBlockScript();
    ~CBlockScript();
};

