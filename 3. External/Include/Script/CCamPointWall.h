#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CCamPointWall :
    public CScript
{
private:
    Ptr<CPrefab>    m_pColPref;

    CGameObject*    m_pWallRight;
    CGameObject*    m_pWallLeft;

    bool            m_bCreatWall;
    bool            m_bDeleteWall;

public:
    void CreateWall() { m_bCreatWall = true; }
    void DeleteWall() { m_bDeleteWall = true; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CCamPointWall);

public:
    CCamPointWall();
    CCamPointWall(const CCamPointWall& _origin);
    ~CCamPointWall();
};

