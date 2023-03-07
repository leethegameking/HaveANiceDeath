#include "pch.h"
#include "CParticleEnder.h"

CParticleEnder::CParticleEnder()
	: CScript((int)SCRIPT_TYPE::PARTICLEENDER)
	, m_fMaxTime(2.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Life Time", &m_fMaxTime);
}

CParticleEnder::CParticleEnder(const CParticleEnder& _origin)
	: CScript(_origin)
	, m_fMaxTime(_origin.m_fMaxTime)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Life Time", &m_fMaxTime);
}

CParticleEnder::~CParticleEnder()
{
}

void CParticleEnder::begin()
{
}

void CParticleEnder::tick()
{
	if (m_fMaxTime <= m_fAccTime)
	{
		Destroy();
	}
	else
	{
		m_fAccTime += DT;
	}
}

void CParticleEnder::BeginOverlap(CCollider2D* _pOther)
{
}

void CParticleEnder::Overlap(CCollider2D* _pOther)
{
}

void CParticleEnder::EndOverlap(CCollider2D* _pOther)
{
}

void CParticleEnder::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_fMaxTime, sizeof(float), 1, _pFile);
}

void CParticleEnder::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_fMaxTime, sizeof(float), 1, _pFile);
}



