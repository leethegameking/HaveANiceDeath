#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, Ptr<CAnimation2D>> m_mapAnim;
    Ptr<CAnimation2D>               m_pCurAnim;

    bool                        m_bRepeat;

public:    
    virtual void finaltick() override;

public:
    // void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    Ptr<CAnimation2D> FindAnimation(const wstring& _strKey);
    void AddAnimation(wstring _key);

    void Play(const wstring& _strKey, bool _bRepeat = true);

    void UpdateData();
    void UpdateData_CS();
    void Clear();

    Ptr<CAnimation2D> GetCurAnim(){return m_pCurAnim.Get(); }
    void SetCurAnim(Ptr<CAnimation2D> _curAnim) { m_pCurAnim = _curAnim; }
    map<wstring, Ptr<CAnimation2D>>& GetAnimMap() { return m_mapAnim; }

    bool IsRepeat() { return m_bRepeat; }

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

