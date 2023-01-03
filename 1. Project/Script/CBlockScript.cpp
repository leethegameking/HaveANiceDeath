#include "pch.h"
#include "CBlockScript.h"

#include <Engine/CGameObject.h>

CBlockScript::CBlockScript()
	: CScript((int)SCRIPT_TYPE::BLOCKSCRIPT)
	, m_bGroundBlock(false)
{
}

CBlockScript::~CBlockScript()
{
}

void CBlockScript::tick()
{
}



void CBlockScript::SaveToFile(FILE* _pFile)
{
}

void CBlockScript::LoadFromFile(FILE* _pFile)
{
}

void CBlockScript::BeginOverlap(CCollider2D* _other)
{
	SetMemberData(_other);

	if (m_pColObj->Rigidbody2D())
	{
		// 방향 구해줌 ( 나중에 SetMemberData로 넣어줄 예정 )
		m_sObjDir = m_pColObj->Rigidbody2D()->GetDir();

		// Vec3 test=  m_pColObj->Rigidbody2D()->GetPrevPos();
		if (m_sObjDir & RB_DOWN && m_vBlockPos.y + m_vBlockScale.y / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().y - m_vObjScale.y / 2.f)
		{
			UpCollision();
		}
		else if (m_sObjDir & RB_UP && m_vBlockPos.y - m_vBlockScale.y / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().y + m_vObjScale.y / 2.f)
		{
 			DownCollision();
		}

		if (m_sObjDir & RB_LEFT && m_vBlockPos.x + m_vBlockScale.x / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().x - m_vObjScale.x / 2.f)
		{
			RightCollision();
		}
 		else if (m_sObjDir & RB_RIGHT && m_vBlockPos.x - m_vBlockScale.x / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().x + m_vObjScale.x / 2.f)
		{
			LeftCollision();
		}
	}
}

void CBlockScript::Overlap(CCollider2D* _other)
{
	SetMemberData(_other);

	if (m_pColObj->Rigidbody2D())
	{
		if (m_sObjDir & RB_LEFT && m_vBlockPos.x + m_vBlockScale.x / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().x - m_vObjScale.x / 2.f)
		{
			RightCollision();
		}
		else if (m_sObjDir & RB_RIGHT && m_vBlockPos.x - m_vBlockScale.x / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().x + m_vObjScale.x / 2.f)
		{
			LeftCollision();
		}
	}
}

void CBlockScript::EndOverlap(CCollider2D* _other)
{
	if (m_bGroundBlock)
	{
		CGameObject* pColObj = _other->GetOwner();
		pColObj->Rigidbody2D()->SetGround(false);
		m_bGroundBlock = false;
	}
}

void CBlockScript::LeftCollision()
{
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockPos.x - m_vBlockScale.x / 2.f - m_vObjScale.x / 2.f - 0.01f, m_vObjPos.y, 0.f));
}

void CBlockScript::RightCollision()
{
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockPos.x + m_vBlockScale.x / 2.f + m_vObjScale.x / 2.f + 0.01f, m_vObjPos.y, 0.f));
}

void CBlockScript::UpCollision()
{
	m_bGroundBlock = true;
	m_pColObj->Rigidbody2D()->SetGround(true);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjPos.x, m_vObjScale.y / 2.f + m_vBlockPos.y + m_vBlockScale.y / 2.f, 0.f));
}

void CBlockScript::DownCollision()
{
	m_pColObj->Rigidbody2D()->SetForceSpeedY(0.f);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjPos.x, m_vBlockPos.y - m_vObjScale.y / 2.f - m_vBlockScale.y / 2.f - 0.01f, 0.f));
}

void CBlockScript::SetMemberData(CCollider2D* _other)
{
	m_pColObj = _other->GetOwner();

	m_vBlockPos = Transform()->GetRelativePos();
	m_vBlockScale = Transform()->GetRelativeScale();

	m_vObjPos = m_pColObj->Transform()->GetRelativePos();
	m_vObjScale = m_pColObj->Transform()->GetRelativeScale();
}




