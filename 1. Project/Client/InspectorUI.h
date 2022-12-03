#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CRes.h>

class ComponentUI;
class ResUI;

class InspectorUI :
    public UI
{
public:
    CGameObject* m_TargetObj;
    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];

    Ptr<CRes> m_TargetRes;
    ResUI* m_arrResUI[(UINT)RES_TYPE::END];

public:
    void SetTargetObj(CGameObject* _Target);
    void SetTargetRes(Ptr<CRes> _Res);

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

