#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CRes.h>

class ScriptUI;
class ComboBox;
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

    ComboBox* m_CompComboBox;
    ComboBox* m_ScriptComboBox;

    vector<ScriptUI*> m_vecScriptUI;

public:
    void SetTargetObj(CGameObject* _Target);
    void SetTargetRes(Ptr<CRes> _Res);

private:
    virtual void update() override;
    virtual void render_update() override;
    virtual void last_render() override;

public:
    void AddComponent(DWORD_PTR _idx);
    void AddScript(DWORD_PTR _idx);

public:
    InspectorUI();
    ~InspectorUI();
};

