#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript()
	: CScript((int)SCRIPT_TYPE::UNITSCRIPT)
{
}

CUnitScript::CUnitScript(int _ScriptType)
	: CScript(_ScriptType)
{
}

CUnitScript::~CUnitScript()
{
}



void CUnitScript::tick()
{

}


void CUnitScript::BeginOverlap(CCollider2D* _other)
{
}

void CUnitScript::Overlap(CCollider2D* _other)
{
}

void CUnitScript::EndOverlap(CCollider2D* _other)
{
}


void CUnitScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CUnitScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
