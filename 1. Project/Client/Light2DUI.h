#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{
private:
	Vec4		m_vDiff;		// 난반사 광
	//Vec4		m_vSpec;		// 반사광 계수
	//Vec4		m_vEmb;			// 환경광

	Vec4		m_vWorldPos;	// 광원 월드 위치
	Vec4		m_vWorldDir;	// 광원의 방향
	LIGHT_TYPE	m_iLightType;	// 광원 타입
	float		m_fRadius;		// 광원 반경 (POINT, SPOT)
	float		m_fAngle;		// 광원 각도 (SPOT)

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    Light2DUI();
    ~Light2DUI();
};

