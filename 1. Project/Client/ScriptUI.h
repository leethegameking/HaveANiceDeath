#pragma once
#include "ComponentUI.h"

class CScript;

class ScriptUI :
    public ComponentUI
{
public:
    static int ID;

private:
    CScript* m_pTargetScript;
    int m_ID;


public:
    void SetTargetScript(CScript* _pScript)
    {
        m_pTargetScript = _pScript;
    }

    CScript* GetTargetScript()
    {
        return m_pTargetScript;
    }
    
public:
    virtual void render_update() override;

public:
    ScriptUI();
    ~ScriptUI();
};

