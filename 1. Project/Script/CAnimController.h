#pragma once
#include <Engine/CScript.h>

enum ANIM_CONDITION
{
    GROUND              = 0x00000001,
    KEY_A_OR_D          = 0x00000002,
    ANIM_DIR_CHANGED    = 0x00000004,
    ANIM_FINISHED       = 0x00000008,
    KEY_SPACE           = 0x00000010,
    GROUND_TO_AERIAL    = 0x00000020,
    AERIAL_TO_GROUND    = 0x00000040,
    SPEED_Y_POSITIVE    = 0x00000080, 
    SPEED_Y_TURN        = 0x00000100,
    //ANIM_CHANGED = 0x00000001,
    //ANIM_NOT_CHANGED = 0x00000002,
    //ANIM_PLAYING = 0x00000004,
    //ANIM_FINISHED = 0x00000008,
    //GROUND = 0x00000010,
    //AERIAL = 0x00000020,
    //ANIM_REPEAT = 0x00000040,
    //ANIM_NOT_REPEAT = 0x00000080,
    //ANIM_DIR_CHANGED = 0x00000100,
    //ANIM_DIR_NOT_CHANGED = 0x00000200,
};

enum ANIM_PREFERENCES
{
    NEED_DIR_CHANGE     = 0x00000001,
    DIR_CHANGE_ANIM     = 0x00000002,
    NO_MOVE             = 0x00000004,
};

enum class ANIM_DIR
{
    ANIM_LEFT = -1,
    ANIM_RIGHT = 1,
};

enum OBJ_TYPE
{
    OBJ_PLAYER,
    OBJ_ENEMY,
};

struct tTransitionNode
{
    Ptr<CAnimation2D>           pAnim;
    wstring                     pAnimKey;

    UINT                        iTranCond;
    UINT                        iExcludeCond;

    tTransitionNode(wstring _Animkey)
        : iTranCond(0)
        , iExcludeCond(0)
    {
        pAnimKey = _Animkey;
        pAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(_Animkey);
    }

    void AddExclude(UINT _Exclude) { AddBit(iExcludeCond, _Exclude); }
    void AddTran(UINT _Tran) { AddBit(iTranCond, _Tran); }
};

struct tAnimNode
{
    Ptr<CAnimation2D>           pAnim;
    wstring                     pAnimKey;

    UINT                        iCond;

    UINT                        iPreferences;

    vector<tTransitionNode*>    vecNextAnim;


    tAnimNode()
        : iPreferences(0)
    {

    }

    void AddPreferences(UINT _Preferences) { AddBit(iPreferences, _Preferences); }
};



class CAnimController :
    public CScript
{
private:
    int         m_ObjType;

    tAnimNode*  m_pAnimNode;
    tAnimNode*  m_pPrevAnimNode;

    ANIM_DIR    m_eAnimDir;

    bool        m_bGround;
    bool        m_bPrevGround;

    float       m_fSpeed;
    float       m_fPrevSpeed;
private:


public:
    static void AnimConInit();

    static void DelAnimConMap();
private:
    static void CreatePlayerAnimCon();
private:
    static  map<wstring, tAnimNode*> mapAnimNode;

public:
    void GetCondBit();

    tAnimNode* GetCurAnimNode() { return m_pAnimNode; }

    ANIM_DIR GetAnimDir() { return m_eAnimDir; }
    void SetAnimDir(ANIM_DIR _eDir) { m_eAnimDir = _eDir; }

public:
    void begin() override;
    virtual void tick();

public:
    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CAnimController)
public:
    CAnimController();
    ~CAnimController();

private:

};

