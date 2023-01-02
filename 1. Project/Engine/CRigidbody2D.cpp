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
	, m_fGravity(- 20.f)
	, m_fForceSpeedX()
	, m_fMaxForceTime()
	, m_bForceSpeedX(false)
	, m_bForceSpeedY(false)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::tick()
{
	m_vPrevPos = Transform()->GetRelativePos();
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
		// 속도 변화량
	vSpeed += DT * vFinalForce / m_fMass;

	// 강제적인 속도 세팅  /s 
	if (m_bForceSpeedX)
	{
		if (m_fAccForceTime <= m_fMaxForceTime) // 초가 0일 때도 한 번은 들어옴.
		{
			vSpeed.x = m_fForceSpeedX;
			m_fAccForceTime += DT;
		}
		else
		{
			m_bForceSpeedX = false;
		}
	}

	if (m_bForceSpeedY)
	{
		if (m_fAccForceTime <= m_fMaxForceTime) // 초가 0일 때도 한 번은 들어옴.
		{
			vSpeed.y = m_bForceSpeedY;
			m_fAccForceTime += DT;
		}
		else
		{
			m_bForceSpeedX = false;
		}
	}
	

	// 보정
	if (fabs(vSpeed.x) > m_fMaxSpeed)
	{
		vSpeed.x = m_fMaxSpeed;
	}

	// 중력 속도 보정
	if (vSpeed.y < 0)
	{
		if (fabs(vSpeed.y) > fabs(m_fMaxGravitySpeed))
		{
			vSpeed.y = -m_fMaxGravitySpeed;
		}
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
}

void CRigidbody2D::SetForceSpeedX(float _ForceSpeedX, float _MaxTime)
{
	m_bForceSpeedX = true;

	m_fAccForceTime = 0.f;
	m_fMaxForceTime = _MaxTime;
	m_fForceSpeedX = _ForceSpeedX;
}

void CRigidbody2D::SetForceSpeedY(float _ForceSpeedY, float _MaxTime)
{
	m_bForceSpeedY = true;

	m_fAccForceTime = 0.f;
	m_fMaxForceTime = _MaxTime;
	m_fForceSpeedX = _ForceSpeedY;
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
	Vec3 vCalDir = Transform()->GetRelativePos() - m_vPrevPos;
	if (vCalDir.x < 0)
		m_sDir |= RB_LEFT;
	else if (vCalDir.x > 0)
		m_sDir |= RB_RIGHT;

	if (vCalDir.y < 0)
		m_sDir |= RB_DOWN;
	else if (vCalDir.y > 0)
		m_sDir |= RB_UP;
}