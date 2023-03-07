#pragma once
#include <Engine/CScript.h>

class CBasicTextScript;
class CPrefab;

class CCutSceneMgr :
    public CScript
{
private:
    CBasicTextScript* m_scrName;
    CBasicTextScript* m_scrContent;

    bool m_bCreateEndMark;

    bool m_bCutSceneEnd;
    UINT m_iArrIdx;

    Ptr<CPrefab> m_pEndMarkPref;

    bool m_bOnce;

public:
    bool IsCutSceneEnd() { return m_bCutSceneEnd; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CCutSceneMgr);

public:
    CCutSceneMgr();
    CCutSceneMgr(const CCutSceneMgr& _origin);
    ~CCutSceneMgr();
};

