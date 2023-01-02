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
	, m_fMaxGravitySpeed(1000.f)
	, m_fGravity(-1000.f)
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
	Vec2 vForce = m_vForce;
	Vec2 vSpeed = Vec2(0.f, m_vSpeed.y);

	if (m_AddForce.Flag)
	{
		if (m_AddForce.fAccTime <= m_AddForce.fMaxTime) // 초가 0일 때도 한 번은 들어옴.
		{
			vForce = m_AddForce.vForce * DT;
			m_AddForce.fAccTime += DT;
			m_AddForce.fPrevAccTime = m_AddForce.fAccTime;
		}
		else
		{
			vForce += m_AddForce.vForce * (m_AddForce.fMaxTime - m_AddForce.fPrevAccTime);
			m_AddForce.Flag = false;
		}
	}

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
	vFinalForce += vForce;
	vFinalForce.y += fGravityForce;
		// 속도 변화량
	vSpeed += DT * vFinalForce / m_fMass;

	// 강제적인 속도 세팅  /s 
	if (m_ForceX.Flag)
	{
		if (m_ForceX.fAccTime <= m_ForceX.fMaxTime) // 초가 0일 때도 한 번은 들어옴.
		{
			vSpeed.x = m_ForceX.fSpeed;
			m_ForceX.fAccTime += DT;
		}
		else
		{
			m_ForceX.Flag = false;
		}
	}

	if (m_ForceY.Flag)
	{
		if (m_ForceY.fAccTime <= m_ForceY.fMaxTime) // 초가 0일 때도 한 번은 들어옴.
		{
			vSpeed.y = m_ForceY.fSpeed;
			m_ForceY.fAccTime += DT;
		}
		else
		{
			m_ForceY.Flag = false;
		}
	}
	

	// 보정
	if (fabs(vSpeed.x) > m_fMaxSpeed)
	{
		vSpeed.x = m_fMaxSpeed;
	}

	// 중력 속도 보정
	if (vSpeed.y < 0.f)
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

void CRigidbody2D::AddForce(Vec2 _vForce, float _MaxTime)
{
	m_AddForce.Flag = true;
	m_AddForce.fAccTime = 0.f;
	m_AddForce.fMaxTime = _MaxTime;
	m_AddForce.vForce = _vForce;
}

void CRigidbody2D::SetForceSpeedX(float _ForceSpeedX, float _MaxTime)
{
	m_ForceX.Flag = true;

	m_ForceX.fAccTime = 0.f;
	m_ForceX.fMaxTime = _MaxTime;
	m_ForceX.fSpeed = _ForceSpeedX;
}

void CRigidbody2D::SetForceSpeedY(float _ForceSpeedY, float _MaxTime)
{
	m_ForceY.Flag = true;

	m_ForceY.fAccTime = 0.f;
	m_ForceY.fMaxTime = _MaxTime;
	m_ForceY.fSpeed = _ForceSpeedY;
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