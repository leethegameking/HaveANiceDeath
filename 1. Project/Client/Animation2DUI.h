#pragma once
#include "ResUI.h"

class CAnimation2D;

class Animation2DUI :
    public ResUI
{
private:
	wstring m_wstrKey;
	const vector<tAnim2DFrm>* m_vecFrm;

	Vec2 m_vImageScale;
	bool m_bAnimPlay;
	bool m_bFullsizeCheck;

	CAnimation2D* m_prevAnim;

public:
	virtual void init() override;
	virtual void update() override;
	virtual void render_update() override;

public:
	void DrawAnimation();

public:
	Animation2DUI();
	~Animation2DUI();
};


