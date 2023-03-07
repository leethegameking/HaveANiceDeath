#pragma once
#include "CBasicTextScript.h"

enum class DAMAGE_TEXT_STATE
{
    UP_1,
    DELAY,
    UP_2,
    END,
};

class CDamageText :
    public CBasicTextScript
{
private:
    int     m_eState;

    float   m_fSpeedY;

    float   m_fUpTime_1;
    float   m_fDelayTime;
    float   m_fUpTime_2;

    float   m_fUpAccTime_1;
    float   m_fDelayAccTime;
    float   m_fUpAccTime_2;

    float   m_fAlpha;

private:
    void StateUp_1();
    void StateDelay();
    void StateUp_2();
    void StateEnd();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CDamageText);
public:
    CDamageText();
    CDamageText(const CDamageText& _origin);
    ~CDamageText();
};

