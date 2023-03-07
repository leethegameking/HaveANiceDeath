#include "pch.h"
#include "CEffectScaler.h"

CEffectScaler::CEffectScaler()
	: CScript((int)SCRIPT_TYPE::EFFECTSCALER)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Option", &m_iOption);

	AddScriptParam(SCRIPT_PARAM::VEC3, "Start-Scale ", &m_vScaleStart);
	AddScriptParam(SCRIPT_PARAM::VEC3, "Middle-Scale ", &m_vScaleMiddle);
	AddScriptParam(SCRIPT_PARAM::VEC3, "End-Scale ", &m_vScaleEnd);

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Middle-Time", &m_fMiddleTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Middle-Delay-Time", &m_fMiddleDelayTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max-Time", &m_fMaxTime);
}

CEffectScaler::CEffectScaler(const CEffectScaler& _origin)
	: CScript(_origin)
	, m_vScaleStart(_origin.m_vScaleStart)
	, m_vScaleMiddle(_origin.m_vScaleMiddle)
	, m_vScaleEnd(_origin.m_vScaleEnd)
	, m_fMiddleTime(_origin.m_fMiddleTime)
	, m_fMiddleDelayTime(_origin.m_fMiddleDelayTime)
	, m_fMaxTime(_origin.m_fMaxTime)
	, m_iOption(_origin.m_iOption)

{
	AddScriptParam(SCRIPT_PARAM::INT, "Option", &m_iOption);

	AddScriptParam(SCRIPT_PARAM::VEC3, "Start-Scale ", &m_vScaleStart);
	AddScriptParam(SCRIPT_PARAM::VEC3, "Middle-Scale ", &m_vScaleMiddle);
	AddScriptParam(SCRIPT_PARAM::VEC3, "End-Scale ", &m_vScaleEnd);

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Middle-Time", &m_fMiddleTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Middle-Delay-Time", &m_fMiddleDelayTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Max-Time", &m_fMaxTime);
}

CEffectScaler::~CEffectScaler()
{
}

void CEffectScaler::begin()
{
	Transform()->SetRelativeScale(m_vScaleStart);
	m_bDelay = false;
	if (m_iOption == (int)SCALER_OPT::START_END)
		m_bDelayFlag = false;
	else
		m_bDelayFlag = true;
}

void CEffectScaler::tick()
{
	switch (SCALER_OPT(m_iOption))
	{
	case SCALER_OPT::START_END:
		Start_End();
		break;

	case SCALER_OPT::START_MIDDLE_END:
		Start_Middle_End();
		break;
	}
}

void CEffectScaler::Start_End()
{
	if (m_fMaxTime <= m_fAccTime)
	{
		Destroy();
		m_fAccTime = 0.f;
	}
	else
	{
		float fRatio = m_fAccTime / m_fMaxTime;
		Transform()->SetRelativeScale(m_vScaleStart + (m_vScaleEnd - m_vScaleStart) * fRatio);
		m_fAccTime += DT;
	}
}

void CEffectScaler::Start_Middle_End()
{
	if (!m_bDelay)
	{
		if (m_fAccTime <= m_fMiddleTime)
		{
			float fRatio = m_fAccTime / m_fMiddleTime;
			Transform()->SetRelativeScale(m_vScaleStart + (m_vScaleMiddle - m_vScaleStart) * fRatio);
			m_fAccTime += DT;
		}
		else if (m_fAccTime <= m_fMaxTime)
		{
			if (m_bDelayFlag)
			{
				m_bDelay = true;
				m_bDelayFlag = false;
				return;
			}

			float fRatio = (m_fAccTime - m_fMiddleTime) / (m_fMaxTime - m_fMiddleTime);
			Transform()->SetRelativeScale(m_vScaleMiddle + (m_vScaleEnd - m_vScaleMiddle) * fRatio);
			m_fAccTime += DT;
		}
		else
		{
			Destroy();
			m_fAccTime = 0.f;
		}
	}

	if (m_bDelay)
	{
		if (m_fMiddleDelayTime <= m_fDelayAccTIme)
		{
			m_bDelay = false;
			m_fDelayAccTIme = 0.f;
		}
		else
		{
			m_fDelayAccTIme += DT;
		}
	}
}

void CEffectScaler::BeginOverlap(CCollider2D* _pOther)
{
}

void CEffectScaler::Overlap(CCollider2D* _pOther)
{
}

void CEffectScaler::EndOverlap(CCollider2D* _pOther)
{
}

void CEffectScaler::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);

	fwrite(&m_iOption, sizeof(int), 1, _pFile);
	
	fwrite(&m_vScaleStart, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScaleMiddle, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScaleEnd, sizeof(Vec3), 1, _pFile);

	fwrite(&m_fMiddleTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMiddleDelayTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxTime, sizeof(float), 1, _pFile);
}

void CEffectScaler::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);

	fread(&m_iOption, sizeof(int), 1, _pFile);

	fread(&m_vScaleStart, sizeof(Vec3), 1, _pFile);
	fread(&m_vScaleMiddle, sizeof(Vec3), 1, _pFile);
	fread(&m_vScaleEnd, sizeof(Vec3), 1, _pFile);

	fread(&m_fMiddleTime, sizeof(float), 1, _pFile);
	fread(&m_fMiddleDelayTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxTime, sizeof(float), 1, _pFile);
}



