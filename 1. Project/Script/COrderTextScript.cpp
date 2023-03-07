#include "pch.h"
#include "COrderTextScript.h"

#include <Engine/SimpleText.h>

COrderTextScript::COrderTextScript()
	: CBasicTextScript((int)SCRIPT_TYPE::ORDERTEXTSCRIPT)
{
}

COrderTextScript::COrderTextScript(const COrderTextScript& _origin)
	: CBasicTextScript(_origin)
{
}

COrderTextScript::~COrderTextScript()
{
}

void COrderTextScript::begin()
{
	m_strTmp = "";
	m_fNextCharTime = 0.025f;
	m_fAccTime = 0.f;
	m_strOutText = m_strText.data();
	m_iLength = 0;
	m_bTextOutEnd = false;
}

void COrderTextScript::tick()
{
	tText tTx;
	tTx.eFormat = (TEXT_FORMAT)m_eFormatType;

	if (m_strOutText.length() > m_iLength)
	{
		if (m_fNextCharTime <= m_fAccTime)
		{
			++m_iLength;
			m_fAccTime = 0.f;
		}
		else
		{
			m_fAccTime += DT;
		}
	}
	else
	{
		m_bTextOutEnd = true;
	}

	m_strTmp = m_strOutText.substr(0, m_iLength);
	tTx.strText = StrToWstr(m_strTmp);
	tTx.vWorldPos = Transform()->GetWorldPos();
	tTx.bScreenPos = true;

	SimpleText::GetInst()->AddDrawVec(tTx);
}

void COrderTextScript::BeginOverlap(CCollider2D* _pOther)
{
}

void COrderTextScript::Overlap(CCollider2D* _pOther)
{
}

void COrderTextScript::EndOverlap(CCollider2D* _pOther)
{
}

void COrderTextScript::SaveToFile(FILE* _pFile)
{
	CBasicTextScript::SaveToFile(_pFile);
}

void COrderTextScript::LoadFromFile(FILE* _pFile)
{
	CBasicTextScript::LoadFromFile(_pFile);
}


