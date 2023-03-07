#include "pch.h"
#include "CDamageText.h"
#include <Engine/SimpleText.h>

CDamageText::CDamageText()
	: CBasicTextScript((int)SCRIPT_TYPE::DAMAGETEXT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpeedY ", &m_fSpeedY);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Up-Time 1 ", &m_fUpTime_1);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Delay ", &m_fDelayTime);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Up-Time 2 ", &m_fUpTime_2);
}

CDamageText::CDamageText(const CDamageText& _origin)
	: CBasicTextScript(_origin)
	, m_fSpeedY(_origin.m_fSpeedY)
	, m_fUpTime_1(_origin.m_fUpTime_1)
	, m_fDelayTime(_origin.m_fDelayTime)
	, m_fUpTime_2(_origin.m_fUpTime_2)
{
}

CDamageText::~CDamageText()
{
}

void CDamageText::begin()
{
	m_fUpAccTime_1 = 0.f;
	m_fDelayAccTime = 0.f;
	m_fUpAccTime_2 = 0.f;
	m_eState = (int)DAMAGE_TEXT_STATE::UP_1;
	m_fAlpha = 1.f;
}

void CDamageText::tick()
{
	switch ((DAMAGE_TEXT_STATE)m_eState)
	{
	case DAMAGE_TEXT_STATE::UP_1:
	{
		StateUp_1();
	}
		break;

	case DAMAGE_TEXT_STATE::DELAY:
	{
		StateDelay();
	}
		break;

	case DAMAGE_TEXT_STATE::UP_2:
	{
		StateUp_2();
	}
		break;

	case DAMAGE_TEXT_STATE::END:
	{
		StateEnd();
	}
		break;

	}

	// Format 3 -> TEXT_DAMAGE
	tText tTx;
	tTx.eFormat = (TEXT_FORMAT)m_eFormatType;
	tTx.strText = StrToWstr(m_strText.data());
	tTx.vWorldPos = Transform()->GetWorldPos();
	tTx.bScreenPos = false;
	tTx.vColor = { 1.f, 1.f, 1.f, m_fAlpha };

	SimpleText::GetInst()->AddDrawVec(tTx);
}

void CDamageText::StateUp_1()
{
	if (m_fUpTime_1 <= m_fUpAccTime_1)
	{
		m_eState = (int)DAMAGE_TEXT_STATE::DELAY;
		m_fUpAccTime_1 = 0.f;
	}
	else
	{
		Transform()->AddRelativePos(Vec3(0.f, m_fSpeedY * DT, 0.f));
		m_fUpAccTime_1 += DT;
	}
}

void CDamageText::StateDelay()
{
	if (m_fDelayTime <= m_fDelayAccTime)
	{
		m_eState = (int)DAMAGE_TEXT_STATE::UP_2;
		m_fDelayAccTime = 0.f;
	}
	else
	{
		m_fDelayAccTime += DT;
	}
}

void CDamageText::StateUp_2()
{
	if (m_fUpTime_2 <= m_fUpAccTime_2)
	{
		m_eState = (int)DAMAGE_TEXT_STATE::END;
		m_fUpAccTime_2 = 0.f;
	}
	else
	{
		m_fAlpha = 1.f - m_fUpAccTime_2 / m_fUpTime_2;
		Transform()->AddRelativePos(Vec3(0.f, m_fSpeedY * DT, 0.f));
		m_fUpAccTime_2 += DT;
	}
}

void CDamageText::StateEnd()
{
	Destroy();
}

void CDamageText::BeginOverlap(CCollider2D* _pOther)
{
}

void CDamageText::Overlap(CCollider2D* _pOther)
{
}

void CDamageText::EndOverlap(CCollider2D* _pOther)
{
}

void CDamageText::SaveToFile(FILE* _pFile)
{
	CBasicTextScript::SaveToFile(_pFile);
	fwrite(&m_fSpeedY, sizeof(float), 1, _pFile);
	fwrite(&m_fUpTime_1, sizeof(float), 1, _pFile);
	fwrite(&m_fDelayTime, sizeof(float), 1, _pFile);
	fwrite(&m_fUpTime_2, sizeof(float), 1, _pFile);
}

void CDamageText::LoadFromFile(FILE* _pFile)
{
	CBasicTextScript::LoadFromFile(_pFile);
	fread(&m_fSpeedY, sizeof(float), 1, _pFile);
	fread(&m_fUpTime_1, sizeof(float), 1, _pFile);
	fread(&m_fDelayTime, sizeof(float), 1, _pFile);
	fread(&m_fUpTime_2, sizeof(float), 1, _pFile);
}

