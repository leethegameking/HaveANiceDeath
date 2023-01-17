#pragma once
#include <Engine/CScript.h>
class CTextScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CTextScript);
public:
    CTextScript();
    CTextScript(int _iScriptType);
    CTextScript(const CTextScript& _origin);
    ~CTextScript();
};

