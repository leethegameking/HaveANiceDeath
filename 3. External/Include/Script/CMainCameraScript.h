#pragma once
#include "CGameCameraScript.h"
class CMainCameraScript :
    public CGameCameraScript
{
private:
    int m_eMainCamState;

    CGameObject* m_pCamPoint;

    tCameraEvent m_tCurEvent;
    tCameraEvent m_tNextEvent;

    float       m_fRange;
    float       m_fRotRange;
    float       m_fRotBack;

    float       m_fRotMaxTime;
    float       m_fRotAccTime;

public:
    void SetEvent(const tCameraEvent& _evn);
    void EventProgress();

public:
    int GetMainCamState() { return m_eMainCamState; }
    void SetMainCamState(MAIN_CAM_STATE _eType) { m_eMainCamState = (int)_eType; }

private:
    void StateGeneral();
    void StateBossBrad();

private:
    void CameraMove(Vec3 _vStart, Vec3 _vTaret, Vec3 _vOffset, float _fMoveDistRatio = 0.1f, float _fPerSecond = 40.f);

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};

    CLONE(CMainCameraScript);

public:
    CMainCameraScript();
    CMainCameraScript(const CMainCameraScript& _origin);
    ~CMainCameraScript();
};

