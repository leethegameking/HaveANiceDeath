#pragma once
#include <Engine/CScript.h>
class CMouseTextScript :
    public CScript
{
private:
    bool m_bVisible;

public:
    virtual void begin() override;
    virtual void tick() override;



private:
    CLONE(CMouseTextScript);
public:
    CMouseTextScript();
    ~CMouseTextScript();

};

