#pragma once
#include "CRes.h"

#include "CTexture.h"
class CAnimator2D;

class CAnimation2D :
    public CRes
{
private:
    vector<tAnim2DFrm>  m_vecFrm;
    int                 m_iCurIdx;

    CAnimator2D*        m_pOwner;
    Ptr<CTexture>       m_AtlasTex;

    float               m_fAccTime;
    bool                m_bFinish;

public:
    Ptr<CTexture> GetAtlas() { return m_AtlasTex; }
    const vector<tAnim2DFrm>* GetFrmVec() { return &m_vecFrm; }

public:
    void finaltick();

public:
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize = Vec2(400.f, 400.f), bool _bVTHZ = HORIZONTAL);
    void Reset()
    {
        m_fAccTime = 0.f;
        m_iCurIdx = 0;
        m_bFinish = false;
    }

    virtual int Load(const wstring& _strFilePath) { return  true; };

    void UpdateData();
    void Clear();

    CLONE(CAnimation2D);
public:
    CAnimation2D();
    ~CAnimation2D();

    friend class CAnimator2D;
};

