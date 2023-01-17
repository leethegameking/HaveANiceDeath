#include "pch.h"
#include "CTextScript.h"

#include <Engine/SimpleText.h>

CTextScript::CTextScript()
	: CScript((int)SCRIPT_TYPE::TEXTSCRIPT)
{
}

CTextScript::CTextScript(int _iScriptType)
	: CScript(_iScriptType)
{
}

CTextScript::CTextScript(const CTextScript& _origin)
	: CScript(_origin)
{
}

CTextScript::~CTextScript()
{
}


void CTextScript::begin()
{
}

void CTextScript::tick()
{
}

void CTextScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CTextScript::Overlap(CCollider2D* _pOther)
{
}

void CTextScript::EndOverlap(CCollider2D* _pOther)
{
}

void CTextScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CTextScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}

