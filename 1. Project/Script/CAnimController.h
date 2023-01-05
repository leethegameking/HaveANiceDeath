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
    COMBO_PROGRESS      = 0x00000200,
    MOUSE_LEFT          = 0x00000400,
};

enum PLAYER_COMBO
{
    COMBO_NONE,
    COMBO1,
    COMBO2, // FightToIdle
    COMBO3, // FightToIdle
    COMBO4,
};

enum ANIM_PREFERENCES
{
    NEED_DIR_CHANGE     = 0x00000001,
    DIR_CHANGE_ANIM     = 0x00000002,
    NO_MOVE             = 0x00000004,
    COMBO_ANIM          = 0x00000008,
    HAS_RESERVE         = 0x00000010,
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

    tAnimNode*                  pReserveAnim;

    tAnimNode()
        : iPreferences(0)
        , iCond(0)
    {

    }

    void AddPreferences(UINT _Preferences) { AddBit(iPreferences, _Preferences); }
    void SetReserve(const wstring& _pAnimPath);
};



class CAnimController :
    public CScript
{
private:
    int             m_ObjType;

    tAnimNode*      m_pAnimNode;
    tAnimNode*      m_pPrevAnimNode;
    tAnimNode*      m_pReserveNode;
    tAnimNode*      m_pTmpSaveNode;

    ANIM_DIR        m_eAnimDir;
    UINT            m_iCombo;
    UINT            m_iPrevCombo;
    bool            m_bCombo;
    bool            m_bBitCombo;
    bool            m_bComboProgress;

    bool            m_bGround;
    bool            m_bPrevGround;

    float           m_fSpeed;
    float           m_fPrevSpeed;
private:


public:
    static void AnimConInit();

    static void DelAnimConMap();
private:
    static void CreatePlayerAnimCon();
public:
    static  map<wstring, tAnimNode*> mapAnimNode;

public:
    void SetCondBit();
    void SetComboProgress();
    void SetReserveNode();

    tAnimNode* GetCurAnimNode() { return m_pAnimNode; }

    ANIM_DIR GetAnimDir() { return m_eAnimDir; }
    void SetAnimDir(ANIM_DIR _eDir) { m_eAnimDir = _eDir; }

private:
    void ComboDelay(); // 콤보 모션 지연
    void NodeProgress();
    void SetDir();

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

