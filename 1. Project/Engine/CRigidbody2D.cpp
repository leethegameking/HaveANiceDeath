#include "pch.h"
#include "CRigidbody2D.h"

#include "CTransform.h"

#include "CTimeMgr.h"

CRigidbody2D::CRigidbody2D()
	: CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, m_bGround(false)
	, m_bIgnoreGravity(false)
	, m_fMass(1.f)
	, m_fMaxSpeed(100.f)
	, m_fMaxGravitySpeed(100.f)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::finaltick()
{
	// f = ma;
	float fGravityForce = 0.f;
	Vec2 vFinalForce = Vec2::Zero;
	Vec2 fForce = m_vForce;
	Vec2 vSpeed = Vec2(0.f, m_vSpeed.y);

	// gravity
	if (m_bGround || m_bIgnoreGravity)
	{
		m_vSpeed.y = 0.f;
		vSpeed.y = 0.f;
	}
	else
	{
		fGravityForce = m_fMass * m_fGravity;
	}

	// 최종 속도
	vFinalForce += fForce;
	vFinalForce.y += fGravityForce;
	vSpeed += vFinalForce / m_fMass * DT;

	// 보정
	if (fabs(vSpeed.x) > m_fMaxSpeed)
	{
		vSpeed.x = m_fMaxSpeed;
	}

	if (fabs(vSpeed.y) > m_fMaxGravitySpeed)
	{
		vSpeed.y = m_fMaxGravitySpeed;
	}

	// 위치 결정
	Vec3 vPos =Transform()->GetRelativePos();
	vPos.x += vSpeed.x * DT;
	vPos.y += vSpeed.y * DT;
	Transform()->SetRelativePos(vPos);

	// 초기화
	m_vForce = Vec2::Zero;
}

