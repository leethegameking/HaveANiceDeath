#pragma once
#include <Engine/CScript.h>

enum ANIM_CONDITION
{
    GROUND              = 0x00000001,
    KEY_A_OR_D          = 0x00000002,
    ANIM_DIR_CHANGED    = 0x00000004,
    ANIM_FINISHED       = 0x00000008,
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

    tTransitionNode(wstring _Animkey)
        : iTranCond(0)
    {
        pAnimKey = _Animkey;
        pAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(_Animkey);
    }
};

struct tAnimNode
{
    Ptr<CAnimation2D>           pAnim;
    wstring                     pAnimKey;

    UINT                        iCond;

    vector<tTransitionNode*>    vecNextAnim;

    bool                        bNeedDirChange; // Idle, Run 
    bool                        bDirChangeAnim; // IdleUturn , RunUturn
    bool                        bNoMove;        // Uturn

    tAnimNode()
        : bNeedDirChange(false)
        , bDirChangeAnim(false)
        , bNoMove(false)
    {

    }
};



class CAnimController :
    public CScript
{
private:
    int         m_ObjType;

    tAnimNode*  m_pAnimNode;
    tAnimNode*  m_pPrevAnimNode;

    ANIM_DIR    m_eAnimDir;

private: // 외부에서 bit 결정
    bool m_bDirChanged; // CController

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

    bool GetDirChanged() { return m_bDirChanged; }
    void SetDirChanged(bool _b) { m_bDirChanged = true; }

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

