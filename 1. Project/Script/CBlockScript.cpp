#include "pch.h"
#include "CBlockScript.h"

#include "CScriptMgr.h"
#include "CPlayerScript.h"

#include <Engine/CGameObject.h>

CBlockScript::CBlockScript()
	: CScript((int)SCRIPT_TYPE::BLOCKSCRIPT)
	, m_bPlatform(false)
	, m_bGround(false)
	, m_fAcceptHeight(10.f)
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
	if (!IsUnit(_other))
		return;

	SetMemberData(_other);

	if (m_pColObj->Rigidbody2D())
	{
		// 방향 구해줌 ( 나중에 SetMemberData로 넣어줄 예정 )
		m_pColObj->Rigidbody2D()->CalcDir();
		m_sObjDir = m_pColObj->Rigidbody2D()->GetDir();

		// Vec3 test=  m_pColObj->Rigidbody2D()->GetPrevPos();
		if (m_sObjDir & RB_DOWN && m_vBlockColPos.y + m_vBlockColScale.y / 2.f < m_pColObj->Rigidbody2D()->GetPrevPos().y - m_vObjColScale.y / 2.f)
		{
 			UpCollision(_other);
		}
		else if (m_sObjDir & RB_UP && m_vBlockColPos.y - m_vBlockColScale.y / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().y + m_vObjColScale.y / 2.f
			&& !m_bPlatform)
		{
 			DownCollision();
		}

		bool bGround = m_pColObj->Rigidbody2D()->IsGround();

		// 차이가 미묘한 블록 사이를 움직일 때. 
		if ( bGround && !m_bGround &&  
			(m_vObjPos.y - m_vObjColScale.y / 2.f + m_fAcceptHeight > m_vBlockColPos.y + m_vBlockColScale.y / 2.f
			&& m_vObjPos.y - m_vObjColScale.y / 2.f - m_fAcceptHeight < m_vBlockColPos.y + m_vBlockColScale.y / 2.f))
		{
			UpCollision(_other);
		}
		else
		{
			if (!m_bPlatform)
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
	}
}

void CBlockScript::Overlap(CCollider2D* _other)
{
	if (!IsUnit(_other))
		return;

	SetMemberData(_other);

	if (m_pColObj->Rigidbody2D())
	{
		m_pColObj->Rigidbody2D()->CalcDir();
		m_sObjDir = m_pColObj->Rigidbody2D()->GetDir();

		if (m_sObjDir & RB_UP && m_vBlockColPos.y - m_vBlockColScale.y / 2.f > m_pColObj->Rigidbody2D()->GetPrevPos().y + m_vObjColScale.y / 2.f
			&& !m_bPlatform)
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

void CBlockScript::EndOverlap(CCollider2D* _other)
{
	if (!IsUnit(_other))
		return;

	SetMemberData(_other);
	m_pColObj->Rigidbody2D()->CalcDir();
	m_sObjDir = m_pColObj->Rigidbody2D()->GetDir();
	CUnitScript* pPlayerScript = _other->GetOwner()->GetScript<CUnitScript>();

	// UnitState allocate
	CUnitScript* pUnitScr = m_pColObj->GetScript<CUnitScript>();
	vector<CGameObject*>& vecGroundObj = pUnitScr->GetGroundObjVec();
	vector<CGameObject*>::iterator iter = vecGroundObj.begin();
	for (; iter != vecGroundObj.end();)
	{
		if (*iter == this->GetOwner())
		{
			iter = vecGroundObj.erase(iter);

			if (m_bPlatform)
				pPlayerScript->RemoveUnitState(UNIT_GROUND_PLATFORM);
			else
				pPlayerScript->RemoveUnitState(UNIT_GROUND);

			if (vecGroundObj.empty())
			{
				CGameObject* pColObj = _other->GetOwner();
				pColObj->Rigidbody2D()->SetGround(false);
			}
		}
		else
		{
			++iter;
		}
	}

	m_bGround = false;
}

void CBlockScript::UpCollision(CCollider2D* _other)
{
	CUnitScript* pUnitScr = m_pColObj->GetScript<CUnitScript>();
	pUnitScr->PushGroundObj((this->GetOwner()));
	m_pColObj->Rigidbody2D()->SetGround(true);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjPos.x, m_vObjColScale.y / 2.f + m_vBlockColPos.y + m_vBlockColScale.y / 2.f -0.00001f, 0.f));
	m_bGround = true;

	CUnitScript* pPlayerScript = _other->GetOwner()->GetScript<CUnitScript>();
	if (m_bPlatform && pPlayerScript)
	{
		pPlayerScript->AddUnitState(UNIT_GROUND_PLATFORM);
	}
	else
	{
		pPlayerScript->AddUnitState(UNIT_GROUND);
	}
}

void CBlockScript::DownCollision()
{
	m_pColObj->Rigidbody2D()->SetForceSpeedY(0.f);
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vObjPos.x, m_vBlockColPos.y - m_vObjColScale.y / 2.f - m_vBlockColScale.y / 2.f - 0.1f, 0.f));
}

void CBlockScript::LeftCollision()
{
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockColPos.x - m_vBlockColScale.x / 2.f - m_vObjColScale.x / 2.f - 0.01f, m_vObjPos.y, 0.f));
}

void CBlockScript::RightCollision()
{
	m_pColObj->Transform()->SetRelativePos(Vec3(m_vBlockColPos.x + m_vBlockColScale.x / 2.f + m_vObjColScale.x / 2.f + 0.01f, m_vObjPos.y, 0.f));
}

void CBlockScript::SetMemberData(CCollider2D* _other)
{
	m_vBlockPos = Transform()->GetRelativePos();
	m_vBlockScale = Transform()->GetRelativeScale();

	m_vBlockColPos = Collider2D()->GetFinalPos();
	m_vBlockColScale = Collider2D()->GetFinalScale();

	m_pColObj = _other->GetOwner();
	
	m_vObjPos = m_pColObj->Transform()->GetRelativePos();
	m_vObjScale = m_pColObj->Transform()->GetRelativeScale();

	m_vObjColPos = m_pColObj->Collider2D()->GetFinalPos();
	m_vObjColScale = m_pColObj->Collider2D()->GetFinalScale();;
}

bool CBlockScript::IsUnit(CCollider2D* _other)
{
	CGameObject* pObj = _other->GetOwner();
	if (pObj->GetScript<CUnitScript>())
		return true;
	return false;
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

