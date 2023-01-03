#pragma once
#include <Engine/CScript.h>

class CAnimation2D;

enum PLAYER_ANIM
{
    PlayerIdle              = 0x00000001,
    PlayerIdleToRun         = 0x00000002,
    PlayerRun               = 0x00000004,
    PlayerRunToIdle         = 0x00000008,
    PlayerIdleUturn         = 0x00000010,
    PlayerRunUturn          = 0x00000020,
    PLAYER_ANIM_END,
};

extern wstring PlayerKey[PLAYER_ANIM_END];


class CPlayerScript :
    public CScript
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

private:


public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

