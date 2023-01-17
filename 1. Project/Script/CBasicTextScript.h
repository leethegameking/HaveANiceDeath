#pragma once
#include "CTextScript.h"
class CBasicTextScript :
    public CTextScript
{
private:
    string  m_strText;

    int     m_eFormatType;


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CBasicTextScript);
public:
    CBasicTextScript();
    CBasicTextScript(const CBasicTextScript& _origin);
    ~CBasicTextScript();
};

