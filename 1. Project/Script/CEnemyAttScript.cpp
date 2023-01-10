#include "pch.h"
#include "CEnemyAttScript.h"

CEnemyAttScript::CEnemyAttScript()
	: CAttScript((int)SCRIPT_TYPE::ENEMYATTSCRIPT)
{
}

CEnemyAttScript::CEnemyAttScript(const CEnemyAttScript& _origin)
	: CAttScript(_origin)
{
}

CEnemyAttScript::~CEnemyAttScript()
{
}

void CEnemyAttScript::begin()
{
	CAttScript::begin();
}

void CEnemyAttScript::tick()
{
	CAttScript::tick();
}

void CEnemyAttScript::SaveToFile(FILE* _pFile)
{
	CAttScript::SaveToFile(_pFile);
}

void CEnemyAttScript::LoadFromFile(FILE* _pFile)
{
	CAttScript::LoadFromFile(_pFile);
}

void CEnemyAttScript::BeginOverlap(CCollider2D* _other)
{
	CAttScript::BeginOverlap(_other);
}

void CEnemyAttScript::Overlap(CCollider2D* _other)
{
	CAttScript::Overlap(_other);
}

void CEnemyAttScript::EndOverlap(CCollider2D* _other)
{
	CAttScript::EndOverlap(_other);
}


