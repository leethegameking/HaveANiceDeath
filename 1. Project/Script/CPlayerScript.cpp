#include "pch.h"
#include "CPlayerScript.h"

wstring PlayerKey[PLAYER_ANIM_END] =
{
	L"PlayerIdle",
	L"PlayerIdleToRun",
	L"PlayerRun",
	L"PlayerRunToIdle",
	L"PlayerIdleUturn",
	L"PlayerRunUturn",
};

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
{
}



CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{

}

void CPlayerScript::tick()
{
}


void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}



void CPlayerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
