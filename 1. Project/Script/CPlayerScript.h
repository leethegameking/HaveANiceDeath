#pragma once
#include <Engine/CScript.h>

class CAnimation2D;

enum class ANIM_DIR
{
    LEFT = -1,
    RIGHT = 1,
};

enum ANIM_CONDITION
{
	ANIM_CHANGED            = 0x00000001,
	ANIM_NOT_CHANGED        = 0x00000002,
	ANIM_PLAYING            = 0x00000004,
	ANIM_FINISHED           = 0x00000008,
	GROUND                  = 0x00000010,
	AERIAL                  = 0x00000020,
	ANIM_REPEAT             = 0x00000040,
    ANIM_NOT_REPEAT         = 0x00000080,
    ANIM_DIR_CHANGED        = 0x00000100,
    ANIM_DIR_NOT_CHANGED    = 0x00000200,
};

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

