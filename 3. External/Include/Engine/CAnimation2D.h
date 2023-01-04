﻿#pragma once
#include "CRes.h"

#include "CTexture.h"
class CAnimator2D;

class CAnimation2D :
    public CRes
{
private:
    vector<tAnim2DFrm>                  m_vecFrm;
    int                                 m_iCurIdx;

    CAnimator2D*                        m_pOwner;
    Ptr<CTexture>                       m_AtlasTex;

    float                               m_fAccTime;
    bool                                m_bFinish;

    Ptr<CAnimation2D>                   m_pMasterAnim;
    vector<Ptr<CAnimation2D>>           m_vecChildAnim;

    Vec2                                m_vPosChange;

public:
    Ptr<CTexture> GetAtlas() { return m_AtlasTex; }
    vector<tAnim2DFrm>* GetFrmVec() { return &m_vecFrm; }

    const Ptr<CAnimation2D>& GetMasterAnim() { return m_pMasterAnim; }
    Vec2 GetPosChange() { return m_vPosChange; }
    void SetPosChange(Vec2 _PosChange) { m_vPosChange = _PosChange; }


public:
    void finaltick();

public:
    bool IsFinish() { return m_bFinish; }

    // OneFrameAfter Finish
    bool OFAFinish();

public:
    void Reset()
    {
        m_fAccTime = 0.f;
        m_iCurIdx = 0;
        m_bFinish = false;
    }

    // UI용
    void Reallocate();

    void UpdateData();
    void Clear();

private:
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize = Vec2(400.f, 400.f), bool _bVTHZ = HORIZONTAL);
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm);

public:
    virtual int Load(const wstring& _strFilePath) override;
    virtual void Save(const wstring& _strRelativePath) override;

    CLONE(CAnimation2D);
public:
    CAnimation2D();
    CAnimation2D(CAnimation2D& _origin);
    ~CAnimation2D();

    friend class CAnimator2D;
    friend class CResMgr;
};

