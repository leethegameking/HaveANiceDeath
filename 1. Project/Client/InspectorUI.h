#pragma once
#include "UI.h"

class ComponentUI;

class InspectorUI :
    public UI
{
public:
    CGameObject* m_TargetObj;
    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];

public:
    void SetTarget(CGameObject* _Target);

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

