#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CRes.h>

class ResUI :
    public UI
{
private:
	Ptr<CRes> m_TargetRes;
	RES_TYPE m_eType;

protected:
	wstring m_wstrKey;

public:
	void SetTarget(Ptr<CRes> _pTarget) { m_TargetRes = _pTarget; }
	Ptr<CRes> GetTarget() { return m_TargetRes; }
	RES_TYPE GetResType() { return m_eType; }

	virtual void init() {};

	virtual void update() override;

	virtual void render_update() override;

public:
	ResUI(const string& _UIName, RES_TYPE _eType);
	~ResUI();
};

