#pragma once
#include <Engine/CScript.h>
class CAnimStarter :
    public CScript
{
private:
    wstring m_strAnimKey;
    bool m_bOneAnim;
    bool m_bRepeat;
    bool m_bEndDestory;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CAnimStarter);

public:
    CAnimStarter();
    CAnimStarter(const CAnimStarter& _origin);
    ~CAnimStarter();
};

