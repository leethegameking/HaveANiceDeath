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

	// ���� �ӵ�
	vFinalForce += fForce;
	vFinalForce.y += fGravityForce;
		// �ӵ� ��ȭ��
	vSpeed += DT * vFinalForce / m_fMass;

	// �������� �ӵ� ����  /s 
	if (m_bForceSpeedX)
	{
		if (m_fAccForceTime <= m_fMaxForceTime) // �ʰ� 0�� ���� �� ���� ����.
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
		if (m_fAccForceTime <= m_fMaxForceTime) // �ʰ� 0�� ���� �� ���� ����.
		{
			vSpeed.y = m_bForceSpeedY;
			m_fAccForceTime += DT;
		}
		else
		{
			m_bForceSpeedX = false;
		}
	}
	

	// ����
	if (fabs(vSpeed.x) > m_fMaxSpeed)
	{
		vSpeed.x = m_fMaxSpeed;
	}

	// �߷� �ӵ� ����
	if (vSpeed.y < 0)
	{
		if (fabs(vSpeed.y) > fabs(m_fMaxGravitySpeed))
		{
			vSpeed.y = -m_fMaxGravitySpeed;
		}
	}

	// ��ġ ����
	Vec3 vPos =Transform()->GetRelativePos();
	vPos.x += vSpeed.x * DT;
	vPos.y += vSpeed.y * DT;
	Transform()->SetRelativePos(vPos);

	m_vSpeed.y = vSpeed.y;

	// Dir ����
	CalcDir();

	// �ʱ�ȭ
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