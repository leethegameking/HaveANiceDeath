#pragma once
#include <Engine/CScript.h>



class CGameCameraScript :
    public CScript
{
protected:
    CGameObject* m_pPlayer;
    float m_fCamSpeed;
    CAMERA_TYPE m_eType;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};

    CLONE(CGameCameraScript);

public:
    CAMERA_TYPE GetType() { return m_eType; }

public:
    CGameCameraScript();
    CGameCameraScript(int _iScriptType);
    CGameCameraScript(const CGameCameraScript& _origin);
    ~CGameCameraScript();
};

