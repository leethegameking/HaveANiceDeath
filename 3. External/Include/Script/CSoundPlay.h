#pragma once
#include <Engine/CScript.h>


class CSoundPlay :
    public CScript
{
private:
    string m_strSoundPath;
    float m_fVolume;
    int m_iRepeat;

    bool m_bBGM;

    bool m_bEndDestroy;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CSoundPlay);
public:
    CSoundPlay();
    CSoundPlay(const CSoundPlay& _origin);
    ~CSoundPlay();
};

