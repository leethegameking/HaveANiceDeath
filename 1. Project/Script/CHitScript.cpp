#include "pch.h"
#include "CHitScript.h"

CHitScript::CHitScript()
	: CScript((int)SCRIPT_TYPE::HITSCRIPT)
{
}

CHitScript::~CHitScript()
{
}


void CHitScript::begin()
{
}

void CHitScript::tick()
{
}

void CHitScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CHitScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}

void CHitScript::BeginOverlap(CCollider2D* _other)
{
}

void CHitScript::Overlap(CCollider2D* _other)
{
}

void CHitScript::EndOverlap(CCollider2D* _other)
{
}

