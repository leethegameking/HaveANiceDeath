#pragma once
#include "CBasicTextScript.h"
class COrderTextScript :
    public CBasicTextScript
{
private:
    string      m_strTmp;
    float       m_fNextCharTime;
    float       m_fAccTime;
    string      m_strOutText;
    size_t      m_iLength;

    bool        m_bTextOutEnd;


public:
    bool IsTextOutEnd() { return m_bTextOutEnd; }
    void SetTexOutEnd(bool _b) 
    { 
        m_bTextOutEnd = _b;
        begin();
    }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(COrderTextScript);

public:
    COrderTextScript();
    COrderTextScript(const COrderTextScript& _origin);
    ~COrderTextScript();
};

