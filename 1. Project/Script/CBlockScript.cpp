#include "pch.h"
#include "CBlockScript.h"

#include "CScriptMgr.h"
#include "CUnitScript.h"

#include <Engine/CGameObject.h>

CBlockScript::CBlockScript()
	: CScript((int)SCRIPT_TYPE::BLOCKSCRIPT)
	, m_bGroundBlock(false)
	, m_bPlatform(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "IsPlatform", (void*)&m_bPlatform);
}

CBlockScript::~CBlockScript()
{
}

void CBlockScript::tick()
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
		if (m_sObjDir & RB_DOWN && m_vBlockColPos.y + m_vBlockColScale.y / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().y - m_vObjColScale.y / 2.f)
		{
			UpCollision(_other);
		}
		else if (m_sObjDir & RB_UP && m_vBlockColPos.y - m_vBlockColScale.y / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().y + m_vObjColScale.y / 2.f)
		{
 			DownCollision();
		}

		if (m_sObjDir & RB_LEFT && m_vBlockColPos.x + m_vBlockColScale.x / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().x - m_vObjColScale.x / 2.f)
		{
			RightCollision();
		}
 		else if (m_sObjDir & RB_RIGHT && m_vBlockColPos.x - m_vBlockColScale.x / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().x + m_vObjColScale.x / 2.f)
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
		if (m_sObjDir & RB_LEFT && m_vBlockColPos.x + m_vBlockColScale.x / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().x - m_vObjColScale.x / 2.f)
		{
			RightCollision();
		}
		else if (m_sObjDir & RB_RIGHT && m_vBlockColPos.x - m_vBlockColScale.x / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().x + m_vObjColScale.x / 2.f)
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
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockColPos.x - m_vBlockColScale.x / 2.f - m_vObjColScale.x / 2.f - 0.01f, m_vObjColPos.y, 0.f));
}

void CBlockScript::RightCollision()
{
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockColPos.x + m_vBlockColScale.x / 2.f + m_vObjColScale.x / 2.f + 0.01f, m_vObjColPos.y, 0.f));
}

void CBlockScript::UpCollision(CCollider2D* _other)
{
	m_bGroundBlock = true;
	m_pColObj->Rigidbody2D()->SetGround(true);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjColPos.x, m_vObjColScale.y / 2.f + m_vBlockColPos.y + m_vBlockColScale.y / 2.f, 0.f));

	CUnitScript* pUnitScript = _other->GetOwner()->GetScript<CUnitScript>();
	if (m_bPlatform)
	{
		pUnitScript->AddStateBits(UNIT_GROUND_PLATFORM);
	}
	else
	{
		pUnitScript->AddStateBits(UNIT_GROUND);
	}
}

void CBlockScript::DownCollision()
{
	m_pColObj->Rigidbody2D()->SetForceSpeedY(0.f);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjColPos.x, m_vBlockColPos.y - m_vObjColScale.y / 2.f - m_vBlockColScale.y / 2.f - 0.01f, 0.f));
}

void CBlockScript::SetMemberData(CCollider2D* _other)
{
	m_pColObj = _other->GetOwner();

	m_vBlockPos = Transform()->GetRelativePos();
	m_vBlockScale = Transform()->GetRelativeScale();

	m_vBlockColPos = Collider2D()->GetFinalPos();
	m_vBlockColScale = Collider2D()->GetFinalScale();
	
	m_vObjPos = m_pColObj->Transform()->GetRelativePos();
	m_vObjScale = m_pColObj->Transform()->GetRelativeScale();

	m_vObjColPos = m_pColObj->Collider2D()->GetFinalPos();
	m_vObjColScale = m_pColObj->Collider2D()->GetFinalScale();;
}


void CBlockScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_bPlatform, sizeof(bool), 1, _pFile);
}

void CBlockScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_bPlatform, sizeof(bool), 1, _pFile);
}

