#pragma once
#include <Engine/CScript.h>
class CTitleMenuScript :
    public CScript
{
private:
    int m_eType; // TILE_MENU_TYPE

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

    CLONE(CTitleMenuScript);
public:
    CTitleMenuScript();
    CTitleMenuScript(const CTitleMenuScript& _origin);
    ~CTitleMenuScript();
};

