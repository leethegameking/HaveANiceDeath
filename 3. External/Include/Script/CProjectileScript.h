#pragma once
#include <Engine/CScript.h>
class CProjectileScript :
    public CScript
{
private:
    wstring m_strAnimKey;
    bool    m_bFirstTick;
    Vec2    m_vDir;
    float   m_fAccLifeTime;
    
    bool    m_bDestroyAnimFinsh;
    bool    m_bRepeat;
    float   m_fSpeed;
    bool    m_bRot;
    float   m_fMaxLifeTime;
    float   m_fAtt;
    bool    m_bAnimHasCol;
    

    

public:
    void SetDir(Vec2 _vDir) { m_vDir = _vDir; }
    void SetAtt(float _fAtt) { m_fAtt = _fAtt; }
    void SetAnimationKey(wstring _key) { m_strAnimKey = _key; }
    void SetRepeat(bool _b) { m_bRepeat = _b; }
    void SetAnimOnce() { m_bRepeat = false; m_bDestroyAnimFinsh = true; }

    float GetAtt() { return m_fAtt; }

private:
    void FirstTick();

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CProjectileScript);
public:
    CProjectileScript();
    CProjectileScript(const CProjectileScript& _origin);
    ~CProjectileScript();
};

