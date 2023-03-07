#include "pch.h"
#include "CBasicTextScript.h"

#include <Engine/SimpleText.h>

CBasicTextScript::CBasicTextScript()
	: CTextScript((int)SCRIPT_TYPE::BASICTEXTSCRIPT)
	, m_strText("Test")
	, m_eFormatType()
{
	m_strText.reserve(1000);
	AddScriptParam(SCRIPT_PARAM::STRING, "Text", (char*)m_strText.data());
	AddScriptParam(SCRIPT_PARAM::INT, "Format", &m_eFormatType);
}

CBasicTextScript::CBasicTextScript(int _iScriptType)
	: CTextScript(_iScriptType)
	, m_strText("Test")
	, m_eFormatType()
{
	m_strText.reserve(1000);
	AddScriptParam(SCRIPT_PARAM::STRING, "Text", (char*)m_strText.data());
	AddScriptParam(SCRIPT_PARAM::INT, "Format", &m_eFormatType);
}

CBasicTextScript::CBasicTextScript(const CBasicTextScript& _origin)
	: CTextScript(_origin)
	, m_strText(_origin.m_strText)
	, m_eFormatType(_origin.m_eFormatType)
{
	m_strText.reserve(1000);
	AddScriptParam(SCRIPT_PARAM::STRING, "Text", (char*)m_strText.data());
	AddScriptParam(SCRIPT_PARAM::INT, "Format", &m_eFormatType);
}

CBasicTextScript::~CBasicTextScript()
{
}

void CBasicTextScript::begin()
{
}

void CBasicTextScript::tick()
{
	tText tTx;
	tTx.eFormat = (TEXT_FORMAT)m_eFormatType;
	tTx.strText = StrToWstr(m_strText.data());
	tTx.vWorldPos = Transform()->GetWorldPos();
	tTx.bScreenPos = true;

	SimpleText::GetInst()->AddDrawVec(tTx);
}

void CBasicTextScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CBasicTextScript::Overlap(CCollider2D* _pOther)
{
}

void CBasicTextScript::EndOverlap(CCollider2D* _pOther)
{
}

void CBasicTextScript::SaveToFile(FILE* _pFile)
{
	CTextScript::SaveToFile(_pFile);
	static string strForSave;
	strForSave = m_strText.data();

	SaveStringToFile(strForSave, _pFile);
	fwrite(&m_eFormatType, sizeof(m_eFormatType), 1, _pFile);
}

void CBasicTextScript::LoadFromFile(FILE* _pFile)
{
	CTextScript::LoadFromFile(_pFile);
	LoadStringFromFile(m_strText, _pFile);
	fread(&m_eFormatType, sizeof(m_eFormatType), 1, _pFile);
}


