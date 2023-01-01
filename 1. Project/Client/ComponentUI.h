﻿#pragma once
#include "UI.h"

class CGameObject;
class ComboBox;


class ComponentUI :
    public UI
{
private:
	CGameObject*	m_TargetObj;
	COMPONENT_TYPE	m_eType;

public:
	void SetTarget(CGameObject* _pTarget) { m_TargetObj = _pTarget; }
	CGameObject* GetTarget() { return m_TargetObj; }
	COMPONENT_TYPE GetComponentType() { return m_eType; }

	virtual void render_update() override;

protected:
	void DeleteCompButton();

public:
	ComponentUI(const string& _UIName, COMPONENT_TYPE _eType);
	~ComponentUI();

};

