#pragma once
#include <Engine/CScript.h>

class CUnitScript;

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
    // 해당 비트만 포함된다면
    void AddInclude(UINT _Include, UINT _IncludeZero = 0) { AddBit(iTranCond, _Include); AddBit(iExcludeCond, ~(_Include | _IncludeZero)); }
    void AddTran(UINT _Tran) { AddBit(iTranCond, _Tran); }
    
};

struct tAnimNode
{
    Ptr<CAnimation2D>           pAnim;
    wstring                     pAnimKey;

    UINT                        iCond;
    UINT                        iPreferences;

    vector<tTransitionNode*>    vecNextAnim;

    tAnimNode*                  arrReserveAnim[2];
    UNIT_NAME                   eUnitName;

    UINT                        ePattern;

    tAnimNode()
        : iPreferences(0)
        , iCond(0)
        , ePattern(0)
        , arrReserveAnim{}
    {

    }

    void AddPreferences(UINT _Preferences) { AddBit(iPreferences, _Preferences); }
    void SetPattern(UINT _Pattern) { ePattern = _Pattern; }
    void SetReserve(const wstring& _pAnimPath, bool _bGroundAnim = true);
    void SetCond(UINT _Cond) { iCond = _Cond; }
};



class CAnimController :
    public CScript
{
protected:
    CGameObject*    m_pHitObj;
    CGameObject*    m_pAttObj;
    
    CUnitScript*    m_pUnitScr;

    tAnimNode*      m_pCurAnimNode;
    tAnimNode*      m_pPrevAnimNode;
    tAnimNode*      m_pReserveNode;
    tAnimNode*      m_pTmpSaveNode;
    tAnimNode*      m_pNextNode;
    tAnimNode*      m_pAnyStateNode;

    ANIM_DIR        m_eCurAnimDir;
    UINT            m_iCombo;
    UINT            m_iPrevCombo;
    bool            m_bCombo;
    bool            m_bComboProgress;

    bool            m_bGround;
    bool            m_bPrevGround;
    bool            m_bDirChanging;
    ANIM_DIR        m_eNextDir;

    float           m_fSpeed;
    float           m_fPrevSpeed;

    bool            m_bCanDash;

    vector<tAnimNode*> m_vecAnyStateNode;
private:

public:
    virtual void begin() override;
    virtual void tick();

public:
    static void AnimConInit();

    static void DelAnimConMap();
private:
    static void CreateMapAnimNode();
    static void CreatePlayerAnimCon();
    static void AddPlayerAnyNode();

public:
    static  map<wstring, tAnimNode*> mapAnimNode;

public:
    tAnimNode* GetCurAnimNode() { return m_pCurAnimNode; }
    void SetCurAnimNode(tAnimNode* _pNode) { m_pCurAnimNode = _pNode; }

    virtual ANIM_DIR GetCurAnimDir() { return m_eCurAnimDir; }
    virtual void SetAnimDir(ANIM_DIR _eDir) { m_eCurAnimDir = _eDir; }

    void SetAnyStateNode(tAnimNode* _pNode) { m_pAnyStateNode = _pNode; }

protected:
    // m_pPrevAnimNode = m_pCurAnimNode;
    virtual void PlayNextNode();
    virtual void SetCurDir();
    virtual void SetCondBit();
    virtual void SetGravity();
    virtual void SetAttackCollider();
    virtual void SetInvincible(); // 피격 분기 추가
    virtual void SetComboProgress();
    virtual void PosChangeProgress();
    virtual void NodeProgress();
    virtual void SetDir();

private:
    virtual void Timer();
    void CalDashTime();

public:


public:
    virtual void BeginOverlap(CCollider2D* _other) override;
    virtual void Overlap(CCollider2D* _other) override;
    virtual void EndOverlap(CCollider2D* _other) override;

    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CAnimController)
public:
    CAnimController();
    CAnimController(int _ScriptType);
    ~CAnimController();

    friend class CWorkman;
};