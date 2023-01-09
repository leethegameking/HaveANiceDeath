#include "pch.h"
#include "CProjectileScript.h"

#include <Engine/CGameObject.h>

CProjectileScript::CProjectileScript()
	: CScript((int)SCRIPT_TYPE::PROJECTILESCRIPT)
	, m_fSpeed(10.f)
	, m_fMaxLifeTime(3.f)
	, m_fAccLifeTime(0.f)
	, m_vDir(Vec2::Zero)
	, m_bRot(false)
	, m_bFirstTick(true)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed    ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max Life ", &m_fMaxLifeTime);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "Rot      ", &m_bRot);
}

CProjectileScript::~CProjectileScript()
{
}



void CProjectileScript::begin()
{
}

void CProjectileScript::tick()
{
	if (m_bFirstTick)
		FirstTick();

	// 사라짐
	m_fAccLifeTime += DT;
	if (m_fMaxLifeTime <= m_fAccLifeTime)
	{
		GetOwner()->Destroy();
	}

	// 이동
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += m_vDir.x * m_fSpeed * DT;
	vPos.y += m_vDir.y * m_fSpeed * DT;
	Transform()->SetRelativePos(vPos);

	// AttCol 크기 오프셋 설정
	Ptr<CAnimation2D> pAnim = Animator2D()->GetCurAnim();
	int iIdx = pAnim->GetCurIdx();
	const vector<tAnim2DFrm>& vecFrm = pAnim->GetFrmVecRef();
	if (vecFrm[iIdx].bColiiderOn)
	{
		// offset
		m_pAttObj->Collider2D()->SetOffsetPos(vecFrm[iIdx].iColliderPos);
		m_pAttObj->Collider2D()->SetScale(vecFrm[iIdx].iColliderScale);
	}
	else
	{
		m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * m_pAttObj->GetLayerIdx());
	}
}

void CProjectileScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CProjectileScript::Overlap(CCollider2D* _pOther)
{
}

void CProjectileScript::EndOverlap(CCollider2D* _pOther)
{
}

void CProjectileScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CProjectileScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}

void CProjectileScript::FirstTick()
{
	if (m_bRot)
	{
		m_vDir.Normalize();
		float fTheta = acosf(m_vDir.Dot(Vec2(0.f, 0.f)));

		Transform()->SetRelativeRotation(0.f, 0.f, fTheta);
	}
	else
	{
		if (m_vDir.x <= 0.f)
		{
			m_vDir = Vec2(-1.f, 0.f);
		}
		else
		{
			m_vDir = Vec2(1.f, 0.f);
		}
		Transform()->SetRelativeRotation(0.f, 0.f, m_vDir.x);
	}

	const vector<CGameObject*>& vecChildObj = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChildObj.size(); ++i)
	{
		if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::ENEMY_ATTACK)
		{
			m_pAttObj = vecChildObj[i];
		}
	}

	m_bFirstTick = false;
}


