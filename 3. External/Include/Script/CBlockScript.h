#pragma once
#include <Engine/CScript.h>

class CBlockScript :
    public CScript
{
private:
    bool m_bGroundBlock; // 위에서 밟고 있는 블록인지

private:
    Vec3 m_vBlockScale;
    Vec3 m_vBlockPos;

    Vec2 m_vBlockColScale;
    Vec2 m_vBlockColPos;

    CGameObject* m_pColObj;

    Vec3 m_vObjPos;
    Vec3 m_vObjScale;

    Vec2 m_vObjColPos;
    Vec2 m_vObjColScale;

    short m_sObjDir;

    bool m_bPlatform;

public:
    void tick() override;

private:
    //=======================
    //       Box 기준 
    void LeftCollision();
    void RightCollision();
    void UpCollision(CCollider2D* _other);
    void DownCollision();
    //=======================
    
    void SetMemberData(CCollider2D* _other);

public:
    bool IsPlatform() { return m_bPlatform; }

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

