﻿#include "pch.h"
#include "CProjectileScript.h"

#include <Engine/CGameObject.h>

CProjectileScript::CProjectileScript()
	: CScript((int)SCRIPT_TYPE::PROJECTILESCRIPT)
	, m_fSpeed(10.f)
	, m_fMaxLifeTime(3.f)
	, m_fAccLifeTime(0.f)
	, m_fAtt(3.f)
	, m_vDir(Vec2::Zero)
	, m_bRot(false)
	, m_bFirstTick(true)
	, m_bDestroyAnimFinsh(false)
	, m_bRepeat(true)
	, m_bAnimHasCol(true)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed        ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att          ", &m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max Life     ", &m_fMaxLifeTime);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "Rot          ", &m_bRot);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "AnimRepeat   ", &m_bRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "End-of-Anim  ", &m_bDestroyAnimFinsh);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "AnimHasCol   ", &m_bAnimHasCol);

}

CProjectileScript::CProjectileScript(const CProjectileScript& _origin)
	: CScript((int)SCRIPT_TYPE::PROJECTILESCRIPT)
	, m_fSpeed(_origin.m_fSpeed)
	, m_fMaxLifeTime(_origin.m_fMaxLifeTime)
	, m_fAccLifeTime(0.f)
	, m_fAtt(_origin.m_fAtt)
	, m_vDir(Vec2::Zero)
	, m_bRot(_origin.m_bRot)
	, m_bFirstTick(true)
	, m_bDestroyAnimFinsh(_origin.m_bDestroyAnimFinsh)
	, m_bRepeat(_origin.m_bRepeat)
	, m_bAnimHasCol(_origin.m_bAnimHasCol)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed        ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Att          ", &m_fAtt);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max Life     ", &m_fMaxLifeTime);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "Rot          ", &m_bRot);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "AnimRepeat   ", &m_bRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL,  "End-of-Anim  ", &m_bDestroyAnimFinsh);
	AddScriptParam(SCRIPT_PARAM::BOOL, "AnimHasCol   ", &m_bAnimHasCol);
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
	if (m_bDestroyAnimFinsh)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->Destroy();
		}
	}
	else
	{
		if (m_fMaxLifeTime <= m_fAccLifeTime)
		{
			GetOwner()->Destroy();
		}
		else
		{
			m_fAccLifeTime += DT;
		}
	}

	// 이동
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += m_vDir.x * m_fSpeed * DT;
	vPos.y += m_vDir.y * m_fSpeed * DT; 
	Transform()->SetRelativePos(vPos);


	// AttCol 크기 오프셋 설정
	if (m_bAnimHasCol)
	{
		Ptr<CAnimation2D> pAnim = Animator2D()->GetCurAnim();
		int iIdx = pAnim->GetCurIdx();
		const vector<tAnim2DFrm>& vecFrm = pAnim->GetFrmVecRef();
		if (vecFrm[iIdx].bColiiderOn)
		{
			// offset
			Collider2D()->SetOffsetPos(vecFrm[iIdx].iColliderPos);
			Collider2D()->SetScale(vecFrm[iIdx].iColliderScale);
		}
		else
		{
			Collider2D()->SetOffsetPos(EXPEL * GetOwner()->GetLayerIdx());
		}
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
	fwrite(&m_bDestroyAnimFinsh, sizeof(bool), 1, _pFile);
	fwrite(&m_bRepeat, sizeof(bool), 1, _pFile);
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_bRot, sizeof(bool), 1, _pFile);
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fAtt, sizeof(float), 1, _pFile);
	fwrite(&m_bAnimHasCol, sizeof(bool), 1, _pFile);
	fwrite(&m_bPenetrate, sizeof(bool), 1, _pFile);
}

void CProjectileScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_bDestroyAnimFinsh, sizeof(bool), 1, _pFile);
	fread(&m_bRepeat, sizeof(bool), 1, _pFile);
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_bRot, sizeof(bool), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fAtt, sizeof(float), 1, _pFile);
	fread(&m_bAnimHasCol, sizeof(bool), 1, _pFile);
	fread(&m_bPenetrate, sizeof(bool), 1, _pFile);
}

void CProjectileScript::FirstTick()
{
	if (m_bRot)
	{
		m_vDir.Normalize();
		float fTheta = acosf(m_vDir.Dot(Vec2(1.f, 0.f)));

		Transform()->SetRelativeRotation(0.f, 0.f, fTheta);
		if (m_vDir.y > 0.f)
		{
			Transform()->SetRelativeRotation(0.f, 0.f, fTheta);
			Collider2D()->SetRotationZ(fTheta);
		}
		else
		{
			Transform()->SetRelativeRotation(0.f, 0.f, -fTheta);
			Collider2D()->SetRotationZ(fTheta);
		}
	}
	else
	{
		if (m_vDir.x <= 0.f)
		{
			m_vDir = Vec2(-1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
		}
		else
		{
			m_vDir = Vec2(1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		}
	}

	if(Animator2D())
		Animator2D()->Play(m_strAnimKey, m_bRepeat);

	m_bFirstTick = false;
}

