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
	, m_fGravity(- 10.f)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::finaltick()
{
	static bool bInit = true;
	if (bInit)
	{
		m_vPrevPos = Transform()->GetWorldPos();
		bInit = false;
	}

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
	vSpeed += DT * fGravityForce;

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

	m_vSpeed.y = vSpeed.y;

	// Dir 설정
	CalcDir();

	// 초기화
	m_vForce = Vec2::Zero;
	m_vPrevPos = vPos;
}

void CRigidbody2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();
	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);
}

void CRigidbody2D::LoadFromFile(FILE* _File)
{
}

void CRigidbody2D::CalcDir()
{
	m_sDir = 0;
	Vec3 vCalDir = Transform()->GetWorldPos() - m_vPrevPos;
	if (vCalDir.x < 0)
		m_sDir |= RB_LEFT;
	else if (vCalDir.x > 0)
		m_sDir |= RB_RIGHT;

	if (vCalDir.y < 0)
		m_sDir |= RB_DOWN;
	else if (vCalDir.y > 0)
		m_sDir |= RB_UP;
}