#pragma once
#include <Engine/CScript.h>

enum ANIM_CONDITION
{
    GROUND = 0x00000001,
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

struct tAnimNode
{
    wstring pAnimKey;

    vector<tAnimNode*> vecNextAnim;
    vector<bool>       vecEnsureFinish;
    
    bool IsFinished;
    UINT iCondCmp;
    UINT iCond;

    tAnimNode* tNextAnim;

    tAnimNode()
        : iCond(0)
    {}
};

class CAnimController :
    public CScript
{
private:
    int                             m_ObjType;

    //map<wstring, Ptr<CAnimation2D>> m_mapAnim;
    //vector<wstring>                 m_vecAnimName;
    //vector<Ptr<CAnimation2D>>       m_vecAnim;

    tAnimNode*                       m_pAnimNode;

public:
    static void AnimConInit();

    static void DelAnimConMap();
private:
    static void CreatePlayerAnimCon();
private:
    static  map<wstring, tAnimNode*> mapPlayerNode;
    static  map<wstring, tAnimNode*> mapEnemyNode;

public:
    void GetCondBit();

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
};

