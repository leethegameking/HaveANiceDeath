#pragma once
#include <Engine/CScript.h>

enum class SCALER_OPT
{
    START_END,
    START_MIDDLE_END,
};

class CEffectScaler :
    public CScript
{
    Vec3 m_vScaleStart;
    Vec3 m_vScaleMiddle;
    Vec3 m_vScaleEnd;

    float m_fMiddleTime;
    float m_fMiddleDelayTime;
    float m_fDelayAccTIme;
    float m_fMaxTime;
    float m_fAccTime;

    bool m_bDelayFlag;
    bool m_bDelay;

    int m_iOption;

private:
    void Start_End();
    void Start_Middle_End();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CEffectScaler);
public:
    CEffectScaler();
    CEffectScaler(const CEffectScaler& _origin);
    ~CEffectScaler();
};



