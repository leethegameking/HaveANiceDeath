#include "pch.h"
#include "CPlayerAttScript.h"

#include "CEnemyScript.h"
#include "CBasicTextScript.h"
#include "CProjectileScript.h"

#include "CCameraMgr.h"
#include "CMainCameraScript.h"

CPlayerAttScript::CPlayerAttScript()
	: CAttScript((int)SCRIPT_TYPE::PLAYERATTSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Damage Text ", &m_TextDamagePref);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX_ATT ", &m_FXAttPref);
}

CPlayerAttScript::CPlayerAttScript(const CPlayerAttScript& _origin)
	: CAttScript(_origin)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Damage Text ", &m_TextDamagePref);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX_ATT ", &m_FXAttPref);
}

CPlayerAttScript::~CPlayerAttScript()
{
}

void CPlayerAttScript::begin()
{
	CAttScript::begin();
}

void CPlayerAttScript::tick()
{
	CAttScript::tick();
}

void CPlayerAttScript::BeginOverlap(CCollider2D* _other)
{
	//CAttScript::BeginOverlap(_other);
	if (m_pUnitScr)
	{
		float pAtt = m_pUnitScr->GetUnitInfo().m_fAtt;
		CUnitScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CUnitScript>();
		if (pTargetUnit)
		{
			float& pTargetHP = pTargetUnit->GetUnitInfo().m_fHP;
			float  pTargetDef = pTargetUnit->GetUnitInfo().m_fDef;
			pTargetHP -= pAtt * m_fCoefficient - pTargetDef;


			// Damage Text 생성
			Vec3 vTargetPos = pTargetUnit->Transform()->GetWorldPos();

			float fInstPos = vTargetPos.y + 100.f;
			vTargetPos.y = fInstPos;

			CGameObject* pTextDamge = m_TextDamagePref->Instantiate();
			pTextDamge->GetScript<CBasicTextScript>()->SetText(to_string(int(pAtt * m_fCoefficient - pTargetDef)));
 			Instantiate(pTextDamge, vTargetPos);

			CGameObject* pFXAtt = m_FXAttPref->Instantiate();
			Vec2 vTargetColPos = _other->Collider2D()->GetFinalPos();
			Vec3 vPos = Vec3(vTargetColPos.x, vTargetColPos.y, 0.f);
			Instantiate(pFXAtt, vPos);

			//=======================================

			tCameraEvent evn;
			evn.eType = CAMERA_EVENT_TYPE::ATTACK;
			evn.fMaxTime = 0.3f;

			CCameraMgr::GetInst()->GetMainCamera()->GetScript<CMainCameraScript>()->SetEvent(evn);
		}
	}
	else if (m_pProjScr)
	{
		float pAtt = m_pProjScr->GetAtt();
		if (_other->GetOwner()->GetParent())
		{
			CUnitScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CUnitScript>();
			if (pTargetUnit)
			{
				float& pTargetHP = pTargetUnit->GetUnitInfo().m_fHP;
				float  pTargetDef = pTargetUnit->GetUnitInfo().m_fDef;
				pTargetHP -= pAtt * m_fCoefficient - pTargetDef;
			}
		}
	}



	CEnemyScript* pTargetUnit = _other->GetOwner()->GetParent()->GetScript<CEnemyScript>();
	pTargetUnit->FillFaintGauge(3.f);

	if (CalBit(pTargetUnit->GetPattern(), PATTERN_HIT_START | PATTERN_HIT_LOOP | PATTERN_HIT_END, BIT_LEAST_ONE))
	{
		static tKnockBack tKB;
		tKB.bOn = true;

		/*tKB.fDist = GetOwner()->GetParent()->Animator2D()->GetCurAnim()->GetPosChange().x;*/
		tKB.fDist = 70.f;
		if (pTargetUnit->Transform()->GetRelativePos().x - GetOwner()->GetParent()->Transform()->GetRelativePos().x > 0.f)
			tKB.eKnockBackDir = ANIM_DIR::ANIM_RIGHT;
		else
			tKB.eKnockBackDir = ANIM_DIR::ANIM_LEFT;
		tKB.fDurationTime = 0.1f;
		pTargetUnit->SetKnockBack(tKB);
	}
}

void CPlayerAttScript::Overlap(CCollider2D* _other)
{
	CAttScript::Overlap(_other);
}

void CPlayerAttScript::EndOverlap(CCollider2D* _other)
{
	CAttScript::EndOverlap(_other);
}

void CPlayerAttScript::SaveToFile(FILE* _pFile)
{
	CAttScript::SaveToFile(_pFile);
	SaveResourceRef(m_TextDamagePref, _pFile);
	SaveResourceRef(m_FXAttPref, _pFile);
}

void CPlayerAttScript::LoadFromFile(FILE* _pFile)
{
	CAttScript::LoadFromFile(_pFile);
	LoadResourceRef(m_TextDamagePref, _pFile);
	LoadResourceRef(m_FXAttPref, _pFile);
}

