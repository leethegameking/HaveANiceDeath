#pragma once
#include <Engine/CScript.h>
class CGameCameraScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;
    float m_fCamSpeed;

public:
    void tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    virtual void BeginOverlap(CCollider2D* _other) override {};
    virtual void Overlap(CCollider2D* _other) override {};
    virtual void EndOverlap(CCollider2D* _other) override {};

    CLONE(CGameCameraScript);

public:
    CGameCameraScript();
    CGameCameraScript(const CGameCameraScript& _origin);
    ~CGameCameraScript();
};

