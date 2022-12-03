#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;

    bool                        m_bRepeat;

public:    
    virtual void finaltick() override;

public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);
    void AddAnimation(wstring _key);

    void Play(const wstring& _strKey, bool _bRepeat = true);

    void UpdateData();
    void Clear();

    CAnimation2D* GetCurAnim(){return m_pCurAnim;}
    void SetCurAnim(CAnimation2D* _curAnim) { m_pCurAnim = _curAnim; }
    map<wstring, CAnimation2D*>& GetAnimMap() { return m_mapAnim; }

    bool IsRepeat() { return m_bRepeat; }

public:
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    ~CAnimator2D();
};

